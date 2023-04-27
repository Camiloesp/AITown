// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/ShooterCharacter.h"
#include "AITownTypes.h"
#include "AICharacter.generated.h"

/**
 * 
 */
UCLASS()
class AITOWN_API AAICharacter : public AShooterCharacter
{
	GENERATED_BODY()
	
public:
	AAICharacter();

protected:

	virtual void BeginPlay() override;

	/* AAICharacter */
public:

	virtual void SetCurrentInteractableObject( UObject* InteractableObject );

protected:



private:

};
