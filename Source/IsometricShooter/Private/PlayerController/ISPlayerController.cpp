// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerController/ISPlayerController.h"

void AISPlayerController::BeginPlay()
{
	bShowMouseCursor = true;
}

bool AISPlayerController::GetCursorGroundPosition(FVector& CursorGroundPosition, float GroundZ)
{
	FVector WorldPosition;
	FVector WorldDirection;

	if (!DeprojectMousePositionToWorld(WorldPosition, WorldDirection))
	{
		return false;
	}

	if (FMath::IsNearlyZero(WorldDirection.Z))
	{
		return false;
	}

	float DistanceToGround = (GroundZ - WorldPosition.Z) / WorldDirection.Z;
	CursorGroundPosition = WorldPosition + WorldDirection * DistanceToGround;

	return true;
}
