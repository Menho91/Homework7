#include "MyPawn.h"
#include "Components/BoxComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "MyPlayerController.h"

AMyPawn::AMyPawn()
{
	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("RootCollision"));
	SetRootComponent(BoxComp);
	BoxComp->SetSimulatePhysics(false);

	SkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh"));
	SkeletalMeshComp->SetupAttachment(BoxComp);
	SkeletalMeshComp->SetSimulatePhysics(false);

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->SetupAttachment(BoxComp);
	SpringArmComp->TargetArmLength = 350.0f;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
}

void AMyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (AMyPlayerController* PlayerController = Cast<AMyPlayerController>(GetController()))
		{
			if (PlayerController->MoveAction)
			{
				EnhancedInput->BindAction(
					PlayerController->MoveAction,
					ETriggerEvent::Triggered,
					this,
					&AMyPawn::Move
				);
			}

			if (PlayerController->LookAction)
			{
				EnhancedInput->BindAction(
					PlayerController->LookAction,
					ETriggerEvent::Triggered,
					this,
					&AMyPawn::Look
				);
			}
		}
	}
}

void AMyPawn::Move(const FInputActionValue& value)
{
	if (!Controller) return;

	const FVector2D MoveInput = value.Get<FVector2D>();
	const FVector MovingXVector(FVector::XAxisVector * MoveInput.X * 10.0f);
	const FVector MovingYVector(FVector::YAxisVector * MoveInput.Y * 10.0f);

	if (!FMath::IsNearlyZero(MoveInput.X))
	{
		AddActorLocalOffset(MovingXVector);
	}
	
	if (!FMath::IsNearlyZero(MoveInput.Y))
	{
		AddActorLocalOffset(MovingYVector);
	}
}

void AMyPawn::Look(const FInputActionValue& value)
{
	const FVector2D LookInput = value.Get<FVector2D>();

	AddActorLocalRotation(FRotator(0.0f, LookInput.X, 0.0f));
	SpringArmComp->AddLocalRotation(FRotator(LookInput.Y, 0.0f, 0.0f));
}

