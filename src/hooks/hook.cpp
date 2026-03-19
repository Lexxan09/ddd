#include <Windows.h>
#include <gl/GL.h>
#pragma comment(lib, "opengl32.lib")
#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_opengl3.h"
#include "minhook/include/MinHook.h"
#pragma comment(lib, "minhook/lib/MinHook.x64.lib")
#include "hooks/hook.h"
#include "frontend/menu.h"
#include "frontend/colors.h"

namespace Hook
{
    wglSwapBuffers_t o_wglSwapBuffers = nullptr;
    WndProc_t        o_WndProc        = nullptr;
    HWND             g_hWnd           = nullptr;
    bool             g_Initialized    = false;

    static void InitImGui(HDC hDc)
    {
        g_hWnd = WindowFromDC(hDc);
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
        io.IniFilename  = nullptr;
        ImGui_ImplWin32_Init(g_hWnd);
        ImGui_ImplOpenGL3_Init("#version 130");
        Menu::ApplyStyle();
        o_WndProc = (WndProc_t)SetWindowLongPtrW(g_hWnd, GWLP_WNDPROC, (LONG_PTR)hk_WndProc);
        g_Initialized = true;
    }

    BOOL WINAPI hk_wglSwapBuffers(HDC hDc)
    {
        if (!g_Initialized) InitImGui(hDc);
        GLint lp, lt, la;
        glGetIntegerv(GL_CURRENT_PROGRAM,     &lp);
        glGetIntegerv(GL_TEXTURE_BINDING_2D,  &lt);
        glGetIntegerv(GL_ARRAY_BUFFER_BINDING,&la);
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
        Menu::Render();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glUseProgram(lp);
        glBindTexture(GL_TEXTURE_2D, lt);
        glBindBuffer(GL_ARRAY_BUFFER, la);
        return o_wglSwapBuffers(hDc);
    }

    extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND,UINT,WPARAM,LPARAM);

    LRESULT CALLBACK hk_WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        if (uMsg == WM_KEYDOWN && wParam == VK_INSERT)
            Menu::g_ShowMenu = !Menu::g_ShowMenu;
        if (Menu::g_ShowMenu) {
            ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);
            if (ImGui::GetIO().WantCaptureMouse || ImGui::GetIO().WantCaptureKeyboard)
                return TRUE;
        }
        return CallWindowProcW(o_WndProc, hWnd, uMsg, wParam, lParam);
    }

    void Init()
    {
        MH_Initialize();
        HMODULE hGL = GetModuleHandleA("opengl32.dll");
        if (!hGL) hGL = LoadLibraryA("opengl32.dll");
        void* p = (void*)GetProcAddress(hGL, "wglSwapBuffers");
        MH_CreateHook(p, (void*)hk_wglSwapBuffers, (void**)&o_wglSwapBuffers);
        MH_EnableHook(p);
    }

    void Shutdown()
    {
        MH_DisableHook(MH_ALL_HOOKS);
        MH_Uninitialize();
        if (g_hWnd && o_WndProc)
            SetWindowLongPtrW(g_hWnd, GWLP_WNDPROC, (LONG_PTR)o_WndProc);
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();
    }
}
