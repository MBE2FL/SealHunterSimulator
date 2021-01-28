// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Seal.generated.h"

class UCharacterMovementComponent;
//class UCapsuleComponent;
class UBoxComponent;


#define ECC_PolarBear ECC_GameTraceChannel2


UCLASS(Blueprintable)
class SEALHUNTERSIMULATOR_API ASeal : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASeal();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void onComponentBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);


private:
	UPROPERTY()
	UCharacterMovementComponent* _movementComp;
	//UCapsuleComponent* _capsuleComp;
	UPROPERTY()
	UBoxComponent* _boxComp;

	// Dash
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Seal|Dash", meta = (AllowPrivateAccess = true, ClampMin = "1.0", ClampMax = "30.0"))
	float _dashCooldown = 4.0f;
	float _dashCooldownTimer = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Seal|Dash", meta = (AllowPrivateAccess = true, ClampMin = "0.0", ClampMax = "5.0"))
	float _dashEffectTime = 0.3f;
	float _dashEffectTimer = 0.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Seal|Dash", meta = (AllowPrivateAccess = true))
	bool _isDashing = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Seal|Dash", meta = (AllowPrivateAccess = true))
	float _dashForce = 500000.0f;
	float _origMaxAcc;
	float _origMaxWalkSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Seal|Dash", meta = (AllowPrivateAccess = true, ClampMin = "0.0"))
	float _dashDamage = 10.0f;


	// Land
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Seal", meta = (AllowPrivateAccess = true))
	bool _onLand = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Seal", meta = (AllowPrivateAccess = true, ClampMin = "0.0", ClampMax = "10.0"))
	float _onLandGravity = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Seal", meta = (AllowPrivateAccess = true, ClampMin = "0.0", ClampMax = "10.0"))
	float _offLandGravity = 0.3f;


	// Movement
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Seal|Movement", meta = (AllowPrivateAccess = true))
	float _baseTurnRate = 45.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Seal|Movement", meta = (AllowPrivateAccess = true))
	float _baseLookUpRate = 45.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Seal|Movement", meta = (AllowPrivateAccess = true))
	FVector _inRot = FVector(0.0f, 0.0f, 0.0f);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Seal|Movement", meta = (AllowPrivateAccess = true))
	FVector _inRot0 = FVector(0.0f, 0.0f, 0.0f);


	void dashAttack();

};
