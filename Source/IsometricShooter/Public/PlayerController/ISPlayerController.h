// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ISPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ISOMETRICSHOOTER_API AISPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	bool GetCursorGroundPosition(FVector &CursorGroundPosition, float GroundZ);

protected:
	virtual void BeginPlay() override;
};
