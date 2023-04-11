#pragma once

UENUM( BlueprintType )
enum class EOccupationType : uint8
{
	EOT_Unemployed		UMETA( DisplayName = "Unemployed" ),
	EOT_PoliceOfficer	UMETA( DisplayName = "Police Officer" ),
	EOT_Receptionist	UMETA( DisplayName = "Receptionist" ),

	EOT_MAX				UMETA( DisplayName = "DefaultMAX" )
};

UENUM( BlueprintType )
enum class EDayType : uint8
{
	EDT_DayTime			UMETA( DisplayName = "Day Time" ),		// As soon as daytime comes AIs will go to work
	EDT_EveningTime		UMETA( DisplayName = "Evening Time" ),	// AIs will go home
	EDT_NightTime		UMETA( DisplayName = "Night Time" ),	// AIs will be sleeping, or walking.

	EDT_MAX				UMETA( DisplayName = "DefaultMAX" )
};
