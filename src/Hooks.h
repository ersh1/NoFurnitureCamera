#pragma once

namespace Hooks
{
	class FurnitureCameraHooks
	{
	public:
		static void Hook()
		{
			const auto dataHandler = RE::TESDataHandler::GetSingleton();
			if (dataHandler) {
				kywd_furnitureForces1stPerson = dataHandler->LookupForm<RE::BGSKeyword>(0xA56D7, "Skyrim.esm");
				kywd_furnitureForces3rdPerson = dataHandler->LookupForm<RE::BGSKeyword>(0xA56D8, "Skyrim.esm");
			}

			auto& trampoline = SKSE::GetTrampoline();
			REL::Relocation<uintptr_t> hook1{ REL::RelocationID(17034, 17420) };  // 21A4B0, 228450, TESFurniture::Activate

			SKSE::AllocTrampoline(14);
			_EnterFurniture = trampoline.write_call<5>(hook1.address() + REL::VariantOffset(0x295, 0x2A2, 0).offset(), EnterFurniture);
		}

	private:
		static void EnterFurniture(RE::PlayerCamera* a_this, RE::TESFurniture* a_furniture);

		static inline REL::Relocation<decltype(EnterFurniture)> _EnterFurniture;

		static inline RE::BGSKeyword* kywd_furnitureForces1stPerson = nullptr;
		static inline RE::BGSKeyword* kywd_furnitureForces3rdPerson = nullptr;
	};

	void InitCompatibility();
	void Install();

	static inline bool bImprovedCameraInstalled = false;
}
