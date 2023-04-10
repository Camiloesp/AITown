// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EnhancedInput/Public/InputAction.h"
#include "CharacterInputConfigData.generated.h"

/**
 * Store pointer to Input Action for native binding
*/
UCLASS()
class AITOWN_API UCharacterInputConfigData : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* InputMove;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* InputLook;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* InputSpace;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly )
	UInputAction* InputAction;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly )
	UInputAction* InputFire;
};
