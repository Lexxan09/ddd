#pragma once


namespace Menu
{
    extern bool g_ShowMenu;
    extern int  g_Page;
    extern int  g_SubPage;

    void ApplyStyle();
    void Render();

    // Widgets
    bool KcCheckbox  (const char* label, bool*  v);
    bool KcButton    (const char* label, ImVec2 size = ImVec2(0,0));
    bool KcSliderInt (const char* label, int*   v, int   mn, int   mx);
    bool KcSliderFloat(const char* label, float* v, float mn, float mx);
    bool KcSubtab    (const char* label, int idx, int& current);
    void KcGroupBox  (const char* label, ImVec2 size, std::function<void()> fn);
}
