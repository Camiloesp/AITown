// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WheeledVehiclePawn.h"
#include "Engine/DataTable.h"
#include "InputActionValue.h"
#include "Car.generated.h"


/*
	TODO to add to tutorial:
	- Add Mesh to cars that have storage space.
	- Data driven design for helicopter, plane, car, boats... (Types: GroundVehicle, Flyer, Water, Amphibious)
	- Animations, sounds, FXs.
	- IK for wheels, landing gear.
	- Weaponised cars? Tanks, attack helicopters, military boats?
	- Smart Way to size springarm lenght and camera rotation and socket offset
*/

/*
	AI tutorial:			https://www.udemy.com/course/full-ai-system-in-unreal-engine5-and-cpp-beginners-to-advance/
	Chaos Vehicle Physics:	https://www.youtube.com/watch?v=T6vvnLRzjvY
*/

USTRUCT(BlueprintType)
struct FVehicleData : public FTableRowBase
{
	GENERATED_BODY()

public:
	/**
	*	How to create a vehicle:
	*	- All wheel bones should have the following naming convention: WheelPrefix followed by either Front or Rear initial and end with Left or Right initial. If the WheelPrefix is 'wheel' then it would look like this: 'wheelFL' for the front left wheel.
	*	- You need all of the assets like sounds, fxs.. Then you have to create a UCurveFloat for the TorqueCurve.
	*/
	UPROPERTY(VisibleDefaultsOnly)
	FString IMPORTANT = FString(TEXT("Hover over the 'IMPORTANT' to see instructions."));

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString WheelPrefix = FString("wheel");

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USkeletalMesh* VehicleMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UCurveFloat* TorqueCurve = nullptr;
};



class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UWheeledVehicleInputConfigData;
class UDataTable;
class UChaosVehicleWheel;
class UChaosWheeledVehicleMovementComponent;

UCLASS()
class AITOWN_API ACar : public AWheeledVehiclePawn
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
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArm;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* Camera;

	/* reference to the VehicleMovementComponent */
	UChaosWheeledVehicleMovementComponent* VehicleMovementComponentRef;

	/** MappingContext */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Enhanced Input")
	UInputMappingContext* DefaultMappingContext;

	/*
		Instead of having a long list of UInputAction*, I like to add those actions in a config file (UDataAsset subclass).
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Enhanced Input")
	UWheeledVehicleInputConfigData* InputActions;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), meta = (GetOptions = "GetVehicleTypes"))
	FName VehicleName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Car Information")
	UDataTable* VehicleTypesTable;

	FVehicleData VehicleInformation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Car Information")
	TSubclassOf<UChaosVehicleWheel> FrontWheel;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Car Information")
	TSubclassOf<UChaosVehicleWheel> RearWheel;

protected:

	/** Called for movement input */
	void Throttle(const FInputActionValue& Value);

	/** Called for Steer input */
	void Steer(const FInputActionValue& Value);

	/** Called for lookaround input */
	void LookAround(const FInputActionValue& Value);

	/** Called for lookaround input */
	void Break(const FInputActionValue& Value);

	void CompletedBreak(const FInputActionValue& Value);

	/** Called for lookaround input */
	void HandBreak(const FInputActionValue& Value);

	/** Called for lookaround input */
	void CompletedHandBreak(const FInputActionValue& Value);

	void SetupVehicleMovementComponent();

	UFUNCTION()
	TArray<FName> GetVehicleTypes() const;

};
