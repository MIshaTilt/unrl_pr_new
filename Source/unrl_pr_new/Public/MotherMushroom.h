#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MotherMushroom.generated.h"

// 1. Создаем Стейт-машину (Наши состояния)
UENUM(BlueprintType)
enum class EMushroomState : uint8
{
	NeedsSpores   UMETA(DisplayName = "Ожидает споры"),
	ReadyToBloom  UMETA(DisplayName = "Готов распуститься"),
	Symbiosis     UMETA(DisplayName = "Бабочка выпущена")
};

UCLASS()
class UNRL_PR_NEW_API AMotherMushroom : public AActor
{
	GENERATED_BODY()
	
public:	
	AMotherMushroom();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USceneComponent* RootComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UStaticMeshComponent* MeshComp;

	// Точка, где будет появляться бабочка
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USceneComponent* ButterflySpawnPoint;

	// --- ПАРАМЕТРЫ СТЕЙТ-МАШИНЫ ---

	// Текущее состояние
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State Machine")
	EMushroomState CurrentState;

	// Сколько спор нужно для активации
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State Machine")
	int32 RequiredSpores = 5;

	// Сколько спор уже положено
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State Machine")
	int32 CurrentSpores = 0;

	// Класс бабочки, которую будем спавнить
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "State Machine")
	TSubclassOf<APawn> ButterflyClass;

	// Событие для Blueprint, чтобы мы могли красиво менять цвет/эффекты при смене состояния
	UFUNCTION(BlueprintImplementableEvent, Category = "State Machine")
	void OnStateChanged(EMushroomState NewState);

public:
	// --- ФУНКЦИИ ДЛЯ СВЯЗИ С ИНТЕРФЕЙСАМИ ---

	// Для интерфейса BI_FuelReceiver -> GetRequiredResources
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	int32 GetMissingSpores() const;

	// Для интерфейса BI_FuelReceiver -> TryAddResource
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	bool AddSpores(int32 Amount);

	// Для интерфейса BI_Interactable -> Interact
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void SpawnAndPossessButterfly();
};