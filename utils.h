#pragma once
#include <iostream>
#include <windows.h>

namespace utils {
	template <typename T>
	void write(uintptr_t address, T value) {
		DWORD oldProtection;
		VirtualProtect(reinterpret_cast<void**>(address), sizeof(T), PAGE_EXECUTE_READWRITE, &oldProtection);
		*reinterpret_cast<T*>(address) = value;
		VirtualProtect(reinterpret_cast<void**>(address), sizeof(T), oldProtection, &oldProtection);
	}

	template <typename K>
	K read(uintptr_t address) {
		return *reinterpret_cast<K*>(address);
	}

	bool create_console();
}