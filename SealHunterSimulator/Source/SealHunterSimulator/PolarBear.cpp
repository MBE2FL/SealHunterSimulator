// Fill out your copyright notice in the Description page of Project Settings.


#include "PolarBear.h"

// Sets default values
APolarBear::APolarBear()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APolarBear::BeginPlay()
{
	Super::BeginPlay();

	OnTakeAnyDamage.AddDynamic(this, &APolarBear::onTakeAnyDamage);
}

// Called every frame
void APolarBear::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APolarBear::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void APolarBear::onTakeAnyDamage(
	AActor* DamagedActor, 
	float Damage, 
	const UDamageType* DamageType, 
	AController* InstigatedBy, 
	AActor* DamageCauser)
{
	UE_LOG(LogTemp, Warning, TEXT("Polar bear took %f damage!"), Damage);
	_health -= Damage;

	if (_health <= 0.0f)
	{
		UE_LOG(LogTemp, Warning, TEXT("Polar Bear Died!"));
		Destroy();
	}
}

