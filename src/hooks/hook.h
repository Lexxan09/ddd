#pragma once
#include "../pch.h"

namespace Hook
{
    using wglSwapBuffers_t = BOOL(WINAPI*)(HDC hDc);
    using WndProc_t        = LRESULT(CALLBACK*)(HWND, UINT, WPARAM, LPARAM);

    extern wglSwapBuffers_t o_wglSwapBuffers;
    extern WndProc_t        o_WndProc;
    extern HWND             g_hWnd;
    extern bool             g_Initialized;

    void Init();
    void Shutdown();

    BOOL     WINAPI   hk_wglSwapBuffers(HDC hDc);
    LRESULT  CALLBACK hk_WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
}
