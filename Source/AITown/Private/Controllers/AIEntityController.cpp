// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/AIEntityController.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIs/AICharacter.h"

#include "Characters/ShooterCharacter.h"

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

	//IGenericTeamAgentInterface
	SetGenericTeamId( FGenericTeamId(1) );
}

void AAIEntityController::BeginPlay()
{
	Super::BeginPlay();

	RunBehaviorTree( BehaviorTree );
	PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic( this, &AAIEntityController::OnPerception );
}

void AAIEntityController::OnPerception( AActor* Actor, FAIStimulus Stimulos )
{
	/*
	* Same team Id is friendly
	* if no team or team != Team id then it is hostile.
	* if does not have the IGenericTeamAgentInterface, then neutral.
	*/

	/*
	* https://www.youtube.com/watch?v=PW1pwD0Kib4
	*/
	AShooterCharacter* Player = Cast<AShooterCharacter>( Actor );
	if (Player)
	{
		ETeamAttitude::Type TeamAttitude = GetTeamAttitudeTowards( *Actor );
		switch (TeamAttitude)
		{
			case ETeamAttitude::Friendly:
				UE_LOG(LogTemp, Error, TEXT("Friendly"));
				break;
			case ETeamAttitude::Hostile:
				UE_LOG( LogTemp, Error, TEXT( "Hostile" ) );
				// Run to safety.
				break;
			case ETeamAttitude::Neutral:
				UE_LOG( LogTemp, Error, TEXT( "Neutral" ) );
				break;
		}

		//ShooterCharacter = !ShooterCharacter ? Cast<AShooterCharacter>( TryGetPawnOwner() ) : ShooterCharacter;
		ControlledPawn = !ControlledPawn ? Cast<AAICharacter>( GetCharacter() ) : ControlledPawn;
		ControlledPawn->SetCurrentInteractableObject( Player );
	}

	bool bCanSeePlayer = Stimulos.WasSuccessfullySensed();
	if (bCanSeePlayer)
	{
		if (Player) {} // Set player BB to Player
		UE_LOG( LogTemp, Error, TEXT( "bCanSeePlayer" ) );
		//Blackboard->SetValueAsVector( FName("MoveToLocation"), Player->GetActorLocation() );
	}
	else
	{
		// Set player BB to nullptr
		UE_LOG( LogTemp, Error, TEXT( "!bCanSeePlayer" ) );
	}

	// TODO:
	// If we are neutral do nothing. Just look at the player pass and go back to work.
	// Check team, if enemy new target. SetFocus();
	// Head look around anim blend space to look at player when passing by.
	// SetFocus( Actor );
} 
