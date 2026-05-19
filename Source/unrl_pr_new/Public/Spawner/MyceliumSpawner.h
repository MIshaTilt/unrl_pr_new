#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyceliumSpawner.generated.h"

UCLASS()
class UNRL_PR_NEW_API AMyceliumSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	AMyceliumSpawner();

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, Category = "Spawner Settings")
	float CooldownDuration = 60.0f; 

	// Зона, в которую должен войти игрок для активации
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USphereComponent* TriggerZone;
	
	UPROPERTY(EditAnywhere, Category = "Spawner Settings")
	int32 MaxPlayerSpores = 20;

	// Эта функция вызывается в C++, но её логика прописывается в Blueprint!
	UFUNCTION(BlueprintNativeEvent, Category = "Spawner Settings")
	int32 GetPlayerSporeCount(AActor* PlayerActor);

	// --- ПАРАМЕТРЫ ГЕНЕРАЦИИ И ВАРИАТИВНОСТЬ ---
	
	// Массив для вариативности (сюда мы положим разные блюпринты спор: красные, синие и т.д.)
	UPROPERTY(EditDefaultsOnly, Category = "Spawner Settings")
	TArray<TSubclassOf<class ASporePickup>> SporeClasses;

	UPROPERTY(EditAnywhere, Category = "Spawner Settings")
	int32 MinSpores = 5;

	UPROPERTY(EditAnywhere, Category = "Spawner Settings")
	int32 MaxSpores = 10;

	UPROPERTY(EditAnywhere, Category = "Spawner Settings")
	float SpawnDuration = 10.0f; // За сколько секунд должны появиться все споры

	UPROPERTY(EditAnywhere, Category = "Spawner Settings")
	float SpawnRadius = 300.0f; // Радиус вокруг игрока, где появятся споры

private:
	
	FTimerHandle CooldownTimerHandle;

	// Функция, которая "будит" спавнер
	void WakeUp();

	
	// Функция, которая сработает при входе в зону
	UFUNCTION()
	void OnPlayerEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// Функция для спавна одной споры (будет вызываться таймером)
	void SpawnSingleSpore();

	// Внутренние переменные для таймера
	FTimerHandle SpawnTimerHandle;
	int32 TotalSporesToSpawn;
	int32 SporesSpawned;

	// Ссылка на игрока, чтобы спавнить вокруг него
	AActor* TargetPlayer; 
};