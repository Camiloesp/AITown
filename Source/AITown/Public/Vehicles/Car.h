// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "GameFramework/Pawn.h"
#include "WheeledVehiclePawn.h"
#include "InputActionValue.h"
#include "Engine/DataTable.h"
#include "Car.generated.h"


/*
	Personal TODO:
	- Add Mesh to cars that have storage space.
	- Data driven design for helicopter, plane, car, boats... (Types: GroundVehicle, Flyer, Water, Amphibious)
	- Animations, sounds, FXs.
	- IK for wheels, landing gear.
	- Weaponised cars? Tanks, attack helicopters, military boats?
	- Smart Way to size springarm lenght and camera rotation and socket offset
*/


USTRUCT(BlueprintType)
struct FVehicleData : public FTableRowBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USkeletalMesh* VehicleMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UCurveFloat* TorqueCurve = nullptr;
};



class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UCharacterInputConfigData;
class UDataTable;

UCLASS()
class AITOWN_API ACar : public AWheeledVehiclePawn//APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACar();

protected:

	void OnConstruction(const FTransform& Transform);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:

	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess="true"))
	//USkeletalMeshComponent* VehicleMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArm;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* Camera;


	/** MappingContext */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Enhanced Input")
	UInputMappingContext* DefaultMappingContext;

	/*
		Instead of having a long list of UInputAction*, I like to add those actions in a config file (UDataAsset subclass).
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Enhanced Input")
	UCharacterInputConfigData* InputActions;


	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), meta = (GetOptions = "GetVehicleTypes"))
	FName VehicleName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Car Information")
	UDataTable* VehicleTypesTable;

	FVehicleData VehicleInformation;

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	void SetupVehicleMovementComponent();

	UFUNCTION()
	TArray<FName> GetVehicleTypes() const;

};
