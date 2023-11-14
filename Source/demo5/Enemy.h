// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enemy.generated.h"

UCLASS()
class DEMO5_API AEnemy : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemy();
	// Body
	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* Mesh;
	// Action
	UFUNCTION()
		void DestroyHandler(AActor* DestroyedActor);
	// Effects
	UPROPERTY(EditAnywhere)
		class USoundBase* ExplosionSound;
	UPROPERTY(VisibleAnywhere)
		float FloatSpeed = 500.0f;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
