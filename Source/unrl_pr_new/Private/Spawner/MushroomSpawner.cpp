// Fill out your copyright notice in the Description page of Project Settings.


#include "Spawner/MushroomSpawner.h"

#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "WorldPartition/HLOD/HLODActor.h"


// Sets default values
AMushroomSpawner::AMushroomSpawner()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	SpawnArea = CreateDefaultSubobject<UBoxComponent>(TEXT("Spawn Area"));
	
	MinSpawnDelay = 0.5f;
	MaxSpawnDelay = 2.5f;
}

// Called when the game starts or when spawned
void AMushroomSpawner::BeginPlay()
{
	Super::BeginPlay();
	
	StartSpawnTimer();
}

// Called every frame
void AMushroomSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

FVector AMushroomSpawner::GetRandomSpawnPoint()
{
	const FVector SpawnOrigin = SpawnArea->Bounds.Origin;
	const FVector SpawnLimits = SpawnArea->Bounds.BoxExtent;
	
	return UKismetMathLibrary::RandomPointInBoundingBox(SpawnOrigin, SpawnLimits);
}

void AMushroomSpawner::SpawnActors()
{
	if (!ActorToSpawn || !GetWorld())
	{
		return;
	}
	
	FActorSpawnParameters Parameters;
	Parameters.Owner = this;
	Parameters.Instigator = GetInstigator();
	Parameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	
	APickUpBase* SpawnedActor = GetWorld()->SpawnActor<APickUpBase>(ActorToSpawn, GetRandomSpawnPoint(),
		UKismetMathLibrary::RandomRotator(), Parameters);
	
	StartSpawnTimer();
}

void AMushroomSpawner::StartSpawnTimer()
{
	RandomSpawnDelay = FMath::RandRange(MinSpawnDelay, MaxSpawnDelay);
	
	GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, 
		&AMushroomSpawner::SpawnActors,RandomSpawnDelay, false);
}
