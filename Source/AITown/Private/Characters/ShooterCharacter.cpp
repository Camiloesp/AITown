// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/ShooterCharacter.h"

#include "EnhancedInputSubsystems.h" // BindAction
#include "EnhancedInputComponent.h"   // UEnhancedInputComponent
#include "Components/InputComponent.h" // InputAction

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

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
}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>( Controller ))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>( PlayerController->GetLocalPlayer() ))
		{
			Subsystem->AddMappingContext( InputMapping, 0 );
		}
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
		EnhancedInputComponent->BindAction( InputActions->InputMove, ETriggerEvent::Triggered, this, &AShooterCharacter::Move );
		EnhancedInputComponent->BindAction( InputActions->InputLook, ETriggerEvent::Triggered, this, &AShooterCharacter::Look );
		EnhancedInputComponent->BindAction( InputActions->InputSpace, ETriggerEvent::Triggered, this, &ThisClass::Jump );
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

