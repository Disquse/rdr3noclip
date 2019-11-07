// dllmain.cpp : Script entry point

#include "pch.h"
#include "Noclip.h"

ALT_LOG_IMPL;

static std::wstring GetModulePath(HMODULE module)
{
	DWORD size = MAX_PATH;
	std::vector<wchar_t> buffer(size);

	do
	{
		buffer.resize(size);
		GetModuleFileNameW(module, buffer.data(), size);
		size *= 1.5;
	} while (GetLastError() == ERROR_INSUFFICIENT_BUFFER);

	std::wstring modulePath = std::wstring(buffer.begin(), buffer.end());

	size_t slashPos = modulePath.size();
	for (int i = modulePath.size() - 1; i >= 0; --i)
	{
		if (modulePath[i] == L'/' || modulePath[i] == L'\\') {
			slashPos = i;
			break;
		}
	}

	std::wstring moduleDir = modulePath.substr(0, slashPos);
	return moduleDir;
}

Noclip noclipResource;

void TickCpp()
{
	noclipResource.Tick();
}

extern "C" {
	DLL_EXPORT void Init(GetNativeAddressFunc getAddress)
	{
		Native::SetEssentialFunction(getAddress);
		Log::Info << "Natives registered" << Log::Endl;
	}

	DLL_EXPORT void Tick()
	{
		TickCpp();
	}

	DLL_EXPORT void OnKeyDown(uint32_t key)
	{
		if (key == 0x71)
		{
			noclipResource.Toggle();
		}
		if (key == 0x10)
		{
			noclipResource.SetHighSpeed(true);
		}
	}

	DLL_EXPORT void OnKeyUp(uint32_t key)
	{
		if (key == 0x10)
		{
			noclipResource.SetHighSpeed(false);
		}
	}
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
	{
		std::wstring _moduleDir = GetModulePath(hModule);

		std::wstring logPath = _moduleDir + L"/noclip-log.txt";
		Log::Push(new Log::FileStream(logPath));
		break;
	}
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

