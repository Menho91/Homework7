#include "DroneGameMode.h"
#include "MyDrone.h"
#include "DroneController.h"

ADroneGameMode::ADroneGameMode()
{
	DefaultPawnClass = AMyDrone::StaticClass();
	PlayerControllerClass = ADroneController::StaticClass();
}