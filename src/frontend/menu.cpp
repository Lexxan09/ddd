#include "../pch.h"
#include "menu.h"

// Colors namespace is included via pch.h -> colors.h

namespace Menu
{
    bool g_ShowMenu  = false;
    int  g_Page      = 0;
    int  g_SubPage   = 0;

    // ── Feature flags ────────────────────────────────────────────────
    struct Features {
        bool mod_fly         = false;
        bool fast_fruits     = false;
        bool can_see_ghost   = false;
        bool no_name         = false;
        bool no_clip         = false;
        bool anti_portal     = false;
        bool night_vision    = false;
        bool no_block        = false;
        bool draw_sign_label = false;
        bool draw_door_label = false;
        bool entrance_check  = false;
        bool path_finder     = false;
        bool dev_mode        = false;
        bool render_minimap  = false;
        bool render_esp      = false;
        float render_range   = 20.f;
        bool show_fps        = true;
        bool skip_update     = false;
    } feats;

    constexpr float W         = 680.f;
    constexpr float H         = 470.f;
    constexpr float SIDEBAR_W = 70.f;
    constexpr float HEADER_H  = 55.f;

    // ── Helper: convert ImVec4 to ImU32 ──────────────────────────────
    static ImU32 C(const ImVec4& v) {
        return ImGui::ColorConvertFloat4ToU32(v);
    }

    // ── ApplyStyle ───────────────────────────────────────────────────
    void ApplyStyle()
    {
        ImGuiStyle& s = ImGui::GetStyle();
        s.WindowPadding    = ImVec2(0.f, 0.f);
        s.WindowRounding   = 8.f;
        s.WindowBorderSize = 0.f;
        s.ChildRounding    = 4.f;
        s.ChildBorderSize  = 0.f;
        s.FramePadding     = ImVec2(8.f, 4.f);
        s.FrameRounding    = 4.f;
        s.FrameBorderSize  = 0.f;
        s.ItemSpacing      = ImVec2(8.f, 6.f);
        s.ScrollbarSize    = 4.f;
        s.ScrollbarRounding= 4.f;
        s.GrabMinSize      = 8.f;
        s.GrabRounding     = 4.f;
        s.TabRounding      = 4.f;

        ImVec4* c = s.Colors;
        c[ImGuiCol_WindowBg]            = Colors::window_bg;
        c[ImGuiCol_ChildBg]             = Colors::child_bg;
        c[ImGuiCol_PopupBg]             = ImColor(12, 4, 18, 240);
        c[ImGuiCol_Border]              = Colors::stroke;
        c[ImGuiCol_FrameBg]             = Colors::chk_bg;
        c[ImGuiCol_FrameBgHovered]      = Colors::btn_hov;
        c[ImGuiCol_FrameBgActive]       = Colors::accent_dark;
        c[ImGuiCol_TitleBg]             = Colors::sidebar_bg;
        c[ImGuiCol_TitleBgActive]       = Colors::sidebar_bg;
        c[ImGuiCol_ScrollbarBg]         = Colors::scroll_bg;
        c[ImGuiCol_ScrollbarGrab]       = Colors::scroll_grab;
        c[ImGuiCol_ScrollbarGrabHovered]= Colors::scroll_hov;
        c[ImGuiCol_ScrollbarGrabActive] = Colors::accent;
        c[ImGuiCol_CheckMark]           = Colors::chk_mark;
        c[ImGuiCol_SliderGrab]          = Colors::slider_grab;
        c[ImGuiCol_SliderGrabActive]    = Colors::accent;
        c[ImGuiCol_Button]              = Colors::btn_bg;
        c[ImGuiCol_ButtonHovered]       = Colors::btn_hov;
        c[ImGuiCol_ButtonActive]        = Colors::accent;
        c[ImGuiCol_Header]              = Colors::sel_active_bg;
        c[ImGuiCol_HeaderHovered]       = Colors::btn_hov;
        c[ImGuiCol_HeaderActive]        = Colors::accent_dark;
        c[ImGuiCol_Text]                = Colors::text_active;
        c[ImGuiCol_TextDisabled]        = Colors::text_inactive;
        c[ImGuiCol_Separator]           = Colors::stroke;
        c[ImGuiCol_Tab]                 = Colors::btn_bg;
        c[ImGuiCol_TabHovered]          = Colors::btn_hov;
        c[ImGuiCol_TabActive]           = Colors::accent_dark;
    }

