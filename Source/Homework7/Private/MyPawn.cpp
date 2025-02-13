#include "MyPawn.h"
#include "Components/BoxComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "MyPlayerController.h"

AMyPawn::AMyPawn()
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
	SpringArmComp->bUsePawnControlRotation = false;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	CameraComp->bUsePawnControlRotation = false;
}

void AMyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	HandleMove(DeltaTime);
	HandleLook(DeltaTime);
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

				EnhancedInput->BindAction(
					PlayerController->MoveAction,
					ETriggerEvent::Completed,
					this,
					&AMyPawn::StopMove
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

				EnhancedInput->BindAction(
					PlayerController->LookAction,
					ETriggerEvent::Completed,
					this,
					&AMyPawn::StopLook
				);
			}
		}
	}
}

void AMyPawn::HandleMove(float DeltaTime)
{
	if (!FMath::IsNearlyZero(MoveInput.X))
	{
		FVector MovingXVector = FVector::XAxisVector * MoveInput.X * WalkSpeed * DeltaTime;
		AddActorLocalOffset(MovingXVector);
	}

	if (!FMath::IsNearlyZero(MoveInput.Y))
	{
		FVector MovingYVector = FVector::YAxisVector * MoveInput.Y * WalkSpeed * DeltaTime;
		AddActorLocalOffset(MovingYVector);
	}
}

void AMyPawn::HandleLook(float DeltaTime)
{
	if (!FMath::IsNearlyZero(LookInput.X))
	{
		const float YawRotation = LookInput.X * LookRate * DeltaTime;
		AddActorLocalRotation(FRotator(0.0f, YawRotation, 0.0f));
	}

	if (!FMath::IsNearlyZero(LookInput.Y))
	{
		FRotator CurrentArmRotator = SpringArmComp->GetRelativeRotation();
		const float PitchRotation = LookInput.Y * LookRate * DeltaTime;
		CurrentArmRotator.Pitch = FMath::Clamp(CurrentArmRotator.Pitch + PitchRotation, -80.0f, 80.0f);
		SpringArmComp->SetRelativeRotation(CurrentArmRotator);
	}
}

void AMyPawn::Move(const FInputActionValue& value)
{
	MoveInput = value.Get<FVector2D>();
}

void AMyPawn::StopMove(const FInputActionValue& value)
{
	MoveInput = FVector2D::ZeroVector;
}

void AMyPawn::Look(const FInputActionValue& value)
{
	LookInput = value.Get<FVector2D>();
}

void AMyPawn::StopLook(const FInputActionValue& value)
{
	LookInput = FVector2D::ZeroVector;
}

