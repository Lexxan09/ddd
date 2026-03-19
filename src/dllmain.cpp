// KatieCute - GTInternal
// Entry point DLL

#include "pch.h"
#include "hooks/hook.h"

DWORD WINAPI MainThread(LPVOID lpParam)
{
    // Tunggu sampai game window siap
    while (!FindWindowA(nullptr, "Growtopia"))
        Sleep(500);

    Sleep(1000); // kasih waktu game load penuh

    // Init hooks
    Hook::Init();

    // Loop sampai unload
    while (!GetAsyncKeyState(VK_END))
        Sleep(100);

    // Cleanup
    Hook::Shutdown();

    FreeLibraryAndExitThread((HMODULE)lpParam, 0);
    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
    if (dwReason == DLL_PROCESS_ATTACH)
    {
        DisableThreadLibraryCalls(hModule);
        CreateThread(nullptr, 0, MainThread, hModule, 0, nullptr);
    }
    return TRUE;
}
