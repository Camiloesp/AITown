// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Inputs/CharacterInputConfigData.h"
#include "Interfaces/HealthInterface.h"
#include "AITownTypes.h"
#include "ShooterCharacter.generated.h"

class UInputMappingContext;
class UCameraComponent;
class USpringArmComponent;
class AWeapon;
class APlayerController;

UCLASS()
class AITOWN_API AShooterCharacter : public ACharacter, public IHealthInterface
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

	/* AShooterCharacter */
public:

	bool bFireButtonPressed = false;

	// Set the current interactable object
	void SetCurrentInteractableObject( UObject* InteractableObject );

	// Clear the current interactable object
	void ClearCurrentInteractableObject();

	/* Gives this character a weapon. Drops the current equipped weapon and gets a new one. */
	UFUNCTION( BlueprintCallable )
	virtual void GiveWeapon( AWeapon* WeaponToEquipp );

protected:

	/* Input functions */
	void Move( const FInputActionValue& Value );
	void Look( const FInputActionValue& Value );
	void JumpButtonPressed( const FInputActionValue& Value );
	void Action( const FInputActionValue& Value );
	void FireButtonPressed( const FInputActionValue& Value );

	/* Function signature from AActor::OnTakeAnyDamage */
	UFUNCTION()
	void ReceiveDamage( AActor* DamagedActor, float Damage, const UDamageType* DamageType, class AController* InstigatorController, AActor* DamageCauser );

	/* HealthInterface */
	virtual void HealthDepleted() override;

private:

	/* Default components */
	UPROPERTY( EditDefaultsOnly )
	USpringArmComponent* SpringArm;
	UPROPERTY( EditDefaultsOnly )
	UCameraComponent* Camera;

	/* Inputs */
	UPROPERTY( EditDefaultsOnly, Category = "00 - Enhanced Input" )
	UInputMappingContext* InputMapping;
	UPROPERTY(EditAnywhere, Category = "00 - Enhanced Input" )
	UCharacterInputConfigData* InputActions;

	UPROPERTY()
	APlayerController* PlayerController;

	/* Movemement */
	bool bJumpButtonPressed = false;

	/* Interaction references */
	UPROPERTY()
	TWeakObjectPtr<UObject> CurrentInteractableObject;
	UPROPERTY( EditAnywhere, Category = "01 - Interaction" )
	float InteractionDistance = 100.f;
	
	/* Weapon references */
	UPROPERTY( EditAnywhere )
	TSubclassOf<AWeapon> WeaponClass; // GunBPClass
	UPROPERTY()
	AWeapon* EquippedWeapon; // GunSpawn

	/* Health */
	UPROPERTY( EditAnywhere, Category = "02 - Health Interface" )
	float MaxHealth = 100.f;
	UPROPERTY( VisibleAnywhere, Category = "02 - Health Interface" )
	float Health = 100.f;

	/* Combat */
	UPROPERTY( EditAnywhere, Category = "03 - Combat" )
	bool bUseRagdollDeath = true;
	UPROPERTY( EditAnywhere, Category = "03 - Combat" )
	UAnimMontage* DeathMontage;
	UPROPERTY( EditAnywhere, Category = "03 - Combat" )
	TArray<FName> DeathMontageSectionNames;

	/* Player stats */
	UPROPERTY( EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "04 - Player Stats" )
	EOccupationType Occupation = EOccupationType::EOT_Unemployed;

public:

	FORCEINLINE void SetEquippedWeapon( AWeapon* NewWeapon ) { EquippedWeapon = NewWeapon; }

	FORCEINLINE AWeapon* GetEquippedWeapon() const { return EquippedWeapon; }
	FORCEINLINE bool IsTryingToJump() const { return bJumpButtonPressed; }
	FORCEINLINE UObject* GetCurrentInteractableObject() const { return CurrentInteractableObject.Get(); }
};
