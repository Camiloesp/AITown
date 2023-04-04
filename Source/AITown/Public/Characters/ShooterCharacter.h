// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Inputs/CharacterInputConfigData.h"
#include "ShooterCharacter.generated.h"

class UInputMappingContext;
class UCameraComponent;
class USpringArmComponent;

UCLASS()
class AITOWN_API AShooterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AShooterCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:

protected:

	void Move( const FInputActionValue& Value );
	void Look( const FInputActionValue& Value );

private:
	UPROPERTY( EditDefaultsOnly )
	USpringArmComponent* SpringArm;

	UPROPERTY( EditDefaultsOnly )
	UCameraComponent* Camera;



	UPROPERTY( EditDefaultsOnly, Category = "00 - Enhanced Input" )
	UInputMappingContext* InputMapping;

	UPROPERTY(EditAnywhere, Category = "00 - Enhanced Input" )
	UCharacterInputConfigData* InputActions;
};
