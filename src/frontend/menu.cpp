#include "menu.h"
#include "colors.h"

// ═══════════════════════════════════════════════════════════════════
//   KatieCute Menu  —  Ostin-Style Layout
//   Layout: Sidebar kiri (70px) + Header atas (55px) + Content area
// ═══════════════════════════════════════════════════════════════════

namespace Menu
{
    // ── State ────────────────────────────────────────────────────────
    bool g_ShowMenu     = false;
    int  g_Page         = 0;   // sidebar tab aktif
    int  g_SubPage      = 0;   // subtab aktif

    // ── Feature flags (ported dari DLL lama) ─────────────────────────
    struct Features
    {
        // Patches
        bool mod_fly        = false;
        bool fast_fruits    = false;
        bool can_see_ghost  = false;
        bool no_name        = false;
        bool no_clip        = false;
        bool anti_portal    = false;
        bool night_vision   = false;
        bool no_block       = false;

        // Cheats
        bool draw_sign_label = false;
        bool draw_door_label = false;
        bool entrance_check  = false;
        bool path_finder     = false;
        bool dev_mode        = false;

        // Minimap
        bool render_minimap  = false;
        bool render_esp      = false;
        float render_range   = 20.f;

        // Settings
        bool show_fps        = true;
        bool skip_update     = false;
    } feats;

    // ── Menu dimensions (Ostin style) ────────────────────────────────
    constexpr float W           = 680.f;
    constexpr float H           = 470.f;
    constexpr float SIDEBAR_W   = 70.f;
    constexpr float HEADER_H    = 55.f;

    // ── Sidebar tab names ────────────────────────────────────────────
    const char* g_TabNames[] = {
        "Main", "Minimap", "Script", "Hooks", "Settings"
    };
    constexpr int TAB_COUNT = 5;

    // ── ApplyStyle: Ostin-style ImGuiStyle ───────────────────────────
    void ApplyStyle()
    {
        ImGuiStyle& s = ImGui::GetStyle();

        s.WindowPadding     = ImVec2(0.f, 0.f);
        s.WindowRounding    = 8.f;
        s.WindowBorderSize  = 0.f;
        s.ChildRounding     = 4.f;
        s.ChildBorderSize   = 0.f;
        s.FramePadding      = ImVec2(8.f, 4.f);
        s.FrameRounding     = 4.f;
        s.FrameBorderSize   = 0.f;
        s.ItemSpacing       = ImVec2(8.f, 6.f);
        s.ItemInnerSpacing  = ImVec2(6.f, 4.f);
        s.ScrollbarSize     = 4.f;
        s.ScrollbarRounding = 4.f;
        s.GrabMinSize       = 8.f;
        s.GrabRounding      = 4.f;
        s.TabRounding       = 4.f;
        s.IndentSpacing     = 12.f;

        ImVec4* c = s.Colors;
        c[ImGuiCol_WindowBg]         = Colors::window_bg;
        c[ImGuiCol_ChildBg]          = Colors::child_bg;
        c[ImGuiCol_PopupBg]          = ImColor(12, 4, 18, 240);
        c[ImGuiCol_Border]           = Colors::stroke;
        c[ImGuiCol_FrameBg]          = Colors::chk_bg;
        c[ImGuiCol_FrameBgHovered]   = Colors::btn_hov;
        c[ImGuiCol_FrameBgActive]    = Colors::accent_dark;
        c[ImGuiCol_TitleBg]          = Colors::sidebar_bg;
        c[ImGuiCol_TitleBgActive]    = Colors::sidebar_bg;
        c[ImGuiCol_ScrollbarBg]      = Colors::scroll_bg;
        c[ImGuiCol_ScrollbarGrab]    = Colors::scroll_grab;
        c[ImGuiCol_ScrollbarGrabHovered] = Colors::scroll_hov;
        c[ImGuiCol_ScrollbarGrabActive]  = Colors::accent;
        c[ImGuiCol_CheckMark]        = Colors::chk_mark;
        c[ImGuiCol_SliderGrab]       = Colors::slider_grab;
        c[ImGuiCol_SliderGrabActive] = Colors::accent;
        c[ImGuiCol_Button]           = Colors::btn_bg;
        c[ImGuiCol_ButtonHovered]    = Colors::btn_hov;
        c[ImGuiCol_ButtonActive]     = Colors::accent;
        c[ImGuiCol_Header]           = Colors::sel_active_bg;
        c[ImGuiCol_HeaderHovered]    = Colors::btn_hov;
        c[ImGuiCol_HeaderActive]     = Colors::accent_dark;
        c[ImGuiCol_Text]             = Colors::text_active;
        c[ImGuiCol_TextDisabled]     = Colors::text_inactive;
        c[ImGuiCol_Separator]        = Colors::stroke;
        c[ImGuiCol_Tab]              = Colors::btn_bg;
        c[ImGuiCol_TabHovered]       = Colors::btn_hov;
        c[ImGuiCol_TabActive]        = Colors::accent_dark;
    }

