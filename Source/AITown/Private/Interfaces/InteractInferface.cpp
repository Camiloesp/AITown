// Fill out your copyright notice in the Description page of Project Settings.


#include "Interfaces/InteractInferface.h"
#include "Materials/MaterialInstanceDynamic.h"

// Add default functionality here for any IInteractInferface functions that are not pure virtual.

void IInteractInferface::OnHoverEnter( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult )
{
	ensure( InterfaceOwner );

	// Set the emissive color of the static mesh to indicate that the object is interactable
	USkeletalMeshComponent* StaticMesh = Cast<USkeletalMeshComponent>( InterfaceOwner->GetOwner()->GetComponentByClass( USkeletalMeshComponent::StaticClass() ) );
	if (StaticMesh)
	{
		StaticMesh->SetScalarParameterValueOnMaterials( "EmissiveStrength", 10.0f );
	}
}

void IInteractInferface::OnHoverExit( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex )
{
	ensure( InterfaceOwner );

	// Reset the emissive color of the static mesh to indicate that the object is no longer interactable
	USkeletalMeshComponent* StaticMesh = Cast<USkeletalMeshComponent>( InterfaceOwner->GetOwner()->GetComponentByClass( USkeletalMeshComponent::StaticClass() ) );
	if (StaticMesh)
	{
		StaticMesh->SetScalarParameterValueOnMaterials( "EmissiveStrength", 0.0f );
	}
}
