// Fill out your copyright notice in the Description page of Project Settings.


#include "AIs/AICharacter.h"

AAICharacter::AAICharacter()
{
}

void AAICharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AAICharacter::SetCurrentInteractableObject( UObject* InteractableObject )
{
	if (AShooterCharacter* Player = Cast<AShooterCharacter>( InteractableObject ))
	{

	}
	else
	{
		Super::SetCurrentInteractableObject( InteractableObject ); // Sets interactable object focus and ready for pickup.
	}
}
