// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "AITownTypes.h"
#include "TownGameMode.generated.h"

class APointOfInterest;

/**
 * 
 */
UCLASS()
class AITOWN_API ATownGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:

protected:

	virtual void BeginPlay() override;

	/* ATownGameMode */
public:

	void AddPointOfInterest( APointOfInterest* PointToAdd );

protected:
private:

	/* 
	* All point of interests according their OccupationType 
	*/
	UPROPERTY( VisibleAnywhere, Category = "00-AI" )
	TArray<APointOfInterest*> PointOfinterests;

	UPROPERTY( EditAnywhere, Category = "00-AI" )
	TSubclassOf<APointOfInterest> PointOfInterestClass;

	UPROPERTY( VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "01-DayNightCycle")
	EDayType DayType;

};
