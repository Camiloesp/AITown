// Fill out your copyright notice in the Description page of Project Settings.


#include "Vehicles/Car.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

// Enhanced Input
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "ChaosWheeledVehicleMovementComponent.h"
#include "ChaosVehicleMovementComponent.h"


// Custom classes
#include "Inputs/CharacterInputConfigData.h"

// Sets default values
ACar::ACar()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Spring Arm"));
	SpringArm->SetupAttachment(GetMesh());
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritYaw = false;
	SpringArm->bInheritRoll = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera View"));
	Camera->SetupAttachment(SpringArm);
}

void ACar::OnConstruction(const FTransform& Transform)
{
	if (!VehicleTypesTable) return;

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


	SetupVehicleMovementComponent();


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

}

// Called to bind functionality to input
void ACar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	ensureMsgf(InputActions, TEXT("InputActions is empty. It should be assigned at %s BP"), *GetClass()->GetFName().ToString());

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//EnhancedInputComponent->BindAction(InputActions->InputJump, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		//EnhancedInputComponent->BindAction(InputActions->InputJump, ETriggerEvent::Completed, this, &ACharacter::StopJumping);


		EnhancedInputComponent->BindAction(InputActions->InputMove, ETriggerEvent::Triggered, this, &ACar::Move);
		EnhancedInputComponent->BindAction(InputActions->InputLook, ETriggerEvent::Triggered, this, &ACar::Look);
	}
}

void ACar::Move(const FInputActionValue& Value)
{
}

void ACar::Look(const FInputActionValue& Value)
{
}

void ACar::SetupVehicleMovementComponent()
{
	/*
		TODO:
			- Data driven design to create different types of vehicles
	*/
	if (!VehicleTypesTable) return;
	VehicleInformation = *(VehicleTypesTable->FindRow<FVehicleData>(VehicleName, ""));

	/**
	*	All vehicles skeletal meshes should have "wheel" prefix in their wheel bones to automatically detect the number of wheels and dynamically create them.
	*/
	UChaosWheeledVehicleMovementComponent* ChaosVehicleMovementComponent = Cast<UChaosWheeledVehicleMovementComponent>(GetVehicleMovement());
	if (!ChaosVehicleMovementComponent) return;
	USkeletalMeshComponent* CarMesh = GetMesh();
	if (!CarMesh) return;

	for (FName WheelBoneName : CarMesh->GetAllSocketNames())
	{
		if (WheelBoneName.ToString().Contains("wheel"))
		{
			// Setup wheel. Front wheel first?
			FChaosWheelSetup ChaosWheelSetup = FChaosWheelSetup();
			ChaosWheelSetup.WheelClass = UChaosVehicleWheel::StaticClass();
			ChaosWheelSetup.BoneName = WheelBoneName;

			ChaosVehicleMovementComponent->WheelSetups.Add(ChaosWheelSetup);
		}
	}

	
	ChaosVehicleMovementComponent->EngineSetup.TorqueCurve.ExternalCurve = VehicleInformation.TorqueCurve;
}

TArray<FName> ACar::GetVehicleTypes() const
{
	if(!VehicleTypesTable) return TArray<FName>();
	return VehicleTypesTable->GetRowNames();
}
