#pragma once
#include <Windows.h>
#include <vector>
#include <TlHelp32.h>

class Memory
{
public:
	//Read and Write Process memory
	template <class valuetype>
	valuetype ReadMemory(HANDLE HandleOfProcess, uintptr_t address)
	{
		valuetype ReadAddress;
		::ReadProcessMemory(HandleOfProcess, (LPBYTE*)address, &ReadAddress, sizeof(ReadAddress), NULL);
		return ReadAddress;
	}
	template <class valuetype>
	valuetype WriteMemory(HANDLE HandleOfProcess, uintptr_t address, valuetype valuetowrite)
	{
		::WriteProcessMemory(HandleOfProcess, address, (LPBYTE*)valuetowrite, sizeof(valuetowrite), NULL);
	}

	static uintptr_t GetProcessID(const wchar_t* ProcessName)
	{
		uintptr_t ProcessID = 0;

		HANDLE SnapshotOfProcess = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
		if (SnapshotOfProcess != INVALID_HANDLE_VALUE)
		{
			PROCESSENTRY32W EntryValue{};
			EntryValue.dwSize = sizeof(EntryValue);

			while (Process32NextW(SnapshotOfProcess, &EntryValue))
			{
				if (!_wcsicmp(EntryValue.szExeFile, ProcessName))
				{
					ProcessID = (uintptr_t)EntryValue.th32ProcessID;
					break;
				}
			}
		}
		return ProcessID;
	}

	static uintptr_t GetModuleBaseAddress(uintptr_t ProcessID, const wchar_t* ModuleName)
	{
		uintptr_t ModuleBaseAddress = 0;

		HANDLE SnapshotOfProcess = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, ProcessID);
		if (SnapshotOfProcess != INVALID_HANDLE_VALUE)
		{
			MODULEENTRY32W EntryValue{};
			EntryValue.dwSize = sizeof(EntryValue);

			while (Module32NextW(SnapshotOfProcess, &EntryValue))
			{
				if (!_wcsicmp(EntryValue.szModule, ModuleName))
				{
					ModuleBaseAddress = (uintptr_t)EntryValue.modBaseAddr;
					break;
				}
			}
		}
		CloseHandle(SnapshotOfProcess);

		return ModuleBaseAddress;
	}
};