#pragma once
#include "../pch.h"

namespace Menu
{
    extern bool g_ShowMenu;

    void ApplyStyle();
    void Render();

    // ── Sub-renderers ────────────────────────────────
    void RenderSidebar(ImDrawList* draw, ImVec2 pos);
    void RenderHeader (ImDrawList* draw, ImVec2 pos);
    void RenderMain   (ImVec2 pos);
    void RenderPatches(ImVec2 offset);
    void RenderCheats (ImVec2 offset);
    void RenderMinimap(ImVec2 offset);
    void RenderItemDB (ImVec2 offset);
    void RenderScript (ImVec2 offset);
    void RenderSettings(ImVec2 offset);

    // ── Custom Widgets ───────────────────────────────
    bool KcCheckbox  (const char* label, bool* v);
    bool KcButton    (const char* label, ImVec2 size = ImVec2(0,0));
    bool KcSliderInt (const char* label, int* v, int min, int max);
    bool KcSliderFloat(const char* label, float* v, float min, float max);
    void KcGroupBox  (const char* label, ImVec2 size, std::function<void()> content);
    bool KcSubtab    (const char* label, int idx, int& current);
}
