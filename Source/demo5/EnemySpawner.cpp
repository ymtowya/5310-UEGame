// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawner.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AEnemySpawner::AEnemySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// Out spawn box
	SpawnArea = CreateDefaultSubobject<UBoxComponent>(TEXT("OutCollision"));
	// Config the box
	SpawnArea->SetBoxExtent(FVector(5500.0f, 5500.0f, 10.0f));
}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorld()->GetTimerManager().SetTimer(SpawnHandle, this, &AEnemySpawner::SpawnEnemy, SpawnSpan, true);
}

// Called every frame
void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemySpawner::SpawnEnemy()
{
	FVector Origin;
	FVector Extends;
	this->GetActorBounds(false, Origin, Extends);
	FVector SpawnPoint = UKismetMathLibrary::RandomPointInBoundingBox(Origin, Extends);
	GetWorld()->SpawnActor<AEnemy>(EnemyType, SpawnPoint, FRotator(0.0f,0.0f,0.0f));
}