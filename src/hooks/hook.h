#pragma once
#include <Windows.h>
#include <gl/GL.h>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_opengl3.h"
#include "minhook/include/MinHook.h"

namespace Hook
{
    using wglSwapBuffers_t = BOOL(WINAPI*)(HDC);
    using WndProc_t        = LRESULT(CALLBACK*)(HWND,UINT,WPARAM,LPARAM);

    extern wglSwapBuffers_t o_wglSwapBuffers;
    extern WndProc_t        o_WndProc;
    extern HWND             g_hWnd;
    extern bool             g_Initialized;

    void     Init();
    void     Shutdown();
    BOOL     WINAPI   hk_wglSwapBuffers(HDC hDc);
    LRESULT  CALLBACK hk_WndProc(HWND,UINT,WPARAM,LPARAM);
}
