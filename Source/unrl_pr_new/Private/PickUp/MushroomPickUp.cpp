// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUp/MushroomPickUp.h"

// Sets default values
AMushroomPickUp::AMushroomPickUp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMushroomPickUp::BeginPlay()
{
	Super::BeginPlay();
	
	PickUpMeshComponent->SetSimulatePhysics(true);
}

// Called every frame
void AMushroomPickUp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

