// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/InteractInferface.h"
#include "Weapon.generated.h"

class USphereComponent;
class UParticleSystem;
class USoundCue;

UCLASS()
class AITOWN_API AWeapon : public AActor, public IInteractInferface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	/* AWeapon */

public:

	/* IInteractInferface */
	UFUNCTION()
	virtual void Interact( AActor* Interactor ) override;
	UFUNCTION()
	virtual void OnHoverEnter( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult ) override;
	UFUNCTION()
	virtual void OnHoverExit( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex ) override;

	void FireWeapon();

private:

	UPROPERTY( EditAnywhere )
	USphereComponent* SphereDetection;

	UPROPERTY( EditAnywhere )
	USkeletalMeshComponent* WeaponMesh;

	/* Combat */
	UPROPERTY( EditAnywhere, Category = "00 - Combat")
	float Damage = 50.f;
	UPROPERTY( EditAnywhere, Category = "00 - Combat" )
	float HeadshotDamage = 100.f;
	UPROPERTY( EditAnywhere, Category = "00 - Combat" )
	bool bAutomatic = false;
	UPROPERTY( EditAnywhere, Category = "00 - Combat" )
	float FireRate = 10.f;
	FTimerHandle FireRateHandle;

	/* Particles and sounds */
	UPROPERTY( EditAnywhere, Category = "01 - Effects" )
	UParticleSystem* MuzzleFlash;
	UPROPERTY( EditAnywhere, Category = "01 - Effects" )
	USoundCue* FireSound;
	UPROPERTY( EditAnywhere, Category = "01 - Effects" )
	UParticleSystem* ImpactParticles;
	UPROPERTY( EditAnywhere, Category = "01 - Effects" )
	USoundCue* ImpactSound;

	void FireTimerFinished();

	void EquipWeapon( AActor* ActorEquipping );
	void UnequipWeapon();

	bool GetBeamEndLocation( const FVector& MuzzleSocketLocation, FHitResult& OutHitResult );
	bool TraceUnderCrosshairs( FHitResult& OutHitResult, FVector& OutHitLocation );
};

/*
* TODO:
* - After Dropping weapon, set timer to destroy it if it hasn't been picked up. 1 min?
*/