    // ═══════════════════════════════════════════════════════════════
    //   CUSTOM WIDGETS
    // ═══════════════════════════════════════════════════════════════

    // KcCheckbox: checkbox dengan animasi warna pink
    bool KcCheckbox(const char* label, bool* v)
    {
        ImDrawList* draw = ImGui::GetWindowDrawList();
        ImVec2 pos       = ImGui::GetCursorScreenPos();
        float  sz        = 16.f;
        float  rounding  = 3.f;

        ImGui::InvisibleButton(label, ImVec2(ImGui::GetContentRegionAvail().x, sz + 4.f));
        bool hovered = ImGui::IsItemHovered();
        bool clicked = ImGui::IsItemClicked();
        if (clicked) *v = !(*v);

        // Box
        ImU32 box_col = *v
            ? ImGui::ColorConvertFloat4ToU32(Colors::accent)
            : ImGui::ColorConvertFloat4ToU32(Colors::chk_bg);
        ImU32 border_col = ImGui::ColorConvertFloat4ToU32(
            hovered ? Colors::accent_dim : Colors::chk_stroke);

        draw->AddRectFilled(pos + ImVec2(0, 2), pos + ImVec2(sz, sz + 2),
                            box_col, rounding);
        draw->AddRect(pos + ImVec2(0, 2), pos + ImVec2(sz, sz + 2),
                      border_col, rounding);

        // Checkmark
        if (*v)
        {
            ImVec2 a = pos + ImVec2(3.f,  9.f);
            ImVec2 b = pos + ImVec2(6.f, 12.f);
            ImVec2 c_pt = pos + ImVec2(13.f, 5.f);
            draw->AddLine(a, b, IM_COL32(255,255,255,255), 2.f);
            draw->AddLine(b, c_pt, IM_COL32(255,255,255,255), 2.f);
        }

        // Label
        draw->AddText(pos + ImVec2(sz + 8.f, 2.f),
            ImGui::ColorConvertFloat4ToU32(
                hovered ? Colors::text_active : Colors::text_inactive),
            label);

        return clicked;
    }

    // KcButton
    bool KcButton(const char* label, ImVec2 size)
    {
        if (size.x == 0.f) size.x = ImGui::GetContentRegionAvail().x;
        if (size.y == 0.f) size.y = 24.f;

        ImDrawList* draw = ImGui::GetWindowDrawList();
        ImVec2 pos       = ImGui::GetCursorScreenPos();

        ImGui::InvisibleButton(label, size);
        bool hov  = ImGui::IsItemHovered();
        bool clk  = ImGui::IsItemClicked();

        ImU32 bg = ImGui::ColorConvertFloat4ToU32(
            hov ? Colors::btn_hov : Colors::btn_bg);
        ImU32 border = ImGui::ColorConvertFloat4ToU32(
            hov ? Colors::accent_dim : Colors::btn_stroke);

        draw->AddRectFilled(pos, pos + size, bg, 4.f);
        draw->AddRect(pos, pos + size, border, 4.f);

        // Center text
        ImVec2 tsz = ImGui::CalcTextSize(label);
        draw->AddText(pos + (size - tsz) * 0.5f,
            ImGui::ColorConvertFloat4ToU32(Colors::text_active), label);

        return clk;
    }

