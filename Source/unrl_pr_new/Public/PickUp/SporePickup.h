#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SporePickup.generated.h"

UCLASS()
class UNRL_PR_NEW_API ASporePickup : public AActor
{
	GENERATED_BODY()
	
public:	
	ASporePickup();

	// Обязательно добавляем Tick
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USceneComponent* RootComp;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spore Settings")
	float LifeTime = 10.0f;

	// --- ПАРАМЕТРЫ ПОЛЕТА (можно будет менять в Блюпринте для разных спор) ---
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Settings")
	float FlyRadius = 150.0f; // В каком радиусе от своей начальной точки спора может летать

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Settings")
	float FlySpeed = 1.5f; // Скорость интерполяции (чем меньше, тем более плавный и долгий полет)

private:
	FVector StartLocation;    // Центр, вокруг которого летает спора
	FVector TargetLocation;   // Текущая точка, куда она летит сейчас

	// Функция для генерации новой точки
	void PickNewTargetLocation();
};