// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ShooterCharacterAnimInst.generated.h"

class AShooterCharacter;

/**
 * 
 */
UCLASS()
class AITOWN_API UShooterCharacterAnimInst : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeInitializeAnimation() override;		// Like Beginplay. Called at at the start of the game, load UE and compile and so on.
	virtual void NativeUpdateAnimation( float DeltaTime ) override;	// Like Tick()

	/* UShooterCharacterAnimInst */

private:

	UPROPERTY()
	AShooterCharacter* ShooterCharacter;


	UPROPERTY( BlueprintReadOnly, meta = (AllowPrivateAccess = "true") )
	float Speed = 0.f;
	UPROPERTY( BlueprintReadOnly, meta = (AllowPrivateAccess = "true") )
	float Direction = 0.f;
	UPROPERTY( BlueprintReadOnly, meta = (AllowPrivateAccess = "true") )
	bool bPressedJump = false;
	UPROPERTY( BlueprintReadOnly, meta = (AllowPrivateAccess = "true") )
	bool bIsFalling = false;
	UPROPERTY( BlueprintReadOnly, meta = (AllowPrivateAccess = "true") )
	float AO_Yaw = 0.f;
	UPROPERTY( BlueprintReadOnly, meta = (AllowPrivateAccess = "true") )
	float AO_Pitch = 0.f;
	UPROPERTY( BlueprintReadOnly, meta = (AllowPrivateAccess = "true") )
	FVector HeadLook = FVector::ZeroVector;
	UPROPERTY( BlueprintReadOnly, meta = (AllowPrivateAccess = "true") )
	bool bLookingAtSomething = false;
};
