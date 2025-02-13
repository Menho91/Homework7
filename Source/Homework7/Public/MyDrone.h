#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MyDrone.generated.h"

class UBoxComponent;
class USpringArmComponent;
class UCameraComponent;
struct FInputActionValue;

UCLASS()
class HOMEWORK7_API AMyDrone : public APawn
{
	GENERATED_BODY()
	
public:
	AMyDrone();

protected:
	UFUNCTION()
	void Move(const FInputActionValue& value);

	UFUNCTION()
	virtual void StopMove(const FInputActionValue& value);

	UFUNCTION()
	void Look(const FInputActionValue& value);

	UFUNCTION()
	void StopLook(const FInputActionValue& value);

	void HandleMove(float DeltaTime);
	void HandleLook(float DeltaTime);

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* BoxComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	USkeletalMeshComponent* SkeletalMeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* CameraComp;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MoveSpeed = 600.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float LookRate = 70.0f;

	FVector MoveInput = FVector::ZeroVector;
	FRotator LookInput = FRotator::ZeroRotator;
};
