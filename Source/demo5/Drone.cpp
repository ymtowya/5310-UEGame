// Fill out your copyright notice in the Description page of Project Settings.


#include "Drone.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/InputComponent.h"
#include "PhysicsEngine/PhysicsThrusterComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ADrone::ADrone()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// Body Part Set
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	// Paddles
	Paddle1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Paddle1"));
	Paddle2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Paddle2"));
	Paddle3 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Paddle3"));
	Paddle4 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Paddle4"));
	Paddle1->SetupAttachment(Mesh, TEXT("Paddle1"));
	Paddle2->SetupAttachment(Mesh, TEXT("Paddle2"));
	Paddle3->SetupAttachment(Mesh, TEXT("Paddle3"));
	Paddle4->SetupAttachment(Mesh, TEXT("Paddle4"));
	Paddles.Add(Paddle1);
	Paddles.Add(Paddle2);
	Paddles.Add(Paddle3);
	Paddles.Add(Paddle4);
	// Out collision box
	OutCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("OutCollision"));
	// Config the box
	OutCollision->SetBoxExtent(FVector(100.0f, 100.0f, 50.0f));
	OutCollision->SetSimulatePhysics(true);
	OutCollision->BodyInstance.bLockXRotation = true;
	OutCollision->BodyInstance.bLockYRotation = true;
	OutCollision->SetCollisionProfileName(TEXT("Pawn"));
	OutCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	OutCollision->SetLinearDamping(2.0f);
	OutCollision->SetAngularDamping(0.5f);
	Mesh->SetupAttachment(OutCollision);
	// Set Root Component
	RootComponent = OutCollision;
	// Set Forces
	UpThruster = CreateDefaultSubobject<UPhysicsThrusterComponent>(TEXT("UpThruster"));
	UpThruster->SetupAttachment(RootComponent);
	UpThruster->ThrustStrength = LiftForceDefault;
	UpThruster->SetAutoActivate(true);
	UpThruster->SetWorldRotation(UKismetMathLibrary::MakeRotFromX(-this->GetActorUpVector()));

	ForwardThruster = CreateDefaultSubobject<UPhysicsThrusterComponent>(TEXT("ForwardThruster"));
	ForwardThruster->SetupAttachment(RootComponent);
	ForwardThruster->ThrustStrength = 0.0f;
	ForwardThruster->SetAutoActivate(true);
	ForwardThruster->SetWorldRotation(UKismetMathLibrary::MakeRotFromX(-this->GetActorForwardVector()));
}

// Called when the game starts or when spawned
void ADrone::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADrone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (GetInputAxisValue(TEXT("Lift")) == 0.0f)
	{
		UpThruster->ThrustStrength = LiftForceDefault;
	}
	if (GetInputAxisValue(TEXT("Forward")) == 0.0f)
	{
		// move
		ForwardThruster->ThrustStrength = 0.0f;
		// rotate
		if (Mesh->GetRelativeRotation().Pitch != 0.0f)
		{
			float currentPitch = Mesh->GetRelativeRotation().Pitch;
			float sign = FMath::Sign(currentPitch);
			Mesh->AddRelativeRotation(FRotator(-currentPitch * DeltaTime, 0.0f, 0.0f));
			if (FMath::Abs(Mesh->GetRelativeRotation().Pitch) <= KINDA_SMALL_NUMBER)
			{
				Mesh->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
			}
		}
	}
	RotatePaddle(DeltaTime);
}

// Called to bind functionality to input
void ADrone::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("Lift"), this, &ADrone::Lift);
	PlayerInputComponent->BindAxis(TEXT("Forward"), this, &ADrone::Forward);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ADrone::Turn);
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Released, this, &ADrone::DoFire);
}

void ADrone::Lift(float val)
{
	UpThruster->ThrustStrength += (int) (val * LiftAcc * GetWorld()->DeltaTimeSeconds);
	UpThruster->ThrustStrength = FMath::Clamp(UpThruster->ThrustStrength, -LiftThrustMax, LiftThrustMax);
}

void ADrone::Forward(float val)
{
	ForwardThruster->ThrustStrength += val * GetWorld()->DeltaTimeSeconds * ForwardAcc;
	ForwardThruster->ThrustStrength = FMath::Clamp(ForwardThruster->ThrustStrength, -ForwardThrustMax, ForwardThrustMax);

	float pitch = Mesh->GetRelativeRotation().Pitch;
	if (FMath::Abs(pitch) < 20.0f)
	{
		Mesh->AddRelativeRotation(FRotator(-val * 50.0f * GetWorld()->DeltaTimeSeconds, 0.0f, 0.0f));
	}
}

void ADrone::Turn(float val)
{
	OutCollision->AddTorqueInDegrees(-this->GetActorUpVector() * val * TurnStrength);
}

void ADrone::RotatePaddle(float Delta)
{
	for (auto paddle : Paddles)
	{
		paddle->AddRelativeRotation(FRotator(0.0f, Delta * PaddleRotateSpeed, .0f));
	}
}

void ADrone::DoFire()
{
	FTransform firingpoint =  Mesh->GetSocketTransform(TEXT("FiringSocket"));
	GetWorld()->SpawnActor<AMissle>(Bullet, firingpoint);
}
