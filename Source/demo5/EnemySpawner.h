// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/EngineTypes.h"
#include "Enemy.h"
#include "GameFramework/Actor.h"
#include "EnemySpawner.generated.h"

UCLASS()
class DEMO5_API AEnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemySpawner();
	// Out box
	UPROPERTY(VisibleAnywhere)
		class UBoxComponent* SpawnArea;
	// generate enemy
	UFUNCTION()
		void SpawnEnemy();
	UPROPERTY(EditAnywhere)
		TSubclassOf<AEnemy> EnemyType;
	UPROPERTY(EditAnywhere)
		float SpawnSpan = 1.0f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	FTimerHandle SpawnHandle;
	virtual void Tick(float DeltaTime) override;

};
