// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EnhancedInput/Public/InputAction.h"
#include "WheeledVehicleInputConfigData.generated.h"

/**
 * 
 */
UCLASS()
class AITOWN_API UWheeledVehicleInputConfigData : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* InputThrottle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* InputSteering;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* InputLookAround;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* InputBreak;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* InputHandBreak;
};
