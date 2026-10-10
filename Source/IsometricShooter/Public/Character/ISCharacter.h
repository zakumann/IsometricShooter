// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ISCharacter.generated.h"

UENUM(BlueprintType)
enum class EFacingMode : uint8
{
	Movement		UMETA(DisplayName = "Movement"),
	PointOfInterest	UMETA(DisplayName = "Point of Interest")
};

class USpringArmComponent;
class UInputAction;
class UCameraComponent;
class UInputMappingContext;
struct FInputActionInstance;
struct FInputActionValue;

UCLASS()
class ISOMETRICSHOOTER_API AISCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AISCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<class USpringArmComponent> SpringArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<class UCameraComponent> TopDownCameraComp;

	UPROPERTY(EditAnywhere, Category = "Input")
	TSoftObjectPtr<UInputMappingContext> InputMapping;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> ShootAction;

	UPROPERTY()
	EFacingMode FacingMode;

	UPROPERTY()
	FVector PointOfInterest;

	void SetFocusPointOfInterest(bool focus);

	class AISPlayerController* PlayerController;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void Move(const FInputActionInstance& Instance);
	void StartShoot(const FInputActionInstance& Instance);
	void HandleShoot(const FInputActionInstance& Instance);
	void StopShoot(const FInputActionInstance& Instance);
	void LookAt(FVector location, bool bSmoothRotation, float DeltaTime, float RotationSpeed);
};
