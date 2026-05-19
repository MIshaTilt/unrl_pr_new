#include "MotherMushroom.h"
#include "Kismet/GameplayStatics.h"

AMotherMushroom::AMotherMushroom()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	RootComponent = RootComp;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);

	ButterflySpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("ButterflySpawnPoint"));
	ButterflySpawnPoint->SetupAttachment(RootComponent);
	// Поднимем точку спавна чуть выше гриба
	ButterflySpawnPoint->SetRelativeLocation(FVector(0.0f, 0.0f, 150.0f));

	// По умолчанию гриб голодный
	CurrentState = EMushroomState::NeedsSpores;
}

void AMotherMushroom::BeginPlay()
{
	Super::BeginPlay();
	// Вызываем эвент, чтобы обновить визуал на старте
	OnStateChanged(CurrentState);
}

int32 AMotherMushroom::GetMissingSpores() const
{
	// Возвращаем, сколько еще не хватает
	return FMath::Max(0, RequiredSpores - CurrentSpores);
}

bool AMotherMushroom::AddSpores(int32 Amount)
{
	// СТЕЙТ-МАШИНА: Защита. Если мы уже готовы или бабочка выпущена - ресурсы не принимаем!
	if (CurrentState != EMushroomState::NeedsSpores)
	{
		return false; // Игрок не потратит свои ресурсы
	}

	CurrentSpores += Amount;

	// Проверяем, набрали ли мы нужное количество
	if (CurrentSpores >= RequiredSpores)
	{
		CurrentState = EMushroomState::ReadyToBloom;
		OnStateChanged(CurrentState); // Вызываем событие для смены визуала
	}

	return true; // Ресурсы успешно приняты
}

APawn* AMotherMushroom::SpawnButterfly()
{
	// Защита стейт-машины
	if (CurrentState != EMushroomState::ReadyToBloom)
	{
		return nullptr; // Если не готовы, возвращаем пустоту
	}

	APawn* SpawnedButterfly = nullptr;

	if (ButterflyClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		SpawnParams.Owner = this;

		// Спавним бабочку
		SpawnedButterfly = GetWorld()->SpawnActor<APawn>(
			ButterflyClass, 
			ButterflySpawnPoint->GetComponentLocation(), 
			ButterflySpawnPoint->GetComponentRotation(), 
			SpawnParams
		);

		// Если спавн прошел успешно
		if (SpawnedButterfly)
		{
			// Сбрасываем стейт гриба (как ты и хотел)
			CurrentState = EMushroomState::NeedsSpores;
			OnStateChanged(CurrentState);
		}
	}

	// Возвращаем заспавненную бабочку в Блюпринт!
	return SpawnedButterfly; 
}
