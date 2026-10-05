// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/ISCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "InputActionValue.h"
#include "InputAction.h"
#include "InputMappingContext.h"

// Sets default values
AISCharacter::AISCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(GetRootComponent());

	TopDownCameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCameraComp"));
	TopDownCameraComp->SetupAttachment(SpringArmComp);
}

// Called when the game starts or when spawned
void AISCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(GetWorld()->GetFirstLocalPlayerFromController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* InputSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (!InputMapping.IsNull())
			{
				InputSystem->AddMappingContext(InputMapping.LoadSynchronous(),0);
			}
		}
	}
}


void AISCharacter::Move(const FInputActionInstance& Instance)
{
	UE_LOG(LogTemp, Warning, TEXT("Move"));
	if (!TopDownCameraComp) return;

	const FVector2D MovementVector = Instance.GetValue().Get<FVector2D>();
	if (MovementVector.IsNearlyZero()) return;

	FVector Forward = TopDownCameraComp->GetForwardVector();
	Forward.Z = 0.0f;
	Forward.Normalize();
	FVector Right = TopDownCameraComp->GetRightVector();
	Right.Z = 0.0f;
	Right.Normalize();

	AddMovementInput(Forward, MovementVector.Y);
	AddMovementInput(Right, MovementVector.X);
}

void AISCharacter::StartShoot(const FInputActionInstance& Instance)
{
	UE_LOG(LogTemp, Warning, TEXT("Start Shoot"));
}

void AISCharacter::HandleShoot(const FInputActionInstance& Instance)
{
	UE_LOG(LogTemp, Warning, TEXT("Holding Shoot"));
}

void AISCharacter::StopShoot(const FInputActionInstance& Instance)
{
	UE_LOG(LogTemp, Warning, TEXT("Stop Shoot"));
}


// Called every frame
void AISCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AISCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AISCharacter::Move);
		Input->BindAction(ShootAction, ETriggerEvent::Started, this, &AISCharacter::StartShoot);
		Input->BindAction(ShootAction, ETriggerEvent::Ongoing, this, &AISCharacter::HandleShoot);
		Input->BindAction(ShootAction, ETriggerEvent::Completed, this, &AISCharacter::StopShoot);
	}
}