// Fill out your copyright notice in the Description page of Project Settings.


#include "Seal.h"
#include "GameFramework/CharacterMovementComponent.h"
//#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASeal::ASeal()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASeal::BeginPlay()
{
	Super::BeginPlay();
	
	_movementComp = GetCharacterMovement();
	//_capsuleComp = GetCapsuleComponent();
	//_capsuleComp->OnComponentBeginOverlap.AddDynamic(this, &ASeal::onComponentBeginOverlap);

	_boxComp = Cast<UBoxComponent>(GetComponentByClass(UBoxComponent::StaticClass()));
	_boxComp->OnComponentBeginOverlap.AddDynamic(this, &ASeal::onComponentBeginOverlap);
}

// Called every frame
void ASeal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	_dashCooldownTimer += DeltaTime;


	GetCharacterMovement()->GravityScale = _onLand ? _onLandGravity : _offLandGravity;


	// Dash is in effect.
	if (_isDashing)
	{
		_dashEffectTimer += DeltaTime;
		UE_LOG(LogTemp, Warning, TEXT("Timer: %f"), _dashEffectTimer);

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
void ASeal::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("Dash"), EInputEvent::IE_Pressed, this, &ASeal::dashAttack);
}

void ASeal::onComponentBeginOverlap( 
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
		UE_LOG(LogTemp, Warning, TEXT("Overlapped with: "), *OtherActor->GetClass()->GetName());

		if (OverlappedComponent->GetCollisionObjectType() == ECC_PolarBear)
		{
			UGameplayStatics::ApplyDamage(OtherActor, _dashDamage, Controller, this, nullptr);
		}
	}
}

void ASeal::dashAttack()
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

