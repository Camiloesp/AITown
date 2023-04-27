// Fill out your copyright notice in the Description page of Project Settings.


#include "AIs/PointOfInterest.h"
#include "Components/ArrowComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
APointOfInterest::APointOfInterest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PointOfInterestRadius = CreateDefaultSubobject<USphereComponent>(TEXT("PointOfInterestRadius"));
	SetRootComponent( PointOfInterestRadius );

	ForwardDirection = CreateDefaultSubobject<UArrowComponent>( TEXT( "ForwardDirection" ) );
	ForwardDirection->SetupAttachment( GetRootComponent() );
}

// Called when the game starts or when spawned
void APointOfInterest::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APointOfInterest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

