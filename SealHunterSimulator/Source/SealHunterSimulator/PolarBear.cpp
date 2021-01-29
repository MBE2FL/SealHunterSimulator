// Fill out your copyright notice in the Description page of Project Settings.


#include "PolarBear.h"
#include "Kismet/KismetMathLibrary.h"

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

	_currSpeedMultiplier = _speedMultiplier;
}

// Called every frame
void APolarBear::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (_isSlow)
	{
		_slowEffectTimer += DeltaTime;

		if (_slowEffectTimer >= _slowEffectTime)
		{
			_currSpeedMultiplier = _speedMultiplier;
			_slowEffectTimer = 0.0f;
			_isSlow = false;
		}
	}
}

// Called to bind functionality to input
void APolarBear::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &APolarBear::moveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &APolarBear::moveRight);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &APolarBear::turn);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APolarBear::lookUp);
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


	_isSlow = true;
	_currSpeedMultiplier = _slowSpeedMultiplier;
}

void APolarBear::moveForward(float value)
{
	FRotator rotator = GetControlRotation();
	rotator.Roll = 0.0f;
	AddMovementInput(UKismetMathLibrary::GetForwardVector(rotator), value * _currSpeedMultiplier);

	//AddMovementInput(GetActorForwardVector(), value * _speedMultiplier);
	//_movementVector += GetActorForwardVector() * value;
}

void APolarBear::moveRight(float value)
{
	FRotator rotator = GetControlRotation();
	rotator.Roll = 0.0f;
	AddMovementInput(UKismetMathLibrary::GetRightVector(rotator), value * _currSpeedMultiplier);

	//AddMovementInput(GetActorRightVector(), value * _speedMultiplier);
	//_movementVector += GetActorRightVector() * value;
}

void APolarBear::turn(float value)
{
	AddControllerYawInput(value);
}

void APolarBear::lookUp(float value)
{
	AddControllerPitchInput(value);
}

