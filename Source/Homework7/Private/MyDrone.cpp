#include "MyDrone.h"
#include "Components/BoxComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "DroneController.h"

AMyDrone::AMyDrone()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("RootCollision"));
	SetRootComponent(BoxComp);
	BoxComp->SetSimulatePhysics(false);

	SkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh"));
	SkeletalMeshComp->SetupAttachment(BoxComp);
	SkeletalMeshComp->SetSimulatePhysics(false);

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->SetupAttachment(BoxComp);
	SpringArmComp->TargetArmLength = 350.0f;
	SpringArmComp->bUsePawnControlRotation = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	CameraComp->bUsePawnControlRotation = false;
}

void AMyDrone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	HandleMove(DeltaTime);
	HandleLook(DeltaTime);
}

void AMyDrone::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
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

				EnhancedInput->BindAction(
					DroneController->MoveAction,
					ETriggerEvent::Completed,
					this,
					&AMyDrone::StopMove
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

				EnhancedInput->BindAction(
					DroneController->LookAction,
					ETriggerEvent::Completed,
					this,
					&AMyDrone::StopLook
				);
			}
		}
	}
}

void AMyDrone::HandleMove(float DeltaTime)
{
	if (!FMath::IsNearlyZero(MoveInput.X))
	{
		FVector MovingXVector = FVector::XAxisVector * MoveInput.X * MoveSpeed * DeltaTime;
		AddActorLocalOffset(MovingXVector);
	}

	if (!FMath::IsNearlyZero(MoveInput.Y))
	{
		FVector MovingYVector = FVector::YAxisVector * MoveInput.Y * MoveSpeed * DeltaTime;
		AddActorLocalOffset(MovingYVector);
	}

	if (!FMath::IsNearlyZero(MoveInput.Z))
	{
		FVector MovingZVector = FVector::ZAxisVector * MoveInput.Z * MoveSpeed * DeltaTime;
		AddActorWorldOffset(MovingZVector);
	}
}

void AMyDrone::HandleLook(float DeltaTime)
{
	FRotator CurrentRotator = GetActorRotation();

	const float RollRotation = LookInput.Roll * LookRate * DeltaTime * 10.0f;
	const float PitchRotation = LookInput.Pitch * LookRate * DeltaTime;
	const float YawRotation = LookInput.Yaw * LookRate * DeltaTime;

	CurrentRotator.Roll = FMath::Clamp(CurrentRotator.Roll + RollRotation, -80.0f, 80.0f);
	CurrentRotator.Pitch = FMath::Clamp(CurrentRotator.Pitch + PitchRotation, -80.0f, 80.0f);
	CurrentRotator.Yaw = CurrentRotator.Yaw + YawRotation;

	SetActorRotation(CurrentRotator);
}

void AMyDrone::Move(const FInputActionValue& value)
{
	MoveInput = value.Get<FVector>();
}

void AMyDrone::StopMove(const FInputActionValue& value)
{
	MoveInput = FVector::ZeroVector;
}

void AMyDrone::Look(const FInputActionValue& value)
{
	const FVector RotationInput = value.Get<FVector>();

	LookInput.Roll = RotationInput.X;
	LookInput.Pitch = RotationInput.Y;
	LookInput.Yaw = RotationInput.Z;
}

void AMyDrone::StopLook(const FInputActionValue& value)
{
	LookInput = FRotator::ZeroRotator;
}

