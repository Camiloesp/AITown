// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AIEntityController.generated.h"

class UAIPerceptionComponent;
struct FAIStimulus;

/**
 * 
 */
UCLASS()
class AITOWN_API AAIEntityController : public AAIController
{
	GENERATED_BODY()

public:

	AAIEntityController();

protected:

	virtual void BeginPlay() override;
	
	/* AAIEntityController */

public:
protected:
private:

	//AAIController::PerceptionComponent already exists.
	UPROPERTY()
	AActor* CombatTarget;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true) )
	UBehaviorTree* BehaviorTree;

	UFUNCTION()
	void OnPerception( AActor* Actor, FAIStimulus Stimulos );
};


/*
	What I want the AI to do:
	- Citizens with jobs. Cashiers/FrontDesk, Drivers, Cops, Delinquents. If no job just have them walk around point of interests.
	- Citizens get scared if violence nearby. Can they see player carrying weapons? They constantly run away from danger. Timer to go back to normal.
	- Cops try to arrest player if player did something bad. Speed up, Crash, Cought stealing. They shoot to kill if player is using weapons. Timer to forget about player if hidding.
	- AI cars that drive around the map. Do they park and exit the vehicles? Obey traffic?
	- Day/Night cycle behavior.
	- Make AIs not being able to push cars.
	
	More AI Ideas:
	- Dragon. Keep it happy to prevent it from attacking town. (Needs to be spawned from menu.)
	- Zombie apocalipse. Infects, passing ZombieController to other citizens. (Needs to be spawned from menu.)
	- barbarians vs samurais, or something like that? Player choses side or not to chose side. They fight all around town. (Needs to be spawned from menu.)



	Implementation:

	- Each job occupation has interest points where jobs will get done. Stay in point of interest or go to the next one? Interest point system?
		- Start with regular behavior.
		- Different behavior trees for each occupation ?
		- Similar behaviors: MoveTo, Focus, GoToWork, ReactToDanger, Drive(Maybe behavior tree for cars), NightTimeBehavior
	- Hearing sense to hear gunshot and reaction of finding death bodies. Adding weapons to seeing sense. Reaction types like coward or stand your ground?
	- Cops have different states when trying to deal with danger. 
		- Level 1 they try approaching target and stay there for a few seconds to arrest them.
		- Level 2 they shoot to kill.
		- Level 3 They use new weapons and are more tactical.
	- AI cars system to drive around roads on their respective side of the road. If police cars, they are allowed to offroad to follow players
	- Day/Night cycle behavior



	Interest Point:
	- Occupation Enum.
	- Reference characters for this point. array?
		- Interest point has reference to all actors with the same occupation.
	- Points availability? if not available AIs go enjoy day. (MAYBE)
*/
