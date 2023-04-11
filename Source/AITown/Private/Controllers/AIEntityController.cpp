// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/AIEntityController.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionComponent.h"

AAIEntityController::AAIEntityController()
{
	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>( TEXT( "Perception Component" ) );

	// TODO: add more senses.

	UAISenseConfig_Sight* SightSense = CreateDefaultSubobject<UAISenseConfig_Sight>( TEXT( "Sight Sense Config" ) );
	SightSense->SightRadius = 1'000.f;
	SightSense->LoseSightRadius = 1'250.f;
	SightSense->PeripheralVisionAngleDegrees = 75.f;
	SightSense->DetectionByAffiliation.bDetectEnemies = true;
	SightSense->DetectionByAffiliation.bDetectFriendlies = true;
	SightSense->DetectionByAffiliation.bDetectNeutrals = true;

	// Adding sense to perception config
	PerceptionComponent->ConfigureSense( *SightSense );
	PerceptionComponent->SetDominantSense( SightSense->GetSenseImplementation() );
}

void AAIEntityController::BeginPlay()
{
	Super::BeginPlay();

	RunBehaviorTree( BehaviorTree );
	PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic( this, &AAIEntityController::OnPerception );
	//PerceptionComponent->OnTargetPerceptionInfoUpdated.AddDynamic( this, &AAIEntityController::OnPerception );
}

void AAIEntityController::OnPerception( AActor* Actor, FAIStimulus Stimulos )
{
	IGenericTeamAgentInterface* TeamAgentInterface = Cast<IGenericTeamAgentInterface>( Actor );
	if (TeamAgentInterface)
	{
		//TeamAgentInterface->GetGenericTeamId() == GetGenericTeamId();
		ETeamAttitude::Type TeamAttitude = GetTeamAttitudeTowards( *Actor );
		switch (TeamAttitude)
		{
			case ETeamAttitude::Friendly:
				UE_LOG(LogTemp, Error, TEXT("Friendly"));
				break;
			case ETeamAttitude::Hostile:
				UE_LOG( LogTemp, Error, TEXT( "Hostile" ) );
				break;
			case ETeamAttitude::Neutral:
				UE_LOG( LogTemp, Error, TEXT( "Neutral" ) );
				break;
		}
	}
	else
	{
		UE_LOG( LogTemp, Error, TEXT( "TeamAgentInterface failed" ) );
	}

	// TODO:
	// If we are neutral do nothing. Just look at the player pass and go back to work.
	// Check team, if enemy new target. SetFocus();
	// Head look around anim blend space to look at player when passing by.
	// SetFocus( Actor );
}
