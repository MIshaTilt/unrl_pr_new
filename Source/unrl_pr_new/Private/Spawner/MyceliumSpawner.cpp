#include "Spawner/MyceliumSpawner.h"
#include "Components/SphereComponent.h"
#include "PickUp/SporePickup.h"
#include "Kismet/KismetMathLibrary.h" // Для случайных точек
#include "GameFramework/Pawn.h"       // Для проверки, что это игрок

AMyceliumSpawner::AMyceliumSpawner()
{
	PrimaryActorTick.bCanEverTick = false;

	TriggerZone = CreateDefaultSubobject<USphereComponent>(TEXT("TriggerZone"));
	RootComponent = TriggerZone;
	
	// Настраиваем радиус по умолчанию
	TriggerZone->InitSphereRadius(400.0f);
	
	// Настраиваем коллизию (реагируем только на Overlap)
	TriggerZone->SetCollisionProfileName(TEXT("Trigger"));
}

void AMyceliumSpawner::BeginPlay()
{
	Super::BeginPlay();

	// Привязываем функцию к событию Overlap
	TriggerZone->OnComponentBeginOverlap.AddDynamic(this, &AMyceliumSpawner::OnPlayerEnter);
}

void AMyceliumSpawner::OnPlayerEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Проверяем, что в зону вошел именно игрок (Pawn), а не какой-то камень
	if (OtherActor && OtherActor->IsA(APawn::StaticClass()))
	{
		// Запоминаем игрока, чтобы спавнить споры вокруг него
		TargetPlayer = OtherActor;

		// Отключаем коллизию, чтобы спавнер не сработал второй раз
		TriggerZone->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		// Вычисляем, сколько спор мы заспавним (от 5 до 10)
		TotalSporesToSpawn = FMath::RandRange(MinSpores, MaxSpores);
		SporesSpawned = 0;

		// Вычисляем интервал между спавном каждой споры
		// Если 10 секунд и 5 спор, то интервал = 2 секунды
		float SpawnInterval = SpawnDuration / TotalSporesToSpawn;

		// Запускаем таймер, который будет циклично вызывать SpawnSingleSpore
		GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &AMyceliumSpawner::SpawnSingleSpore, SpawnInterval, true);
	}
}

void AMyceliumSpawner::SpawnSingleSpore()
{
	// Проверяем, есть ли классы для спавна в массиве и не исчез ли игрок
	if (SporeClasses.Num() == 0 || !TargetPlayer)
	{
		Destroy(); // Если настройки пустые - просто удаляем спавнер
		return;
	}

	// 1. ВАРИАТИВНОСТЬ: Выбираем случайный класс споры из массива
	int32 RandomClassIndex = FMath::RandRange(0, SporeClasses.Num() - 1);
	TSubclassOf<ASporePickup> SporeToSpawn = SporeClasses[RandomClassIndex];

	// 2. ОБЛАСТЬ: Вычисляем случайную точку вокруг игрока
	FVector PlayerLocation = TargetPlayer->GetActorLocation();
	FVector RandomOffset = UKismetMathLibrary::RandomUnitVector() * FMath::RandRange(50.0f, SpawnRadius);
	
	// Чтобы споры не спавнились под землей, берем Z игрока + небольшую высоту
	FVector SpawnLocation = PlayerLocation + RandomOffset;
	SpawnLocation.Z = PlayerLocation.Z + FMath::RandRange(20.0f, 100.0f);

	FRotator SpawnRotation = FRotator::ZeroRotator;

	// 3. Создаем структуру параметров (как в твоей методичке)
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	// СПАВНИМ!
	GetWorld()->SpawnActor<ASporePickup>(SporeToSpawn, SpawnLocation, SpawnRotation, SpawnParams);

	// Увеличиваем счетчик
	SporesSpawned++;

	// Если заспавнили все споры
	if (SporesSpawned >= TotalSporesToSpawn)
	{
		// Останавливаем таймер
		GetWorld()->GetTimerManager().ClearTimer(SpawnTimerHandle);
		
		// Грибница исчерпала себя - уничтожаем спавнер
		Destroy();
	}
}