// Fill out your copyright notice in the Description page of Project Settings.


#include "PolarBear.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/BoxComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
APolarBear::APolarBear()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
}

// Called when the game starts or when spawned
void APolarBear::BeginPlay()
{
	Super::BeginPlay();

	_movementComp = GetCharacterMovement();

	OnTakeAnyDamage.AddDynamic(this, &APolarBear::onTakeAnyDamage);

	_currSpeedMultiplier = _speedMultiplier;

	_boxComp = Cast<UBoxComponent>(GetComponentByClass(UBoxComponent::StaticClass()));
	_boxComp->OnComponentBeginOverlap.AddDynamic(this, &APolarBear::onComponentBeginOverlap);
}

// Called every frame
void APolarBear::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Slow down in effect
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



	_dashCooldownTimer += DeltaTime;


	// Dash is in effect.
	if (_isDashing)
	{
		_dashEffectTimer += DeltaTime;
		//UE_LOG(LogTemp, Warning, TEXT("Timer: %f"), _dashEffectTimer);

		// Keep dashing forward.
		if (_dashEffectTimer <= _dashEffectTime)
		{
			_movementComp->AddInputVector(GetActorForwardVector() * _dashForce);
		}
		// Dash has ended.
		else
		{
			_dashEffectTimer = 0.0f;
			_isDashing = false;
			_dashCooldownTimer = 0.0f;

			_movementComp->MaxAcceleration = _origMaxAcc;
			_movementComp->MaxWalkSpeed = _origMaxWalkSpeed;
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
	PlayerInputComponent->BindAction(TEXT("Dash"), EInputEvent::IE_Pressed, this, &APolarBear::dashAttack);
}

void APolarBear::onComponentBeginOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	// Deal damage to the polar while dashing into them.
	if (_isDashing)
	{
		UE_LOG(LogTemp, Warning, TEXT("Overlapped with: %s"), *OtherActor->GetName());

		if (OtherComp->GetCollisionObjectType() == ECC_Seal)
		{
			UE_LOG(LogTemp, Warning, TEXT("Polar Bear attacked!"));

			onDestroyActor(OtherActor);
		}
	}
}

void APolarBear::activateSlowEffect()
{
	_isSlow = true;
	_currSpeedMultiplier = _slowSpeedMultiplier;
}

void APolarBear::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APolarBear, _isSlow);
	DOREPLIFETIME(APolarBear, _currSpeedMultiplier);
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

void APolarBear::dashAttack()
{
	// Activate dash if it is off cooldown.
	if (_dashCooldownTimer >= _dashCooldown)
	{
		UE_LOG(LogTemp, Warning, TEXT("Activated Dash!"));
		_dashCooldownTimer = 0.0f;

		_origMaxAcc = _movementComp->MaxAcceleration;
		_origMaxWalkSpeed = _movementComp->MaxWalkSpeed;
		_movementComp->MaxAcceleration = 1000000.0f;
		_movementComp->MaxWalkSpeed = 10000.0f;

		_isDashing = true;
	}
}

