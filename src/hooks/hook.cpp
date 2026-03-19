#include "hook.h"
#include "../frontend/menu.h"

// Explicit extern — avoids IMGUI_IMPL_API dllimport linkage issue when building as DLL
extern "C" LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace Hook
{
    wglSwapBuffers_t o_wglSwapBuffers = nullptr;
    WndProc_t        o_WndProc        = nullptr;
    HWND             g_hWnd           = nullptr;
    bool             g_Initialized    = false;

    // ─── ImGui Init ──────────────────────────────────────────────────
    static void InitImGui(HDC hDc)
    {
        g_hWnd = WindowFromDC(hDc);

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        ImGuiIO& io     = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
        io.IniFilename  = nullptr;

        ImGui_ImplWin32_Init(g_hWnd);
        ImGui_ImplOpenGL3_Init("#version 130");

        Menu::ApplyStyle();

        o_WndProc = reinterpret_cast<WNDPROC>(
            SetWindowLongPtrW(g_hWnd, GWLP_WNDPROC,
                              reinterpret_cast<LONG_PTR>(hk_WndProc)));

        g_Initialized = true;
    }

    // ─── wglSwapBuffers Hook ─────────────────────────────────────────
    BOOL WINAPI hk_wglSwapBuffers(HDC hDc)
    {
        if (!g_Initialized)
            InitImGui(hDc);

        // imgui_impl_opengl3 already saves/restores ALL OpenGL state internally.
        // We must NOT call glUseProgram/glBindBuffer etc. — those are OpenGL 2.0+
        // and require an extension loader. <gl/GL.h> only has OpenGL 1.1.

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        Menu::Render();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        return o_wglSwapBuffers(hDc);
    }

    // ─── WndProc Hook ────────────────────────────────────────────────
    LRESULT CALLBACK hk_WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        if (uMsg == WM_KEYDOWN && wParam == VK_INSERT)
            Menu::g_ShowMenu = !Menu::g_ShowMenu;

        if (Menu::g_ShowMenu)
        {
            if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
                return TRUE;

            if (ImGui::GetIO().WantCaptureMouse ||
                ImGui::GetIO().WantCaptureKeyboard)
                return TRUE;
        }

        return CallWindowProcW(o_WndProc, hWnd, uMsg, wParam, lParam);
    }

    // ─── Init & Shutdown ─────────────────────────────────────────────
    void Init()
    {
        MH_Initialize();

        HMODULE hGL = GetModuleHandleA("opengl32.dll");
        if (!hGL) hGL = LoadLibraryA("opengl32.dll");

        void* pTarget = reinterpret_cast<void*>(
            GetProcAddress(hGL, "wglSwapBuffers"));

        MH_CreateHook(pTarget,
                      reinterpret_cast<void*>(hk_wglSwapBuffers),
                      reinterpret_cast<void**>(&o_wglSwapBuffers));
        MH_EnableHook(pTarget);
    }

    void Shutdown()
    {
        MH_DisableHook(MH_ALL_HOOKS);
        MH_Uninitialize();

        if (g_hWnd && o_WndProc)
            SetWindowLongPtrW(g_hWnd, GWLP_WNDPROC,
                              reinterpret_cast<LONG_PTR>(o_WndProc));

        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();
    }

} // namespace Hook
