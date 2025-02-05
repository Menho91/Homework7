#pragma once

#include "CoreMinimal.h"
#include "MyPlayerController.h"
#include "DroneController.generated.h"

UCLASS()
class HOMEWORK7_API ADroneController : public AMyPlayerController
{
	GENERATED_BODY()
	
public:
	ADroneController();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* UpDownAction;
};
