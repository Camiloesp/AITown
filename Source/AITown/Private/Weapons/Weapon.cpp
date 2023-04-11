// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Weapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

#include "Interfaces/InteractInferface.h"
//#include "Interfaces/HealthInterface.h"
#include "Weapons/Weapon.h"
#include "Characters/ShooterCharacter.h"

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>( TEXT("Weapon Mesh"));
	WeaponMesh->SetupAttachment( GetRootComponent() );

	SphereDetection = CreateDefaultSubobject<USphereComponent>( TEXT( "Sphere Detection" ) );
	SphereDetection->SetupAttachment( WeaponMesh );

	InterfaceOwner = WeaponMesh;
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	SphereDetection->OnComponentBeginOverlap.AddDynamic( this, &AWeapon::OnHoverEnter );
	SphereDetection->OnComponentEndOverlap.AddDynamic( this, &AWeapon::OnHoverExit );
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeapon::Interact( AActor* Interactor )
{
	EquipWeapon( Interactor );
}

void AWeapon::OnHoverEnter( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult )
{
	IInteractInferface::OnHoverEnter( OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult );
	// Show pickup widget?


	AShooterCharacter* Player = Cast<AShooterCharacter>(OtherActor);
	if (Player)
	{
		Player->SetCurrentInteractableObject( this );
	}
}

void AWeapon::OnHoverExit( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex )
{
	IInteractInferface::OnHoverExit( OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex );
	// Hide pickup widget?

	AShooterCharacter* Player = Cast<AShooterCharacter>( OtherActor );
	if (Player)
	{
		Player->ClearCurrentInteractableObject();
	}
}

void AWeapon::EquipWeapon( AActor* ActorEquipping )
{
	AShooterCharacter* Player = Cast<AShooterCharacter>( ActorEquipping );
	if (!Player) return;

	// If a weapon is already equipped drop it.
	if (Player->GetEquippedWeapon()) Player->GetEquippedWeapon()->UnequipWeapon();

	Player->SetEquippedWeapon( this );
	SetOwner( Player );

	// Disable collision
	WeaponMesh->SetSimulatePhysics( false );
	WeaponMesh->SetCollisionEnabled( ECollisionEnabled::NoCollision );
	SphereDetection->SetCollisionEnabled( ECollisionEnabled::NoCollision );

	// Attach to socket.
	const USkeletalMeshComponent* PlayerMesh = Cast<USkeletalMeshComponent>( ActorEquipping->GetComponentByClass( USkeletalMeshComponent::StaticClass() ) );
	if (PlayerMesh)
	{
		const USkeletalMeshSocket* HandSocket = Player->GetMesh()->GetSocketByName( FName( "WeaponHandSocket" ) );

		if (HandSocket)
		{
			HandSocket->AttachActor( this, Player->GetMesh() );
		}
	}

	Player->SetCurrentInteractableObject( nullptr );
}

void AWeapon::UnequipWeapon()
{
	FDetachmentTransformRules DetachmentTransformRules = FDetachmentTransformRules( EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, false );
	DetachFromActor( DetachmentTransformRules );

	SetOwner( nullptr );

	// Disable collision
	WeaponMesh->SetCollisionEnabled( ECollisionEnabled::QueryAndPhysics );
	SphereDetection->SetCollisionEnabled( ECollisionEnabled::QueryAndPhysics );

	WeaponMesh->SetSimulatePhysics( true );
	WeaponMesh->SetCollisionResponseToChannel( ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap );
}

void AWeapon::FireTimerFinished()
{
	AShooterCharacter* Player = Cast<AShooterCharacter>( GetOwner() );
	if (!Player) return;

	if (bAutomatic && Player->bFireButtonPressed)
	{
		FireWeapon();
	}
}

