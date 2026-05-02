// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickUpBase.h"
#include "GameFramework/Actor.h"
#include "MushroomPickUp.generated.h"

UCLASS()
class UNRL_PR_NEW_API AMushroomPickUp : public APickUpBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMushroomPickUp();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
