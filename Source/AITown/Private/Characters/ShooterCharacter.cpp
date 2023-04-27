// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/ShooterCharacter.h"

#include "EnhancedInputSubsystems.h" // BindAction
#include "EnhancedInputComponent.h"   // UEnhancedInputComponent
#include "Components/InputComponent.h" // InputAction

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Perception/AIPerceptionStimuliSourceComponent.h"

#include "Interfaces/InteractInferface.h"
#include "Weapons/Weapon.h"

// Sets default values
AShooterCharacter::AShooterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>( TEXT("Spring Arm"));
	SpringArm->SetupAttachment( GetRootComponent() );
	SpringArm->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>( TEXT("Camera"));
	Camera->SetupAttachment( SpringArm );

	GetCapsuleComponent()->SetCollisionResponseToChannel( ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Overlap );
	GetCapsuleComponent()->SetCollisionResponseToChannel( ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Overlap );

	GetMesh()->SetCollisionResponseToChannel( ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Overlap );


	AIPerceptionStimuliSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>( TEXT("AIPerceptionStimuliSource"));

	//IGenericTeamAgentInterface
	SetGenericTeamId( FGenericTeamId( 1 ) );
}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = Cast<APlayerController>( Controller );
	if (PlayerController)
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>( PlayerController->GetLocalPlayer() ))
		{
			Subsystem->AddMappingContext( InputMapping, 0 );
		}
	}

	// Damage binding
	if (HasAuthority())
	{
		OnTakeAnyDamage.AddDynamic( this, &AShooterCharacter::ReceiveDamage );
	}
}

// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>( PlayerInputComponent ))
	{
		/* Basic movement */
		EnhancedInputComponent->BindAction( InputActions->InputMove, ETriggerEvent::Triggered, this, &AShooterCharacter::Move );
		EnhancedInputComponent->BindAction( InputActions->InputLook, ETriggerEvent::Triggered, this, &AShooterCharacter::Look );
		EnhancedInputComponent->BindAction( InputActions->InputSpace, ETriggerEvent::Triggered, this, &AShooterCharacter::JumpButtonPressed );

		/* Interact action */
		EnhancedInputComponent->BindAction( InputActions->InputAction, ETriggerEvent::Triggered, this, &AShooterCharacter::Action );
		
		/* Combat actions */
		EnhancedInputComponent->BindAction( InputActions->InputFire, ETriggerEvent::Triggered, this, &AShooterCharacter::FireButtonPressed );
	}
}

void AShooterCharacter::Move( const FInputActionValue& Value )
{
	if (Controller != nullptr)
	{
		const FVector2D MoveValue = Value.Get<FVector2D>();
		const FRotator MovementRotation( 0, Controller->GetControlRotation().Yaw, 0 );

		// Forward/Backward direction
		if (MoveValue.Y != 0.f)
		{
			// Get forward vector
			const FVector Direction = MovementRotation.RotateVector( FVector::ForwardVector );

			AddMovementInput( Direction, MoveValue.Y );
		}

		// Right/Left direction
		if (MoveValue.X != 0.f)
		{
			// Get right vector
			const FVector Direction = MovementRotation.RotateVector( FVector::RightVector );

			AddMovementInput( Direction, MoveValue.X );
		}
	}
}

void AShooterCharacter::Look( const FInputActionValue& Value )
{
	if (Controller != nullptr)
	{
		const FVector2D LookValue = Value.Get<FVector2D>();

		if (LookValue.X != 0.f)
		{
			AddControllerYawInput( LookValue.X );
		}

		if (LookValue.Y != 0.f)
		{
			AddControllerPitchInput( LookValue.Y );
		}
	}
}

void AShooterCharacter::JumpButtonPressed( const FInputActionValue& Value )
{
	bJumpButtonPressed = Value.Get<bool>();
	if (bJumpButtonPressed)
	{
		Jump();
	}
}

void AShooterCharacter::Action( const FInputActionValue& Value )
{
	// If there is a current interactable object, call its Interact function
	if (CurrentInteractableObject.IsValid())
	{
		IInteractInferface* Interactable = Cast<IInteractInferface>( CurrentInteractableObject.Get() );
		if (Interactable)
		{
			Interactable->Interact( this );
		}
	}
}

void AShooterCharacter::FireButtonPressed( const FInputActionValue& Value )
{
	bFireButtonPressed = Value.Get<bool>();

	if (EquippedWeapon && bFireButtonPressed)
	{
		EquippedWeapon->FireWeapon();
	}
}

void AShooterCharacter::ReceiveDamage( AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatorController, AActor* DamageCauser )
{
	UpdateHealth( Health, -Damage, MaxHealth );
}

void AShooterCharacter::HealthDepleted()
{
	// TODO: Death sound? UGHHHHHH ?
	// TODO: After a few seconds dematerialize and destroy

	if (bUseRagdollDeath)
	{
		GetMesh()->SetCollisionProfileName( FName( "Ragdoll" ) );
		GetMesh()->SetSimulatePhysics( true );
		GetCharacterMovement()->DisableMovement();

		SpringArm->bDoCollisionTest = false;

		return;
	}
	else
	{
		if (DeathMontage)
		{
			int RandomIndex = FMath::RandRange( 0, DeathMontageSectionNames.Num() - 1 );
			PlayAnimMontage( DeathMontage, 1.f, DeathMontageSectionNames[RandomIndex] );
		}
	}

	// TODO: Remove capsule's collision
	/*GetCapsuleComponent()->SetCollisionResponseToChannel( ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore );
	GetCapsuleComponent()->SetCollisionEnabled( ECollisionEnabled::NoCollision );
	GetCapsuleComponent()->SetCollisionResponseToAllChannels( ECollisionResponse::ECR_Ignore );*/
}

void AShooterCharacter::GiveWeapon( AWeapon* WeaponToEquipp )
{
	IInteractInferface* Interactable = Cast<IInteractInferface>( WeaponToEquipp );
	if (Interactable)
	{
		Interactable->Interact( this );
	}
}

void AShooterCharacter::SetCurrentInteractableObject( UObject* InteractableObject )
{
	// Set the current interactable object
	CurrentInteractableObject = InteractableObject;
}

void AShooterCharacter::ClearCurrentInteractableObject()
{
	// Clear the current interactable object
	CurrentInteractableObject.Reset();
}