void AWeapon::FireWeapon()
{
	// Line trace from barrel socket to the end of our camera trace. Two line traces needed to avoid corners.
	const USkeletalMeshSocket* BarrelSocket = WeaponMesh->GetSocketByName( TEXT( "Muzzle" ) );
	if (!BarrelSocket) return;

	const FTransform SocketTransform = BarrelSocket->GetSocketTransform( WeaponMesh );
	FHitResult BeamHitResult;
	bool bBeamEnd = GetBeamEndLocation( SocketTransform.GetLocation(), BeamHitResult );
	
	if (bBeamEnd)
	{
		if (BeamHitResult.GetActor())
		{
			AShooterCharacter* WeaponOwner = Cast<AShooterCharacter>( GetOwner() );
			AShooterCharacter* HitEnemy = Cast<AShooterCharacter>( BeamHitResult.GetActor());
			//UE_LOG( LogTemp, Error, TEXT( "Target hit: %s" ), *BeamHitResult.GetActor()->GetFName().ToString() );

			if (WeaponOwner && HitEnemy)
			{
				bool bHeadshot = BeamHitResult.BoneName.ToString() == FString( "head" );
				int32 FinalDamage = bHeadshot ? HeadshotDamage : Damage;

				UGameplayStatics::ApplyDamage( HitEnemy, FinalDamage, WeaponOwner->Controller, WeaponOwner, UDamageType::StaticClass() );
				//HitEnemy->ShowHitNumber( FinalDamage, BeamHitResult.Location, true );
			}
		}
	}

	// Fire effects
	if (FireSound)
	{
		UGameplayStatics::PlaySoundAtLocation( this, FireSound, GetActorLocation() );
	}
	if (MuzzleFlash)
	{
		UGameplayStatics::SpawnEmitterAtLocation( GetWorld(), MuzzleFlash, SocketTransform );
	}

	// Impact Effects
	if (ImpactSound)
	{
		UGameplayStatics::PlaySoundAtLocation( this, ImpactSound, BeamHitResult.Location );
	}
	if (ImpactParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation( GetWorld(), ImpactParticles, BeamHitResult.Location );
	}

	GetWorldTimerManager().SetTimer( FireRateHandle, this, &AWeapon::FireTimerFinished, FireRate );
}

bool AWeapon::GetBeamEndLocation( const FVector& MuzzleSocketLocation, FHitResult& OutHitResult )
{
	FVector OutBeamLocation;

	//Check for crosshair trace hit
	FHitResult CrosshairHitResult;
	bool bCrosshairHit = TraceUnderCrosshairs( CrosshairHitResult, OutBeamLocation );

	if (bCrosshairHit)
	{
		// Tentative beam location - still need to trace from gun
		OutBeamLocation = CrosshairHitResult.Location;
	}

	// perform a second trace, this time from the gun barrel
	const FVector WeaponTraceStart{ MuzzleSocketLocation };
	const FVector StartToEnd{ OutBeamLocation - MuzzleSocketLocation };
	const FVector WeaponTraceEnd{ MuzzleSocketLocation + StartToEnd * 1.25f };
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor( GetOwner() );
	GetWorld()->LineTraceSingleByChannel( OutHitResult, WeaponTraceStart, WeaponTraceEnd, ECollisionChannel::ECC_Visibility, QueryParams );


	if (!OutHitResult.bBlockingHit) //object between barrel and BeamEndPoint?
	{
		OutHitResult.Location = OutBeamLocation;
		return false;
	}
	return true;
}

bool AWeapon::TraceUnderCrosshairs( FHitResult& OutHitResult, FVector& OutHitLocation )
{
	// Get Viewport Size
	FVector2D ViewportSize;
	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize( ViewportSize );
	}

	// Get screen space location of crosshairs
	FVector2D CrosshairLocation( ViewportSize.X / 2.f, ViewportSize.Y / 2.f );
	//CrosshairLocation.Y -= 50.f;
	FVector CrosshairWorldPosition;
	FVector CrosshairWorldDirection;

	// Get world position and direction of crosshair
	bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld( UGameplayStatics::GetPlayerController( this, 0 ), CrosshairLocation, CrosshairWorldPosition, CrosshairWorldDirection );

	if (bScreenToWorld)
	{
		// trace from crosshair world location outward
		const FVector Start{ CrosshairWorldPosition };
		const FVector End{ Start + CrosshairWorldDirection * 50'000.f };
		OutHitLocation = End;
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor( GetOwner() );
		GetWorld()->LineTraceSingleByChannel( OutHitResult, Start, End, ECollisionChannel::ECC_Visibility, QueryParams );

		if (OutHitResult.bBlockingHit)
		{
			OutHitLocation = OutHitResult.Location;
			return true;
		}
	}

	return false;
}

