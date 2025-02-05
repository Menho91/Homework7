#pragma once

#include "CoreMinimal.h"
#include "MyPawn.h"
#include "MyDrone.generated.h"

UCLASS()
class HOMEWORK7_API AMyDrone : public AMyPawn
{
	GENERATED_BODY()
	
public:
	AMyDrone();

protected:
	virtual void Move(const FInputActionValue& value) override;
	virtual void Look(const FInputActionValue& value) override;

	UFUNCTION()
	void UpDown(const FInputActionValue& value);

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
