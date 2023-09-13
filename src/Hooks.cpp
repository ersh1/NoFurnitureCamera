#include "Hooks.h"

#include "Offsets.h"

namespace Hooks
{
    void FurnitureCameraHooks::EnterFurniture(RE::PlayerCamera* a_this, RE::TESFurniture* a_furniture)
    {
		auto playerCamera = RE::PlayerCamera::GetSingleton();

		if (!playerCamera->currentState) {  // idk if that's possible to happen but just in case
			return _EnterFurniture(a_this, a_furniture);
		}

		// just run the original with IC installed if we're not in 3rd person, in case it does important stuff
		if (bImprovedCameraInstalled && playerCamera->currentState->id != RE::CameraState::kThirdPerson) {
			return _EnterFurniture(a_this, a_furniture);
		}

		// Let the original function run in some cases like workbenches (as the game already would force 3rd person along with doing some other unknown stuff)
		if (a_furniture->workBenchData.benchType != RE::TESFurniture::WorkBenchData::BenchType::kNone) {
			return _EnterFurniture(a_this, a_furniture);
		}

		if (a_furniture->HasKeyword(kywd_furnitureForces1stPerson) && playerCamera->currentState->id != RE::CameraState::kFirstPerson) {
			playerCamera->ForceFirstPerson();
		} else if (playerCamera->currentState->id != RE::CameraState::kThirdPerson) {
			PlayerCamera_SetCameraState(playerCamera, RE::CameraState::kThirdPerson);
		}
    }

    void InitCompatibility()
    {
		if (GetModuleHandle("ImprovedCameraSE.dll")) {
			bImprovedCameraInstalled = true;
		}
    }

    void Install()
	{
		logger::info("Hooking...");

		FurnitureCameraHooks::Hook();

		logger::info("...success");
	}
}
