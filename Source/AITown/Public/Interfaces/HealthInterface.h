// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HealthInterface.generated.h"

/*
* 
* https://superyateam.com/2021/12/06/difference-between-delegates-in-ue4/
*
* DECLARE_DELEGATE(DelegateName)
* DECLARE_MULTICAST_DELEGATE(DelegateName)
* DECLARE_DYNAMIC_DELEGATE(DelegateName)
* DECLARE_DYNAMIC_MULTICAST_DELEGATE(DelegateName)

* Multicast Delegates: functions cannot use return values. They are best used to easily pass a collection of delegates around.
* Dynamic Delegates:  Functions can be found by name, and they are slower than regular delegates.
* 
*/

// On Update health. for taking damage and healing.
// On Health reached 0.f

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UHealthInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AITOWN_API IHealthInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual void UpdateHealth( float& CurrentHealth, float AddingHealth, float MaxHealthAmount );
	virtual void HealthDepleted() = 0;
};
