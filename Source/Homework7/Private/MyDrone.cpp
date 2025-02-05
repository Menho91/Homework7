#include "MyDrone.h"
#include "DroneController.h"
#include "EnhancedInputComponent.h"

AMyDrone::AMyDrone() : AMyPawn::AMyPawn()
{ }

void AMyDrone::Move(const FInputActionValue& value)
{
	AMyPawn::Move(value);
}

void AMyDrone::Look(const FInputActionValue& value)
{
	const FVector2D LookInput = value.Get<FVector2D>();

	AddActorLocalRotation(FRotator(LookInput.Y, LookInput.X, 0.0f));
}

void AMyDrone::UpDown(const FInputActionValue& value)
{
	const float UpDownInput = value.Get<float>();

	if (!FMath::IsNearlyZero(UpDownInput))
	{
		AddActorLocalOffset(FVector::ZAxisVector * UpDownInput * 10.0f);
	}
}

void AMyDrone::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	AMyPawn::SetupPlayerInputComponent(PlayerInputComponent);

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

