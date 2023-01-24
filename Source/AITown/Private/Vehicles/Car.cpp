// Fill out your copyright notice in the Description page of Project Settings.


#include "Vehicles/Car.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

// Enhanced Input
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "ChaosWheeledVehicleMovementComponent.h"
#include "ChaosVehicleMovementComponent.h"
#include "ChaosVehicleWheel.h"

// Custom classes
#include "Inputs/WheeledVehicleInputConfigData.h"

// Sets default values
ACar::ACar()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetSimulatePhysics(true);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Spring Arm"));
	SpringArm->SetupAttachment(GetMesh());
	//SpringArm->bInheritPitch = false;
	//SpringArm->bInheritYaw = false;
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bInheritRoll = false;
	SpringArm->TargetArmLength = 600.f;
	//SpringArm->bEnableCameraRotationLag = true;
	//SpringArm->CameraRotationLagSpeed = 2.f;
	SpringArm->bDoCollisionTest = true; // Prevents clipping through walls.
	SpringArm->ProbeSize = 1.f;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera View"));
	Camera->SetupAttachment(SpringArm);


	VehicleMovementComponentRef = Cast<UChaosWheeledVehicleMovementComponent>(GetVehicleMovement());
	if (VehicleMovementComponentRef)
	{
		VehicleMovementComponentRef->bLegacyWheelFrictionPosition = true;
		VehicleMovementComponentRef->EngineSetup.EngineBrakeEffect = 0.2;

		VehicleMovementComponentRef->TransmissionSetup.ForwardGearRatios.Empty();
		VehicleMovementComponentRef->TransmissionSetup.ReverseGearRatios.Empty();

		VehicleMovementComponentRef->TransmissionSetup.ForwardGearRatios.Add(2.f);
		VehicleMovementComponentRef->TransmissionSetup.ForwardGearRatios.Add(1.5f);
		VehicleMovementComponentRef->TransmissionSetup.ForwardGearRatios.Add(1.f);
		VehicleMovementComponentRef->TransmissionSetup.ReverseGearRatios.Add(1.8);
		VehicleMovementComponentRef->TransmissionSetup.ChangeUpRPM = 3010.f;
		VehicleMovementComponentRef->TransmissionSetup.ChangeDownRPM = 1500.f;

		VehicleMovementComponentRef->SteeringSetup.SteeringType = ESteeringType::Ackermann;

		VehicleMovementComponentRef->bEnableCenterOfMassOverride = true;
		VehicleMovementComponentRef->CenterOfMassOverride = FVector(-25.f, 0.f, -10.f);
	}

}

void ACar::OnConstruction(const FTransform& Transform)
{
	if (VehicleName.IsNone()) return;
	if (!VehicleTypesTable) return;
	VehicleInformation = *(VehicleTypesTable->FindRow<FVehicleData>(VehicleName, ""));

	SetupVehicleMovementComponent();

	USkeletalMeshComponent* VehicleMeshComponent = GetMesh();

	// If name is None then remove mesh.
	if (VehicleName.IsNone())
	{
		if (VehicleMeshComponent) VehicleMeshComponent->SetSkeletalMesh(nullptr);
		return;
	}
	
	USkeletalMesh* VehicleMesh = VehicleTypesTable->FindRow<FVehicleData>(VehicleName, "")->VehicleMesh;
	if (!VehicleMesh) return;

	VehicleMeshComponent->SetSkeletalMesh(VehicleMesh);
}

// Called when the game starts or when spawned
void ACar::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* EnhancedInputLocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			EnhancedInputLocalPlayerSubsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

// Called every frame
void ACar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Damps rotation when in air
	USkeletalMeshComponent* CarMesh = GetMesh();
	if (!CarMesh && !VehicleMovementComponentRef) return;

	bool bIsMovingOnGround = VehicleMovementComponentRef->IsMovingOnGround();
	float NewAngularDamping = bIsMovingOnGround ? 0.f : 3.f;
	CarMesh->SetAngularDamping(NewAngularDamping);
}

