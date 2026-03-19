#pragma once
#include "../pch.h"

// ═══════════════════════════════════════════════════
//   KatieCute Color Palette  -  Pink Dark Theme
// ═══════════════════════════════════════════════════

namespace Colors
{
    // ── Accent ──────────────────────────────────────
    inline ImVec4 accent         = ImColor(255, 105, 180, 255); // hot pink
    inline ImVec4 accent_dim     = ImColor(255, 105, 180, 160);
    inline ImVec4 accent_glow    = ImColor(255,  20, 147, 255); // deep pink
    inline ImVec4 accent_dark    = ImColor(180,  50, 120, 255);

    // ── Window ──────────────────────────────────────
    inline ImVec4 window_bg      = ImColor( 10,   3,  15, 240); // dark purple-black
    inline ImVec4 sidebar_bg     = ImColor( 15,   5,  22, 245);
    inline ImVec4 header_bg      = ImColor( 15,   5,  22, 245);
    inline ImVec4 stroke         = ImColor( 50,  20,  60, 255); // purple stroke
    inline ImVec4 stroke_bright  = ImColor( 80,  30,  90, 255);

    // ── Child panels ────────────────────────────────
    inline ImVec4 child_bg       = ImColor( 18,   6,  26, 200);
    inline ImVec4 child_label    = ImColor(200, 150, 200, 255);
    inline ImVec4 child_title    = ImColor(255, 105, 180, 255);

    // ── Text ────────────────────────────────────────
    inline ImVec4 text_active    = ImColor(255, 255, 255, 255);
    inline ImVec4 text_inactive  = ImColor(160, 120, 160, 255);
    inline ImVec4 text_dim       = ImColor(100,  70, 110, 255);

    // ── Sidebar selection ───────────────────────────
    inline ImVec4 sel_active_bg  = ImColor( 40,  15,  55, 255);
    inline ImVec4 sel_inactive   = ImColor(  0,   0,   0,   0);
    inline ImVec4 sel_text_on    = ImColor(255, 105, 180, 255);
    inline ImVec4 sel_text_off   = ImColor(140, 100, 150, 255);

    // ── Checkbox ────────────────────────────────────
    inline ImVec4 chk_bg         = ImColor( 30,  10,  40, 255);
    inline ImVec4 chk_bg_active  = ImColor(255, 105, 180, 255);
    inline ImVec4 chk_stroke     = ImColor( 60,  25,  70, 255);
    inline ImVec4 chk_mark       = ImColor(255, 255, 255, 255);

    // ── Slider ──────────────────────────────────────
    inline ImVec4 slider_bg      = ImColor( 25,   8,  35, 255);
    inline ImVec4 slider_fill    = ImColor(255, 105, 180, 255);
    inline ImVec4 slider_grab    = ImColor(255, 255, 255, 255);

    // ── Button ──────────────────────────────────────
    inline ImVec4 btn_bg         = ImColor( 30,  10,  42, 255);
    inline ImVec4 btn_hov        = ImColor( 50,  20,  65, 255);
    inline ImVec4 btn_active     = ImColor(255, 105, 180, 255);
    inline ImVec4 btn_stroke     = ImColor( 60,  25,  75, 255);

    // ── Scrollbar ───────────────────────────────────
    inline ImVec4 scroll_bg      = ImColor( 15,   5,  20, 255);
    inline ImVec4 scroll_grab    = ImColor( 80,  30,  90, 255);
    inline ImVec4 scroll_hov     = ImColor(120,  50, 130, 255);

    // ── Subtab ──────────────────────────────────────
    inline ImVec4 subtab_active  = ImColor(255, 105, 180, 255);
    inline ImVec4 subtab_inactive= ImColor(140, 100, 150, 255);
    inline ImVec4 subtab_line    = ImColor(255, 105, 180, 200);
}
