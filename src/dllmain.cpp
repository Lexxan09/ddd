#include <Windows.h>
#include "hooks/hook.h"

DWORD WINAPI MainThread(LPVOID lp)
{
    while (!FindWindowA(nullptr, "Growtopia")) Sleep(500);
    Sleep(1000);
    Hook::Init();
    while (!GetAsyncKeyState(VK_END)) Sleep(100);
    Hook::Shutdown();
    FreeLibraryAndExitThread((HMODULE)lp, 0);
    return 0;
}

BOOL APIENTRY DllMain(HMODULE hMod, DWORD reason, LPVOID)
{
    if (reason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(hMod);
        CreateThread(nullptr, 0, MainThread, hMod, 0, nullptr);
    }
    return TRUE;
}
