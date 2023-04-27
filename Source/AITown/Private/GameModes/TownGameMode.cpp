// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/TownGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "AIs/PointOfInterest.h"

void ATownGameMode::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> OutPoints;
	UGameplayStatics::GetAllActorsOfClass( this, PointOfInterestClass, OutPoints );

	for (int i = 0; i < OutPoints.Num(); i++)
	{
		if (APointOfInterest* Point = Cast<APointOfInterest>( OutPoints[i] ))
		{
			AddPointOfInterest( Point );
		}
	}
}

void ATownGameMode::AddPointOfInterest( APointOfInterest* PointToAdd )
{
	if (!PointToAdd) return;
	PointOfinterests.Add( PointToAdd );

	/*
	TODO"
https://benui.ca/unreal/iterate-over-enum-tenumrange/
	*/
}
