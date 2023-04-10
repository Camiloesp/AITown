// Fill out your copyright notice in the Description page of Project Settings.


#include "Interfaces/HealthInterface.h"

// Add default functionality here for any IHealthInterface functions that are not pure virtual.

void IHealthInterface::UpdateHealth( float& CurrentHealth, float AddingHealth, float MaxHealthAmount )
{
	if (CurrentHealth <= 0.f) return;

	CurrentHealth = FMath::Clamp( CurrentHealth + AddingHealth, 0.f, MaxHealthAmount );

	UE_LOG( LogTemp, Error, TEXT( "CurrentHealth: %f" ), CurrentHealth );
	if (CurrentHealth == 0.f)
	{
		HealthDepleted();
	}
}
