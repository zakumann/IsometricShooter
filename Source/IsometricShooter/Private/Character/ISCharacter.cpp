// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/ISCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "InputActionValue.h"
#include "PlayerController/ISPlayerController.h"
#include "InputMappingContext.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AISCharacter::AISCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationYaw = false;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(GetRootComponent());
	SpringArmComp->TargetArmLength = 1200.0f;
	SpringArmComp->SetWorldRotation(FRotator(-60.0f, 0.0f, 0.0f));
	SpringArmComp->bInheritPitch = false;
	SpringArmComp->bInheritYaw = false;
	SpringArmComp->bInheritRoll = false;
	SpringArmComp->bDoCollisionTest = false;

	TopDownCameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCameraComp"));
	TopDownCameraComp->SetupAttachment(SpringArmComp);

	GetCharacterMovement()->bOrientRotationToMovement = true;
}

// Called when the game starts or when spawned
void AISCharacter::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = Cast<AISPlayerController>(GetController());

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
void AISCharacter::SetFocusPointOfInterest(bool focus)
{
	if (focus)
	{
		FacingMode = EFacingMode::PointOfInterest;
		GetCharacterMovement()->bOrientRotationToMovement = false;
	}
	else
	{
		FacingMode = EFacingMode::Movement;
		GetCharacterMovement()->bOrientRotationToMovement = true;
	}
}

void AISCharacter::Move(const FInputActionInstance& Instance)
{
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
	SetFocusPointOfInterest(true);
}

void AISCharacter::HandleShoot(const FInputActionInstance& Instance)
{
	UE_LOG(LogTemp, Warning, TEXT("Holding Shoot"));

	if (!PlayerController) return;

	FVector Location = GetActorLocation();

	FVector CursorGroundPosition;
	if (PlayerController->GetCursorGroundPosition(CursorGroundPosition, Location.Z))
	{
		PointOfInterest = CursorGroundPosition;
		FVector Forward = GetActorForwardVector();
		DrawDebugLine(GetWorld(), Location, Location + Forward * 1000.0f, FColor::Purple, false, -1.0f, 0, 5.0f);
	}
}

void AISCharacter::StopShoot(const FInputActionInstance& Instance)
{
	UE_LOG(LogTemp, Warning, TEXT("Stop Shoot"));
	SetFocusPointOfInterest(false);
}

void AISCharacter::LookAt(FVector location, bool bSmoothRotation, float DeltaTime, float RotationSpeed)
{
	FVector Direction = location - GetActorLocation();
	Direction.Z = 0.0f;

	FRotator TargetRotation = Direction.Rotation();

	if (!bSmoothRotation)
	{
		SetActorRotation(Direction.Rotation());
		return;
	}

	FRotator PartialRotation = FMath::RInterpTo(GetActorRotation(), TargetRotation, DeltaTime, RotationSpeed);
	SetActorRotation(PartialRotation);
}


// Called every frame
void AISCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (FacingMode == EFacingMode::PointOfInterest)
	{
		LookAt(PointOfInterest, true, DeltaTime, 10.0f);
	}
}

// Called to bind functionality to input
void AISCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AISCharacter::Move);
		Input->BindAction(ShootAction, ETriggerEvent::Started, this, &AISCharacter::StartShoot);
		Input->BindAction(ShootAction, ETriggerEvent::Triggered, this, &AISCharacter::HandleShoot);
		Input->BindAction(ShootAction, ETriggerEvent::Completed, this, &AISCharacter::StopShoot);
	}
}