    // ═══════════════════════════════════════════════════════════════
    //   CUSTOM WIDGETS
    // ═══════════════════════════════════════════════════════════════

    bool KcCheckbox(const char* label, bool* v)
    {
        ImDrawList* draw = ImGui::GetWindowDrawList();
        ImVec2 pos  = ImGui::GetCursorScreenPos();
        float  sz   = 16.f;

        ImGui::InvisibleButton(label, ImVec2(ImGui::GetContentRegionAvail().x, sz + 4.f));
        bool hov = ImGui::IsItemHovered();
        bool clk = ImGui::IsItemClicked();
        if (clk) *v = !(*v);

        // Box
        draw->AddRectFilled(pos + ImVec2(0,2), pos + ImVec2(sz, sz+2),
            *v ? C(Colors::accent) : C(Colors::chk_bg), 3.f);
        draw->AddRect(pos + ImVec2(0,2), pos + ImVec2(sz, sz+2),
            hov ? C(Colors::accent_dim) : C(Colors::chk_stroke), 3.f);

        // Checkmark
        if (*v) {
            draw->AddLine(pos+ImVec2(3.f,9.f),  pos+ImVec2(6.f,12.f), IM_COL32(255,255,255,255), 2.f);
            draw->AddLine(pos+ImVec2(6.f,12.f), pos+ImVec2(13.f,5.f), IM_COL32(255,255,255,255), 2.f);
        }

        // Label - use ImGui::SameLine approach instead of AddText with 2 args
        ImVec2 lpos = pos + ImVec2(sz + 8.f, 2.f);
        draw->AddText(ImGui::GetFont(), ImGui::GetFontSize(), lpos,
            hov ? C(Colors::text_active) : C(Colors::text_inactive), label);

        return clk;
    }

    bool KcButton(const char* label, ImVec2 size)
    {
        if (size.x == 0.f) size.x = ImGui::GetContentRegionAvail().x;
        if (size.y == 0.f) size.y = 24.f;

        ImDrawList* draw = ImGui::GetWindowDrawList();
        ImVec2 pos = ImGui::GetCursorScreenPos();

        ImGui::InvisibleButton(label, size);
        bool hov = ImGui::IsItemHovered();
        bool clk = ImGui::IsItemClicked();

        draw->AddRectFilled(pos, pos+size, hov ? C(Colors::btn_hov) : C(Colors::btn_bg), 4.f);
        draw->AddRect(pos, pos+size, hov ? C(Colors::accent_dim) : C(Colors::btn_stroke), 4.f);

        ImVec2 tsz = ImGui::CalcTextSize(label);
        draw->AddText(ImGui::GetFont(), ImGui::GetFontSize(),
            pos + (size - tsz) * 0.5f, C(Colors::text_active), label);

        return clk;
    }

