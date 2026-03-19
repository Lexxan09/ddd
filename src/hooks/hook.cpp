#include "hook.h"
#include "../frontend/menu.h"

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

        ImGuiIO& io      = ImGui::GetIO();
        io.ConfigFlags  |= ImGuiConfigFlags_NoMouseCursorChange;
        io.IniFilename   = nullptr;

        ImGui_ImplWin32_Init(g_hWnd);
        ImGui_ImplOpenGL3_Init("#version 130");

        Menu::ApplyStyle();

        // FIX: Use WNDPROC (not custom typedef) — SetWindowLongPtrW returns LONG_PTR
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

        // Save OpenGL state
        GLint last_program, last_texture, last_ab;
        glGetIntegerv(GL_CURRENT_PROGRAM,      &last_program);
        glGetIntegerv(GL_TEXTURE_BINDING_2D,   &last_texture);
        glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &last_ab);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        Menu::Render();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Restore OpenGL state
        glUseProgram(static_cast<GLuint>(last_program));
        glBindTexture(GL_TEXTURE_2D,  static_cast<GLuint>(last_texture));
        glBindBuffer(GL_ARRAY_BUFFER, static_cast<GLuint>(last_ab));

        return o_wglSwapBuffers(hDc);
    }

    // ─── WndProc Hook ────────────────────────────────────────────────
    // FIX: IMGUI_IMPL_API extern already declared in imgui_impl_win32.h
    // Do NOT re-declare it here. The header include in pch.h handles it.

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
