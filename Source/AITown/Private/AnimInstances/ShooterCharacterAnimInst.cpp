// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstances/ShooterCharacterAnimInst.h"
#include "Characters/ShooterCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "KismetAnimationLibrary.h"

void UShooterCharacterAnimInst::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	ShooterCharacter = !ShooterCharacter ? Cast<AShooterCharacter>( TryGetPawnOwner() ) : ShooterCharacter;
}

void UShooterCharacterAnimInst::NativeUpdateAnimation( float DeltaTime )
{
	Super::NativeUpdateAnimation( DeltaTime );

	ShooterCharacter = !ShooterCharacter ? Cast<AShooterCharacter>( TryGetPawnOwner() ) : ShooterCharacter;
	if (!ShooterCharacter) return;

	// Set speed.
	FVector Velocity = ShooterCharacter->GetVelocity();
	Velocity.Z = 0.f;
	Speed = Velocity.Size();

	// Set Direction
	Direction = UKismetAnimationLibrary::CalculateDirection( ShooterCharacter->GetVelocity(), ShooterCharacter->GetActorRotation() );//CalculateDirection( ShooterCharacter->GetVelocity(), ShooterCharacter->GetActorRotation() );

	// Set Jump
	bPressedJump = ShooterCharacter->IsTryingToJump() && ShooterCharacter->GetCharacterMovement()->IsJumpAllowed();
	bIsFalling = ShooterCharacter->GetCharacterMovement()->IsFalling();

	// Aim offset
	FRotator BaseAimRotation = ShooterCharacter->GetBaseAimRotation();
	FRotator NormalizedDeltaRotator = BaseAimRotation - ShooterCharacter->GetActorRotation();
	NormalizedDeltaRotator.Normalize();
	AO_Pitch = NormalizedDeltaRotator.Pitch;
	AO_Yaw = NormalizedDeltaRotator.Yaw;

	// Head aim offset to look for items of interest. If AI to look at the player too. Or maybe for the player to see other entities?
	AActor* InteractableObject = Cast<AActor>(ShooterCharacter->GetCurrentInteractableObject());
	if (InteractableObject)// && ShooterCharacter->IsPlayerControlled())
	{
		//FVector HeadSocketLocation = ShooterCharacter->GetMesh()->GetSocketLocation( "neck_01"); // interp head loc to interactable
		//HeadLook = FMath::VInterpTo( HeadLook, InteractableObject->GetActorLocation(), DeltaTime, 10.f );

		bLookingAtSomething = true;
		HeadLook = InteractableObject->GetActorLocation();
	}
	else
	{
		HeadLook = ShooterCharacter->GetMesh()->GetSocketLocation( "neck_01" );
		bLookingAtSomething = false;
		//HeadLook = FVector::ZeroVector;
	}
}
