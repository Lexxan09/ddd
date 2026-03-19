#include "hook.h"
#include "../frontend/menu.h"

namespace Hook
{
    wglSwapBuffers_t o_wglSwapBuffers = nullptr;
    WndProc_t        o_WndProc        = nullptr;
    HWND             g_hWnd           = nullptr;
    bool             g_Initialized    = false;

    // ─── ImGui Init ─────────────────────────────────────────────────
    static void InitImGui(HDC hDc)
    {
        g_hWnd = WindowFromDC(hDc);

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
        io.IniFilename  = nullptr; // jangan buat imgui.ini

        ImGui_ImplWin32_Init(g_hWnd);
        ImGui_ImplOpenGL3_Init();

        // Apply style KatieCute
        Menu::ApplyStyle();

        // Hook WndProc agar input bisa diterima ImGui
        o_WndProc = (WndProc_t)SetWindowLongPtrW(
            g_hWnd, GWLP_WNDPROC, (LONG_PTR)hk_WndProc);

        g_Initialized = true;
    }

    // ─── wglSwapBuffers Hook ─────────────────────────────────────────
    BOOL WINAPI hk_wglSwapBuffers(HDC hDc)
    {
        if (!g_Initialized)
            InitImGui(hDc);

        // Save OpenGL state
        GLint last_program, last_texture, last_array_buffer;
        glGetIntegerv(GL_CURRENT_PROGRAM,       &last_program);
        glGetIntegerv(GL_TEXTURE_BINDING_2D,    &last_texture);
        glGetIntegerv(GL_ARRAY_BUFFER_BINDING,  &last_array_buffer);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        // Render menu
        Menu::Render();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Restore OpenGL state
        glUseProgram(last_program);
        glBindTexture(GL_TEXTURE_2D, last_texture);
        glBindBuffer(GL_ARRAY_BUFFER, last_array_buffer);

        return o_wglSwapBuffers(hDc);
    }

    // ─── WndProc Hook ────────────────────────────────────────────────
    extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(
        HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

    LRESULT CALLBACK hk_WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        // Toggle menu dengan INSERT
        if (uMsg == WM_KEYDOWN && wParam == VK_INSERT)
            Menu::g_ShowMenu = !Menu::g_ShowMenu;

        if (Menu::g_ShowMenu)
        {
            ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);

            // Block input ke game saat menu terbuka
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

        // Hook wglSwapBuffers dari opengl32.dll
        HMODULE hOpenGL = GetModuleHandleA("opengl32.dll");
        if (!hOpenGL) hOpenGL = LoadLibraryA("opengl32.dll");

        void* pTarget = GetProcAddress(hOpenGL, "wglSwapBuffers");

        MH_CreateHook(pTarget,
                      (void*)hk_wglSwapBuffers,
                      (void**)&o_wglSwapBuffers);
        MH_EnableHook(pTarget);
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
