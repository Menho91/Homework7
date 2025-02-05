#include "MyDrone.h"
#include "DroneController.h"
#include "EnhancedInputComponent.h"

AMyDrone::AMyDrone() : AMyPawn::AMyPawn()
{ }

void AMyDrone::Move(const FInputActionValue& value)
{
	AMyPawn::Move(value);

	const FVector MoveInput = value.Get<FVector>();
	FVector MovingZVector(FVector::ZAxisVector * MoveInput.Z * 10.0f);

	if (!FMath::IsNearlyZero(MoveInput.Z))
	{
		AddActorLocalOffset(MovingZVector);
	}
}

void AMyDrone::Look(const FInputActionValue& value)
{
	const FVector2D LookInput = value.Get<FVector2D>();

	AddActorLocalRotation(FRotator(LookInput.Y, LookInput.X, 0.0f));
}

void AMyDrone::UpDown(const FInputActionValue& value)
{
	const FVector UpDownInput = value.Get<FVector>();

	AddActorLocalOffset(FVector(0.0f, 0.0f, UpDownInput.X));
}

void AMyDrone::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (ADroneController* DroneController = Cast<ADroneController>(GetController()))
		{
			if (DroneController->MoveAction)
			{
				EnhancedInput->BindAction(
					DroneController->MoveAction,
					ETriggerEvent::Triggered,
					this,
					&AMyDrone::Move
				);
			}

			if (DroneController->LookAction)
			{
				EnhancedInput->BindAction(
					DroneController->LookAction,
					ETriggerEvent::Triggered,
					this,
					&AMyDrone::Look
				);
			}

			if (DroneController->UpDownAction)
			{
				EnhancedInput->BindAction(
					DroneController->UpDownAction,
					ETriggerEvent::Triggered,
					this,
					&AMyDrone::UpDown
				);
			}
		}
	}
}

