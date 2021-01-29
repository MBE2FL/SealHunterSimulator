// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PolarBear.generated.h"

UCLASS()
class SEALHUNTERSIMULATOR_API APolarBear : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APolarBear();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Polar Bear", meta = (AllowPrivateAccess = true))
	float _health = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Polar Bear", meta = (AllowPrivateAccess = true, ClampMin = "0.0"))
	float _speedMultiplier = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Polar Bear", meta = (AllowPrivateAccess = true, ClampMin = "0.0"))
	float _slowSpeedMultiplier = 1.0f;
	float _currSpeedMultiplier = 1.0f;
	bool _isSlow = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Polar Bear", meta = (AllowPrivateAccess = true, ClampMin = "0.0"))
	float _slowEffectTime = 1.0f;
	float _slowEffectTimer = 0.0f;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Polar Bear", meta = (AllowPrivateAccess = true))
	//FVector _movementVector = FVector::ZeroVector;


	UFUNCTION()
	void onTakeAnyDamage(
		AActor* DamagedActor,
		float Damage,
		const class UDamageType* DamageType,
		class AController* InstigatedBy,
		AActor* DamageCauser);

	UFUNCTION()
	void moveForward(float value);

	UFUNCTION()
	void moveRight(float value);

	UFUNCTION()
	void turn(float value);

	UFUNCTION()
	void lookUp(float value);
};