    // KcSliderInt
    bool KcSliderInt(const char* label, int* v, int mn, int mx)
    {
        ImGui::PushStyleColor(ImGuiCol_FrameBg,       Colors::slider_bg);
        ImGui::PushStyleColor(ImGuiCol_SliderGrab,    Colors::accent);
        ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, Colors::accent_glow);
        ImGui::PushStyleColor(ImGuiCol_Text,          Colors::text_inactive);
        bool changed = ImGui::SliderInt(label, v, mn, mx);
        ImGui::PopStyleColor(4);
        return changed;
    }

    // KcSliderFloat
    bool KcSliderFloat(const char* label, float* v, float mn, float mx)
    {
        ImGui::PushStyleColor(ImGuiCol_FrameBg,       Colors::slider_bg);
        ImGui::PushStyleColor(ImGuiCol_SliderGrab,    Colors::accent);
        ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, Colors::accent_glow);
        ImGui::PushStyleColor(ImGuiCol_Text,          Colors::text_inactive);
        bool changed = ImGui::SliderFloat(label, v, mn, mx, "%.1f");
        ImGui::PopStyleColor(4);
        return changed;
    }

    // KcGroupBox: panel dengan label dan border pink
    void KcGroupBox(const char* label, ImVec2 size,
                    std::function<void()> content)
    {
        ImDrawList* draw = ImGui::GetWindowDrawList();
        ImVec2 pos       = ImGui::GetCursorScreenPos();

        // Border
        draw->AddRect(pos, pos + size,
            ImGui::ColorConvertFloat4ToU32(Colors::stroke), 4.f);

        // Label background break
        ImVec2 tsz = ImGui::CalcTextSize(label);
        draw->AddRectFilled(pos + ImVec2(8.f, -1.f),
                            pos + ImVec2(10.f + tsz.x, 1.f),
                            ImGui::ColorConvertFloat4ToU32(Colors::child_bg));
        draw->AddText(pos + ImVec2(10.f, -7.f),
            ImGui::ColorConvertFloat4ToU32(Colors::child_title), label);

        // Child window for content
        ImGui::SetCursorScreenPos(pos + ImVec2(0, 12.f));
        ImGui::BeginChild(label, size - ImVec2(0, 12.f), false,
                          ImGuiWindowFlags_NoScrollbar);
        ImGui::SetCursorPos(ImVec2(10.f, 8.f));
        content();
        ImGui::EndChild();
    }

    // KcSubtab: subtab bar di header
    bool KcSubtab(const char* label, int idx, int& current)
    {
        ImDrawList* draw = ImGui::GetWindowDrawList();
        ImVec2 pos       = ImGui::GetCursorScreenPos();
        float  w         = ImGui::CalcTextSize(label).x + 20.f;
        float  h         = 22.f;

        ImGui::InvisibleButton(label, ImVec2(w, h));
        bool hov = ImGui::IsItemHovered();
        bool clk = ImGui::IsItemClicked();
        if (clk) current = idx;

        bool active = (current == idx);

        // Text color
        ImU32 tcol = ImGui::ColorConvertFloat4ToU32(
            active ? Colors::subtab_active :
            hov    ? Colors::text_active   : Colors::subtab_inactive);
        draw->AddText(pos + ImVec2(10.f, 3.f), tcol, label);

        // Active underline
        if (active)
            draw->AddLine(pos + ImVec2(10.f, h - 1.f),
                          pos + ImVec2(w - 10.f, h - 1.f),
                          ImGui::ColorConvertFloat4ToU32(Colors::subtab_line),
                          2.f);

        ImGui::SameLine();
        return clk;
    }

    // ═══════════════════════════════════════════════════════════════
    //   SIDEBAR
    // ═══════════════════════════════════════════════════════════════
    void RenderSidebar(ImDrawList* draw, ImVec2 pos)
    {
        // Sidebar background
        draw->AddRectFilled(pos,
            pos + ImVec2(SIDEBAR_W, H),
            ImGui::ColorConvertFloat4ToU32(Colors::sidebar_bg),
            8.f, ImDrawFlags_RoundCornersLeft);

        // Divider
        draw->AddLine(pos + ImVec2(SIDEBAR_W, 0),
                      pos + ImVec2(SIDEBAR_W, H),
                      ImGui::ColorConvertFloat4ToU32(Colors::stroke));

        // Decorative lines
        draw->AddLine(pos + ImVec2(15.f, 176.f),
                      pos + ImVec2(55.f, 176.f),
                      IM_COL32(255,255,255,15));
        draw->AddLine(pos + ImVec2(15.f, 294.f),
                      pos + ImVec2(55.f, 294.f),
                      IM_COL32(255,255,255,15));

        // Katie Cute logo circle
        draw->AddCircleFilled(pos + ImVec2(35.f, 30.f),
                              18.f,
                              ImGui::ColorConvertFloat4ToU32(Colors::accent_dark),
                              60);
        draw->AddCircle(pos + ImVec2(35.f, 30.f),
                        19.f,
                        ImGui::ColorConvertFloat4ToU32(Colors::accent),
                        60, 1.5f);
        // "KC" text di logo
        ImVec2 logo_pos = pos + ImVec2(24.f, 23.f);
        draw->AddText(logo_pos,
            ImGui::ColorConvertFloat4ToU32(Colors::text_active), "KC");

        // Tab buttons
        const char* icons[] = { "Main", "Map", "</>", "Hook", "Set" };
        for (int i = 0; i < TAB_COUNT; i++)
        {
            float  y      = 58.f + i * 72.f;
            bool   active = (g_Page == i);
            ImVec2 tpos   = pos + ImVec2(0.f, y);
            ImVec2 tsize  = ImVec2(SIDEBAR_W, 60.f);

            // Hover / active background
            if (active)
            {
                draw->AddRectFilled(tpos, tpos + tsize,
                    ImGui::ColorConvertFloat4ToU32(Colors::sel_active_bg));
                // Pink left bar
                draw->AddRectFilled(tpos,
                    tpos + ImVec2(3.f, 60.f),
                    ImGui::ColorConvertFloat4ToU32(Colors::accent));
            }

            // Icon text
            ImVec2 it_sz = ImGui::CalcTextSize(icons[i]);
            draw->AddText(tpos + (tsize - it_sz) * 0.5f,
                ImGui::ColorConvertFloat4ToU32(
                    active ? Colors::accent : Colors::sel_text_off),
                icons[i]);

            // Invisible button for click
            ImGui::SetCursorScreenPos(tpos);
            ImGui::InvisibleButton(("##tab_" + std::to_string(i)).c_str(), tsize);
            if (ImGui::IsItemClicked())
            {
                g_Page    = i;
                g_SubPage = 0;
            }
        }
    }

    // ═══════════════════════════════════════════════════════════════
    //   HEADER
    // ═══════════════════════════════════════════════════════════════
    void RenderHeader(ImDrawList* draw, ImVec2 pos)
    {
        // Header background
        draw->AddRectFilled(
            pos + ImVec2(SIDEBAR_W, 0.f),
            pos + ImVec2(W, HEADER_H),
            ImGui::ColorConvertFloat4ToU32(Colors::header_bg),
            8.f, ImDrawFlags_RoundCornersTopRight);

        // Divider bawah header
        draw->AddLine(
            pos + ImVec2(SIDEBAR_W, HEADER_H),
            pos + ImVec2(W, HEADER_H),
            ImGui::ColorConvertFloat4ToU32(Colors::stroke));

        // Subtab buttons
        float start_x = SIDEBAR_W + 12.f;
        ImGui::SetCursorScreenPos(pos + ImVec2(start_x, 16.f));

        if (g_Page == 0) // Main
        {
            KcSubtab("Patches", 0, g_SubPage);
            KcSubtab("Cheats",  1, g_SubPage);
        }
        else if (g_Page == 1) // Minimap
        {
            KcSubtab("Minimap", 0, g_SubPage);
        }
        else if (g_Page == 2) // Script
        {
            KcSubtab("Scripts", 0, g_SubPage);
            KcSubtab("Logs",    1, g_SubPage);
        }
        else if (g_Page == 4) // Settings
        {
            KcSubtab("General", 0, g_SubPage);
            KcSubtab("Debug",   1, g_SubPage);
        }

        // Clock icon pojok kanan
        char time_buf[16];
        SYSTEMTIME st;
        GetLocalTime(&st);
        sprintf_s(time_buf, "%02d:%02d", st.wHour, st.wMinute);
        ImVec2 tsz = ImGui::CalcTextSize(time_buf);
        draw->AddText(pos + ImVec2(W - tsz.x - 12.f, 18.f),
            ImGui::ColorConvertFloat4ToU32(Colors::accent), time_buf);
    }

    // ═══════════════════════════════════════════════════════════════
    //   PAGE RENDERERS
    // ═══════════════════════════════════════════════════════════════

    void RenderPatches(ImVec2 offset)
    {
        ImGui::SetCursorScreenPos(offset);
        ImGui::BeginChild("##patches_area",
            ImVec2(W - SIDEBAR_W - 20.f, H - HEADER_H - 10.f),
            false, ImGuiWindowFlags_NoScrollbar);

        float col_w = (ImGui::GetContentRegionAvail().x - 20.f) * 0.5f;

        // ── Left column ──────────────────────────────
        ImGui::BeginChild("##p_left", ImVec2(col_w, 0.f), false);

        KcGroupBox("Patches", ImVec2(col_w - 10.f, 210.f), [&]()
        {
            KcCheckbox("Mod Fly",       &feats.mod_fly);
            ImGui::Spacing();
            KcCheckbox("Fast Fruits",   &feats.fast_fruits);
            ImGui::Spacing();
            KcCheckbox("Can See Ghost", &feats.can_see_ghost);
            ImGui::Spacing();
            KcCheckbox("No Name",       &feats.no_name);
        });

        ImGui::EndChild();
        ImGui::SameLine(0.f, 20.f);

        // ── Right column ─────────────────────────────
        ImGui::BeginChild("##p_right", ImVec2(col_w, 0.f), false);

        KcGroupBox("More Patches", ImVec2(col_w - 10.f, 210.f), [&]()
        {
            KcCheckbox("No Clip",       &feats.no_clip);
            ImGui::Spacing();
            KcCheckbox("Anti Portal",   &feats.anti_portal);
            ImGui::Spacing();
            KcCheckbox("Night Vision",  &feats.night_vision);
            ImGui::Spacing();
            KcCheckbox("No Block",      &feats.no_block);
        });

        ImGui::EndChild();
        ImGui::EndChild();
    }

    void RenderCheats(ImVec2 offset)
    {
        ImGui::SetCursorScreenPos(offset);
        ImGui::BeginChild("##cheats_area",
            ImVec2(W - SIDEBAR_W - 20.f, H - HEADER_H - 10.f),
            false, ImGuiWindowFlags_NoScrollbar);

        float col_w = (ImGui::GetContentRegionAvail().x - 20.f) * 0.5f;

        ImGui::BeginChild("##c_left", ImVec2(col_w, 0.f), false);
        KcGroupBox("Cheats", ImVec2(col_w - 10.f, 200.f), [&]()
        {
            KcCheckbox("Draw Sign Label", &feats.draw_sign_label);
            ImGui::Spacing();
            KcCheckbox("Draw Door Label", &feats.draw_door_label);
            ImGui::Spacing();
            KcCheckbox("Entrance Check",  &feats.entrance_check);
        });
        ImGui::EndChild();
        ImGui::SameLine(0.f, 20.f);

        ImGui::BeginChild("##c_right", ImVec2(col_w, 0.f), false);
        KcGroupBox("More Cheats", ImVec2(col_w - 10.f, 200.f), [&]()
        {
            KcCheckbox("Path Finder",     &feats.path_finder);
            ImGui::Spacing();
            KcCheckbox("Dev Mode",        &feats.dev_mode);
        });
        ImGui::EndChild();
        ImGui::EndChild();
    }

    void RenderMinimap(ImVec2 offset)
    {
        ImGui::SetCursorScreenPos(offset);
        ImGui::BeginChild("##minimap_area",
            ImVec2(W - SIDEBAR_W - 20.f, H - HEADER_H - 10.f),
            false, ImGuiWindowFlags_NoScrollbar);

        float col_w = (ImGui::GetContentRegionAvail().x - 20.f) * 0.5f;
        ImGui::BeginChild("##mm_left", ImVec2(col_w, 0.f), false);
        KcGroupBox("Minimap", ImVec2(col_w - 10.f, 180.f), [&]()
        {
            KcCheckbox("Render Minimap", &feats.render_minimap);
            ImGui::Spacing();
            KcCheckbox("Render ESP",     &feats.render_esp);
            ImGui::Spacing();
            KcSliderFloat("Range", &feats.render_range, 5.f, 50.f);
        });
        ImGui::EndChild();
        ImGui::EndChild();
    }

    void RenderSettings(ImVec2 offset)
    {
        ImGui::SetCursorScreenPos(offset);
        ImGui::BeginChild("##settings_area",
            ImVec2(W - SIDEBAR_W - 20.f, H - HEADER_H - 10.f),
            false, ImGuiWindowFlags_NoScrollbar);

        float col_w = (ImGui::GetContentRegionAvail().x - 20.f) * 0.5f;
        ImGui::BeginChild("##st_left", ImVec2(col_w, 0.f), false);
        KcGroupBox("General", ImVec2(col_w - 10.f, 160.f), [&]()
        {
            KcCheckbox("Show FPS",    &feats.show_fps);
            ImGui::Spacing();
            KcCheckbox("Skip Update", &feats.skip_update);
        });
        ImGui::EndChild();
        ImGui::EndChild();
    }

    void RenderScript(ImVec2 offset)
    {
        ImGui::SetCursorScreenPos(offset);
        ImGui::BeginChild("##script_area",
            ImVec2(W - SIDEBAR_W - 20.f, H - HEADER_H - 10.f),
            false);

        ImGui::TextColored(Colors::text_inactive,
            "Script executor via .genta / .lua files");
        ImGui::Spacing();
        ImGui::TextColored(Colors::accent,
            "Use in-game Script tab for Lua execution");

        ImGui::EndChild();
    }

    // ═══════════════════════════════════════════════════════════════
    //   MAIN RENDER
    // ═══════════════════════════════════════════════════════════════
    void Render()
    {
        if (!g_ShowMenu) return;

        // ── Window setup ─────────────────────────────────────────────
        ImGui::SetNextWindowSize(ImVec2(W, H), ImGuiCond_Always);
        ImGui::SetNextWindowBgAlpha(0.f); // kita gambar manual

        ImGui::Begin("##katie_main",
            nullptr,
            ImGuiWindowFlags_NoTitleBar    |
            ImGuiWindowFlags_NoResize      |
            ImGuiWindowFlags_NoScrollbar   |
            ImGuiWindowFlags_NoScrollWithMouse |
            ImGuiWindowFlags_NoBringToDisplayOnFocus);

        ImDrawList* draw = ImGui::GetWindowDrawList();
        ImVec2 pos       = ImGui::GetWindowPos();

        // ── Content background ───────────────────────────────────────
        draw->AddRectFilled(
            pos + ImVec2(SIDEBAR_W, HEADER_H),
            pos + ImVec2(W, H),
            ImGui::ColorConvertFloat4ToU32(Colors::window_bg),
            8.f, ImDrawFlags_RoundCornersBottomRight);

        // ── Shadow (subtle) ──────────────────────────────────────────
        for (int i = 4; i >= 1; i--)
        {
            float   off = (float)(i * 3);
            ImU32 sc = ImGui::ColorConvertFloat4ToU32(ImVec4(0.f, 0.f, 0.f, (30 * i) / 255.f));
            draw->AddRect(pos - ImVec2(off,off),
                          pos + ImVec2(W+off, H+off),
                          sc, 10.f);
        }

        // ── Sidebar ──────────────────────────────────────────────────
        RenderSidebar(draw, pos);

        // ── Header / subtabs ─────────────────────────────────────────
        RenderHeader(draw, pos);

        // ── Content area ─────────────────────────────────────────────
        ImVec2 content_offset = pos +
            ImVec2(SIDEBAR_W + 10.f, HEADER_H + 8.f);

        switch (g_Page)
        {
        case 0: // Main
            if (g_SubPage == 0) RenderPatches(content_offset);
            else                RenderCheats (content_offset);
            break;
        case 1: RenderMinimap (content_offset); break;
        case 2: RenderScript  (content_offset); break;
        case 4: RenderSettings(content_offset); break;
        default:
            ImGui::SetCursorScreenPos(content_offset);
            ImGui::TextColored(Colors::text_inactive, "Coming soon...");
            break;
        }

        ImGui::End();
    }

} // namespace Menu