    bool KcSliderInt(const char* label, int* v, int mn, int mx)
    {
        ImGui::PushStyleColor(ImGuiCol_FrameBg,          Colors::slider_bg);
        ImGui::PushStyleColor(ImGuiCol_SliderGrab,       Colors::accent);
        ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, Colors::accent_glow);
        ImGui::PushStyleColor(ImGuiCol_Text,             Colors::text_inactive);
        bool r = ImGui::SliderInt(label, v, mn, mx);
        ImGui::PopStyleColor(4);
        return r;
    }

    bool KcSliderFloat(const char* label, float* v, float mn, float mx)
    {
        ImGui::PushStyleColor(ImGuiCol_FrameBg,          Colors::slider_bg);
        ImGui::PushStyleColor(ImGuiCol_SliderGrab,       Colors::accent);
        ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, Colors::accent_glow);
        ImGui::PushStyleColor(ImGuiCol_Text,             Colors::text_inactive);
        bool r = ImGui::SliderFloat(label, v, mn, mx, "%.1f");
        ImGui::PopStyleColor(4);
        return r;
    }

    void KcGroupBox(const char* label, ImVec2 size, std::function<void()> content)
    {
        ImDrawList* draw = ImGui::GetWindowDrawList();
        ImVec2 pos = ImGui::GetCursorScreenPos();

        draw->AddRect(pos, pos+size, C(Colors::stroke), 4.f);

        ImVec2 tsz = ImGui::CalcTextSize(label);
        draw->AddRectFilled(pos + ImVec2(8.f,-1.f), pos + ImVec2(10.f+tsz.x, 1.f), C(Colors::child_bg));
        draw->AddText(ImGui::GetFont(), ImGui::GetFontSize(),
            pos + ImVec2(10.f, -7.f), C(Colors::child_title), label);

        ImGui::SetCursorScreenPos(pos + ImVec2(0, 12.f));
        ImGui::BeginChild(label, size - ImVec2(0, 12.f), false, ImGuiWindowFlags_NoScrollbar);
        ImGui::SetCursorPos(ImVec2(10.f, 8.f));
        content();
        ImGui::EndChild();
    }

    bool KcSubtab(const char* label, int idx, int& current)
    {
        ImDrawList* draw = ImGui::GetWindowDrawList();
        ImVec2 pos = ImGui::GetCursorScreenPos();
        float  w   = ImGui::CalcTextSize(label).x + 20.f;
        float  h   = 22.f;

        ImGui::InvisibleButton(label, ImVec2(w, h));
        bool hov = ImGui::IsItemHovered();
        bool clk = ImGui::IsItemClicked();
        if (clk) current = idx;
        bool active = (current == idx);

        ImU32 tcol = active ? C(Colors::subtab_active)
                   : hov    ? C(Colors::text_active)
                   :          C(Colors::subtab_inactive);

        draw->AddText(ImGui::GetFont(), ImGui::GetFontSize(),
            pos + ImVec2(10.f, 3.f), tcol, label);

        if (active)
            draw->AddLine(pos + ImVec2(10.f, h-1.f),
                          pos + ImVec2(w-10.f, h-1.f),
                          C(Colors::subtab_line), 2.f);

        ImGui::SameLine();
        return clk;
    }

    // ═══════════════════════════════════════════════════════════════
    //   SIDEBAR
    // ═══════════════════════════════════════════════════════════════
    void RenderSidebar(ImDrawList* draw, ImVec2 pos)
    {
        draw->AddRectFilled(pos, pos+ImVec2(SIDEBAR_W,H),
            C(Colors::sidebar_bg), 8.f, ImDrawFlags_RoundCornersLeft);
        draw->AddLine(pos+ImVec2(SIDEBAR_W,0), pos+ImVec2(SIDEBAR_W,H),
            C(Colors::stroke));

        // KC logo
        draw->AddCircleFilled(pos+ImVec2(35.f,30.f), 18.f, C(Colors::accent_dark), 60);
        draw->AddCircle(pos+ImVec2(35.f,30.f), 19.f, C(Colors::accent), 60, 1.5f);
        draw->AddText(ImGui::GetFont(), ImGui::GetFontSize(),
            pos+ImVec2(24.f,23.f), C(Colors::text_active), "KC");

        // Decorative lines
        draw->AddLine(pos+ImVec2(15.f,176.f), pos+ImVec2(55.f,176.f), IM_COL32(255,255,255,15));
        draw->AddLine(pos+ImVec2(15.f,294.f), pos+ImVec2(55.f,294.f), IM_COL32(255,255,255,15));

        // Tab buttons
        const char* icons[] = {"Main","Map","</>","Hook","Set"};
        for (int i = 0; i < 5; i++) {
            float  y      = 58.f + i * 72.f;
            bool   active = (g_Page == i);
            ImVec2 tpos   = pos + ImVec2(0.f, y);
            ImVec2 tsize  = ImVec2(SIDEBAR_W, 60.f);

            if (active) {
                draw->AddRectFilled(tpos, tpos+tsize, C(Colors::sel_active_bg));
                draw->AddRectFilled(tpos, tpos+ImVec2(3.f,60.f), C(Colors::accent));
            }

            ImVec2 itsz = ImGui::CalcTextSize(icons[i]);
            draw->AddText(ImGui::GetFont(), ImGui::GetFontSize(),
                tpos + (tsize - itsz) * 0.5f,
                active ? C(Colors::accent) : C(Colors::sel_text_off),
                icons[i]);

            ImGui::SetCursorScreenPos(tpos);
            std::string btn_id = "##tab_" + std::to_string(i);
            ImGui::InvisibleButton(btn_id.c_str(), tsize);
            if (ImGui::IsItemClicked()) { g_Page = i; g_SubPage = 0; }
        }
    }

    // ═══════════════════════════════════════════════════════════════
    //   HEADER
    // ═══════════════════════════════════════════════════════════════
    void RenderHeader(ImDrawList* draw, ImVec2 pos)
    {
        draw->AddRectFilled(
            pos+ImVec2(SIDEBAR_W,0.f), pos+ImVec2(W,HEADER_H),
            C(Colors::header_bg), 8.f, ImDrawFlags_RoundCornersTopRight);
        draw->AddLine(pos+ImVec2(SIDEBAR_W,HEADER_H), pos+ImVec2(W,HEADER_H),
            C(Colors::stroke));

        float start_x = SIDEBAR_W + 12.f;
        ImGui::SetCursorScreenPos(pos + ImVec2(start_x, 16.f));

        if (g_Page == 0) { KcSubtab("Patches",0,g_SubPage); KcSubtab("Cheats",1,g_SubPage); }
        else if (g_Page == 1) { KcSubtab("Minimap",0,g_SubPage); }
        else if (g_Page == 2) { KcSubtab("Scripts",0,g_SubPage); KcSubtab("Logs",1,g_SubPage); }
        else if (g_Page == 4) { KcSubtab("General",0,g_SubPage); KcSubtab("Debug",1,g_SubPage); }

        // Clock
        char tbuf[16];
        SYSTEMTIME st; GetLocalTime(&st);
        sprintf_s(tbuf, "%02d:%02d", st.wHour, st.wMinute);
        ImVec2 tsz = ImGui::CalcTextSize(tbuf);
        draw->AddText(ImGui::GetFont(), ImGui::GetFontSize(),
            pos+ImVec2(W-tsz.x-12.f,18.f), C(Colors::accent), tbuf);
    }

    // ═══════════════════════════════════════════════════════════════
    //   PAGE CONTENT
    // ═══════════════════════════════════════════════════════════════
    void RenderPatches(ImVec2 offset)
    {
        ImGui::SetCursorScreenPos(offset);
        ImGui::BeginChild("##patches", ImVec2(W-SIDEBAR_W-20.f, H-HEADER_H-10.f),
            false, ImGuiWindowFlags_NoScrollbar);

        float col_w = (ImGui::GetContentRegionAvail().x - 20.f) * 0.5f;

        ImGui::BeginChild("##pl", ImVec2(col_w,0.f), false);
        KcGroupBox("Patches", ImVec2(col_w-10.f, 210.f), [&](){
            KcCheckbox("Mod Fly",       &feats.mod_fly);       ImGui::Spacing();
            KcCheckbox("Fast Fruits",   &feats.fast_fruits);   ImGui::Spacing();
            KcCheckbox("Can See Ghost", &feats.can_see_ghost); ImGui::Spacing();
            KcCheckbox("No Name",       &feats.no_name);
        });
        ImGui::EndChild(); ImGui::SameLine(0.f,20.f);

        ImGui::BeginChild("##pr", ImVec2(col_w,0.f), false);
        KcGroupBox("More Patches", ImVec2(col_w-10.f, 210.f), [&](){
            KcCheckbox("No Clip",      &feats.no_clip);      ImGui::Spacing();
            KcCheckbox("Anti Portal",  &feats.anti_portal);  ImGui::Spacing();
            KcCheckbox("Night Vision", &feats.night_vision); ImGui::Spacing();
            KcCheckbox("No Block",     &feats.no_block);
        });
        ImGui::EndChild();
        ImGui::EndChild();
    }

    void RenderCheats(ImVec2 offset)
    {
        ImGui::SetCursorScreenPos(offset);
        ImGui::BeginChild("##cheats", ImVec2(W-SIDEBAR_W-20.f, H-HEADER_H-10.f),
            false, ImGuiWindowFlags_NoScrollbar);

        float col_w = (ImGui::GetContentRegionAvail().x - 20.f) * 0.5f;

        ImGui::BeginChild("##cl", ImVec2(col_w,0.f), false);
        KcGroupBox("Cheats", ImVec2(col_w-10.f,200.f), [&](){
            KcCheckbox("Draw Sign Label", &feats.draw_sign_label); ImGui::Spacing();
            KcCheckbox("Draw Door Label", &feats.draw_door_label); ImGui::Spacing();
            KcCheckbox("Entrance Check",  &feats.entrance_check);
        });
        ImGui::EndChild(); ImGui::SameLine(0.f,20.f);

        ImGui::BeginChild("##cr", ImVec2(col_w,0.f), false);
        KcGroupBox("More Cheats", ImVec2(col_w-10.f,200.f), [&](){
            KcCheckbox("Path Finder", &feats.path_finder); ImGui::Spacing();
            KcCheckbox("Dev Mode",    &feats.dev_mode);
        });
        ImGui::EndChild();
        ImGui::EndChild();
    }

    void RenderMinimap(ImVec2 offset)
    {
        ImGui::SetCursorScreenPos(offset);
        ImGui::BeginChild("##minimap", ImVec2(W-SIDEBAR_W-20.f, H-HEADER_H-10.f), false);
        float col_w = (ImGui::GetContentRegionAvail().x - 20.f) * 0.5f;
        ImGui::BeginChild("##ml", ImVec2(col_w,0.f), false);
        KcGroupBox("Minimap", ImVec2(col_w-10.f,180.f), [&](){
            KcCheckbox("Render Minimap", &feats.render_minimap); ImGui::Spacing();
            KcCheckbox("Render ESP",     &feats.render_esp);     ImGui::Spacing();
            KcSliderFloat("Range", &feats.render_range, 5.f, 50.f);
        });
        ImGui::EndChild();
        ImGui::EndChild();
    }

    void RenderSettings(ImVec2 offset)
    {
        ImGui::SetCursorScreenPos(offset);
        ImGui::BeginChild("##settings", ImVec2(W-SIDEBAR_W-20.f, H-HEADER_H-10.f), false);
        float col_w = (ImGui::GetContentRegionAvail().x - 20.f) * 0.5f;
        ImGui::BeginChild("##sl", ImVec2(col_w,0.f), false);
        KcGroupBox("General", ImVec2(col_w-10.f,160.f), [&](){
            KcCheckbox("Show FPS",    &feats.show_fps);    ImGui::Spacing();
            KcCheckbox("Skip Update", &feats.skip_update);
        });
        ImGui::EndChild();
        ImGui::EndChild();
    }

    void RenderScript(ImVec2 offset)
    {
        ImGui::SetCursorScreenPos(offset);
        ImGui::BeginChild("##script", ImVec2(W-SIDEBAR_W-20.f, H-HEADER_H-10.f), false);
        ImGui::TextColored(Colors::text_inactive, "Use in-game Script tab for Lua execution");
        ImGui::EndChild();
    }

    // ═══════════════════════════════════════════════════════════════
    //   MAIN RENDER
    // ═══════════════════════════════════════════════════════════════
    void Render()
    {
        if (!g_ShowMenu) return;

        ImGui::SetNextWindowSize(ImVec2(W,H), ImGuiCond_Always);
        ImGui::SetNextWindowBgAlpha(0.f);

        ImGui::Begin("##katie_main", nullptr,
            ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse |
            ImGuiWindowFlags_NoBringToDisplayOnFocus);

        ImDrawList* draw = ImGui::GetWindowDrawList();
        ImVec2 pos       = ImGui::GetWindowPos();

        // Content bg
        draw->AddRectFilled(pos+ImVec2(SIDEBAR_W,HEADER_H), pos+ImVec2(W,H),
            C(Colors::window_bg), 8.f, ImDrawFlags_RoundCornersBottomRight);

        // Subtle shadow rings
        for (int i = 4; i >= 1; i--) {
            float off = (float)(i * 3);
            draw->AddRect(pos-ImVec2(off,off), pos+ImVec2(W+off,H+off),
                IM_COL32(0,0,0, 30*i), 10.f);
        }

        RenderSidebar(draw, pos);
        RenderHeader(draw, pos);

        ImVec2 co = pos + ImVec2(SIDEBAR_W+10.f, HEADER_H+8.f);
        switch (g_Page) {
            case 0: g_SubPage==0 ? RenderPatches(co) : RenderCheats(co); break;
            case 1: RenderMinimap(co);  break;
            case 2: RenderScript(co);   break;
            case 4: RenderSettings(co); break;
            default:
                ImGui::SetCursorScreenPos(co);
                ImGui::TextColored(Colors::text_inactive, "Coming soon...");
                break;
        }

        ImGui::End();
    }

} // namespace Menu