// Called to bind functionality to input
void ACar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	ensureMsgf(InputActions, TEXT("InputActions is empty. It should be assigned at %s BP"), *GetClass()->GetFName().ToString());

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(InputActions->InputThrottle, ETriggerEvent::Triggered, this, &ACar::Throttle);
		EnhancedInputComponent->BindAction(InputActions->InputThrottle, ETriggerEvent::Completed, this, &ACar::Throttle);

		EnhancedInputComponent->BindAction(InputActions->InputSteering, ETriggerEvent::Triggered, this, &ACar::Steer);
		EnhancedInputComponent->BindAction(InputActions->InputSteering, ETriggerEvent::Completed, this, &ACar::Steer);

		EnhancedInputComponent->BindAction(InputActions->InputLookAround, ETriggerEvent::Triggered, this, &ACar::LookAround);

		EnhancedInputComponent->BindAction(InputActions->InputBreak, ETriggerEvent::Triggered, this, &ACar::Break);
		EnhancedInputComponent->BindAction(InputActions->InputBreak, ETriggerEvent::Completed, this, &ACar::CompletedBreak);

		EnhancedInputComponent->BindAction(InputActions->InputHandBreak, ETriggerEvent::Triggered, this, &ACar::HandBreak);
		EnhancedInputComponent->BindAction(InputActions->InputHandBreak, ETriggerEvent::Completed, this, &ACar::CompletedHandBreak);
	}
}

void ACar::Throttle(const FInputActionValue& Value)
{
	if (!VehicleMovementComponentRef) return;

	float FValue = Value.Get<float>();
	VehicleMovementComponentRef->SetThrottleInput(FValue);
}

void ACar::Steer(const FInputActionValue& Value)
{
	if (!VehicleMovementComponentRef) return;

	float FValue = Value.Get<float>();
	VehicleMovementComponentRef->SetSteeringInput(FValue);
}

void ACar::LookAround(const FInputActionValue& Value)
{
	if (Controller != nullptr)
	{
		const FVector2D LookValue = Value.Get<FVector2D>();

		if (LookValue.X != 0.f)
		{
			AddControllerYawInput(LookValue.X);
		}

		if (LookValue.Y != 0.f)
		{
			AddControllerPitchInput(LookValue.Y);
		}
	}
}

void ACar::Break(const FInputActionValue& Value)
{
	if (!VehicleMovementComponentRef) return;

	float FValue = Value.Get<float>();
	VehicleMovementComponentRef->SetBrakeInput(FValue);
}

void ACar::CompletedBreak(const FInputActionValue& Value)
{
	if (!VehicleMovementComponentRef) return;

	VehicleMovementComponentRef->SetBrakeInput(0.f);
}

void ACar::HandBreak(const FInputActionValue& Value)
{
	if (!VehicleMovementComponentRef) return;

	VehicleMovementComponentRef->SetHandbrakeInput(true);
}

void ACar::CompletedHandBreak(const FInputActionValue& Value)
{
	if (!VehicleMovementComponentRef) return;

	VehicleMovementComponentRef->SetHandbrakeInput(false);
}

void ACar::SetupVehicleMovementComponent()
{
	/**
	*	All vehicles skeletal meshes should have "wheel" prefix in their wheel bones to automatically detect the number of wheels and dynamically create them.
	*/
	VehicleMovementComponentRef->EngineSetup.TorqueCurve.ExternalCurve = VehicleInformation.TorqueCurve;

	USkeletalMeshComponent* CarMesh = GetMesh();
	if (!CarMesh) return;

	// Since its in the OnConstruction, make sure we dont keep adding wheels.
	VehicleMovementComponentRef->WheelSetups.Empty();
	for (FName WheelBoneName : CarMesh->GetAllSocketNames())
	{
		if (WheelBoneName.ToString().Contains(VehicleInformation.WheelPrefix))
		{
			bool bIsFrontWheel = WheelBoneName.ToString().Contains("F");

			// Setup wheel. Front wheel first?
			FChaosWheelSetup ChaosWheelSetup = FChaosWheelSetup();
			ChaosWheelSetup.WheelClass = bIsFrontWheel ? FrontWheel : RearWheel;
			ChaosWheelSetup.BoneName = WheelBoneName;
			
			VehicleMovementComponentRef->WheelSetups.Add(ChaosWheelSetup);
		}
	}
}

TArray<FName> ACar::GetVehicleTypes() const
{
	if(!VehicleTypesTable) return TArray<FName>();
	return VehicleTypesTable->GetRowNames();
}
