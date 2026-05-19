#include "PickUp/SporePickup.h"
#include "Kismet/KismetMathLibrary.h" // Нужно для случайного вектора

ASporePickup::ASporePickup()
{
	// ВАЖНО: Включаем Tick, чтобы спора могла двигаться каждый кадр!
	PrimaryActorTick.bCanEverTick = true; 

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	RootComponent = RootComp;
}

void ASporePickup::BeginPlay()
{
	Super::BeginPlay();

	// Запоминаем точку, в которой спора появилась (её заспавнил наш Ведьмин Круг)
	StartLocation = GetActorLocation();

	// Сразу выбираем первую случайную точку для полета
	PickNewTargetLocation();
	
	SetLifeSpan(LifeTime);
}

void ASporePickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 1. Получаем текущую позицию
	FVector CurrentLocation = GetActorLocation();

	// 2. Вычисляем новую позицию с помощью плавной интерполяции (VInterpTo)
	FVector NewLocation = FMath::VInterpTo(CurrentLocation, TargetLocation, DeltaTime, FlySpeed);

	// 3. Двигаем спору
	SetActorLocation(NewLocation);

	// 4. Проверяем, долетела ли спора до цели (с погрешностью в 15 юнитов)
	if (FVector::Dist(CurrentLocation, TargetLocation) < 15.0f)
	{
		// Если долетела — выбираем новую точку!
		PickNewTargetLocation();
	}
}

void ASporePickup::PickNewTargetLocation()
{
	// Генерируем случайное направление (RandomUnitVector) и умножаем на случайную длину от 0 до FlyRadius
	FVector RandomOffset = UKismetMathLibrary::RandomUnitVector() * FMath::RandRange(0.0f, FlyRadius);
	
	// Новая цель — это начальная позиция + это случайное смещение
	TargetLocation = StartLocation + RandomOffset;
}