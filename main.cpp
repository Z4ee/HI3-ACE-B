#include "utils.h"
#include "hooks.h"

BOOL DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	if (fdwReason == DLL_PROCESS_ATTACH) {

		auto base_address = reinterpret_cast<uint64_t>(GetModuleHandleA("BH3Base.dll"));

		utils::write<uint32_t>(base_address + 0x106420, 0xCCC3C031);
		utils::write<uint8_t>(base_address + 0x102F40, 0xC3);
		utils::write<uint16_t>(base_address + 0x1BCC60, 0xFEEB);

		SetEnvironmentVariableA("PROCESS_FLAG_AAACCCBBB", "PROCESS_FLAG_AAACCCBBB"); // if this is not done, then HI3 will restart

		if (utils::create_console()) {
			CreateThread(0, 0, (LPTHREAD_START_ROUTINE)hooks::init, 0, 0, 0);
		}
	}

	return TRUE;
}