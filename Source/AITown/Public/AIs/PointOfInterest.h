// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AITownTypes.h"
#include "PointOfInterest.generated.h"

class USphereComponent;
class UArrowComponent;

UCLASS()
class AITOWN_API APointOfInterest : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APointOfInterest();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/* APointOfInterest */

public:
protected:
private:

	UPROPERTY( EditAnywhere )
	USphereComponent* PointOfInterestRadius;

	UPROPERTY( EditAnywhere )
	UArrowComponent* ForwardDirection;

	UPROPERTY( EditAnywhere )
	EOccupationType POIType;

	
public:

	FORCEINLINE EOccupationType GetOccupationType() const { return POIType; }
};
