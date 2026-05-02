// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "PickUp/PickUpBase.h"
#include "MushroomSpawner.generated.h"

UCLASS()
class UNRL_PR_NEW_API AMushroomSpawner : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMushroomSpawner();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawner")
	UBoxComponent* SpawnArea;
	
	UPROPERTY(EditAnywhere, Category = "Spawner")
	TSubclassOf<APickUpBase> ActorToSpawn;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner")
	float MinSpawnDelay;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner")
	float MaxSpawnDelay;
	
	FTimerHandle SpawnTimerHandle;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintPure, Category = "Spawner")
	FVector GetRandomSpawnPoint();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
private:
	void SpawnActors();
	
	void StartSpawnTimer();
	
	float RandomSpawnDelay;	
};


