#include <Windows.h>
#include <string>
#include <functional>
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_opengl3.h"
#include "frontend/menu.h"
#include "frontend/colors.h"

namespace Menu
{
    bool g_ShowMenu = false;
    int  g_Page     = 0;
    int  g_SubPage  = 0;

    struct Features {
        bool mod_fly=false, fast_fruits=false, can_see_ghost=false, no_name=false;
        bool no_clip=false, anti_portal=false, night_vision=false, no_block=false;
        bool draw_sign=false, draw_door=false, entrance=false;
        bool path_finder=false, dev_mode=false;
        bool minimap=false, esp=false;
        float range=20.f;
        bool show_fps=true, skip_update=false;
    } f;

    static const float W=680.f, H=470.f, SW=70.f, HH=55.f;
    static ImU32 C(ImVec4 v){ return ImGui::ColorConvertFloat4ToU32(v); }

    void ApplyStyle()
    {
        ImGuiStyle& s = ImGui::GetStyle();
        s.WindowPadding={0,0}; s.WindowRounding=8; s.WindowBorderSize=0;
        s.ChildRounding=4; s.ChildBorderSize=0;
        s.FramePadding={8,4}; s.FrameRounding=4; s.FrameBorderSize=0;
        s.ItemSpacing={8,6}; s.ScrollbarSize=4; s.ScrollbarRounding=4;
        s.GrabMinSize=8; s.GrabRounding=4; s.TabRounding=4;
        ImVec4* c=s.Colors;
        c[ImGuiCol_WindowBg]=Colors::window_bg; c[ImGuiCol_ChildBg]=Colors::child_bg;
        c[ImGuiCol_PopupBg]=ImColor(12,4,18,240); c[ImGuiCol_Border]=Colors::stroke;
        c[ImGuiCol_FrameBg]=Colors::chk_bg; c[ImGuiCol_FrameBgHovered]=Colors::btn_hov;
        c[ImGuiCol_FrameBgActive]=Colors::accent_dark;
        c[ImGuiCol_ScrollbarBg]=Colors::scroll_bg;
        c[ImGuiCol_ScrollbarGrab]=Colors::scroll_grab;
        c[ImGuiCol_ScrollbarGrabHovered]=Colors::scroll_hov;
        c[ImGuiCol_ScrollbarGrabActive]=Colors::accent;
        c[ImGuiCol_CheckMark]=Colors::chk_mark;
        c[ImGuiCol_SliderGrab]=Colors::slider_grab;
        c[ImGuiCol_SliderGrabActive]=Colors::accent;
        c[ImGuiCol_Button]=Colors::btn_bg; c[ImGuiCol_ButtonHovered]=Colors::btn_hov;
        c[ImGuiCol_ButtonActive]=Colors::accent;
        c[ImGuiCol_Header]=Colors::sel_active_bg;
        c[ImGuiCol_HeaderHovered]=Colors::btn_hov;
        c[ImGuiCol_Text]=Colors::text_active;
        c[ImGuiCol_TextDisabled]=Colors::text_inactive;
        c[ImGuiCol_Separator]=Colors::stroke;
    }

    static void Text(ImDrawList* d, ImVec2 p, ImU32 col, const char* s){
        d->AddText(ImGui::GetFont(), ImGui::GetFontSize(), p, col, s);
    }

    bool KcCheckbox(const char* label, bool* v)
    {
        ImDrawList* d=ImGui::GetWindowDrawList();
        ImVec2 p=ImGui::GetCursorScreenPos(); float sz=16.f;
        ImGui::InvisibleButton(label,ImVec2(ImGui::GetContentRegionAvail().x,sz+4));
        bool hov=ImGui::IsItemHovered(), clk=ImGui::IsItemClicked();
        if(clk)*v=!*v;
        d->AddRectFilled(p+ImVec2(0,2),p+ImVec2(sz,sz+2),*v?C(Colors::accent):C(Colors::chk_bg),3);
        d->AddRect(p+ImVec2(0,2),p+ImVec2(sz,sz+2),hov?C(Colors::accent_dim):C(Colors::chk_stroke),3);
        if(*v){
            d->AddLine(p+ImVec2(3,9),p+ImVec2(6,12),IM_COL32(255,255,255,255),2);
            d->AddLine(p+ImVec2(6,12),p+ImVec2(13,5),IM_COL32(255,255,255,255),2);
        }
        Text(d,p+ImVec2(sz+8,2),hov?C(Colors::text_active):C(Colors::text_inactive),label);
        return clk;
    }

    bool KcButton(const char* label, ImVec2 size)
    {
        if(!size.x)size.x=ImGui::GetContentRegionAvail().x;
        if(!size.y)size.y=24;
        ImDrawList* d=ImGui::GetWindowDrawList();
        ImVec2 p=ImGui::GetCursorScreenPos();
        ImGui::InvisibleButton(label,size);
        bool hov=ImGui::IsItemHovered(),clk=ImGui::IsItemClicked();
        d->AddRectFilled(p,p+size,hov?C(Colors::btn_hov):C(Colors::btn_bg),4);
        d->AddRect(p,p+size,hov?C(Colors::accent_dim):C(Colors::btn_stroke),4);
        ImVec2 ts=ImGui::CalcTextSize(label);
        Text(d,p+(size-ts)*0.5f,C(Colors::text_active),label);
        return clk;
    }

    bool KcSliderFloat(const char* label, float* v, float mn, float mx)
    {
        ImGui::PushStyleColor(ImGuiCol_FrameBg,Colors::slider_bg);
        ImGui::PushStyleColor(ImGuiCol_SliderGrab,Colors::accent);
        ImGui::PushStyleColor(ImGuiCol_Text,Colors::text_inactive);
        bool r=ImGui::SliderFloat(label,v,mn,mx,"%.1f");
        ImGui::PopStyleColor(3); return r;
    }

    void KcGroupBox(const char* label, ImVec2 sz, std::function<void()> fn)
    {
        ImDrawList* d=ImGui::GetWindowDrawList();
        ImVec2 p=ImGui::GetCursorScreenPos();
        d->AddRect(p,p+sz,C(Colors::stroke),4);
        ImVec2 ts=ImGui::CalcTextSize(label);
        d->AddRectFilled(p+ImVec2(8,-1),p+ImVec2(10+ts.x,1),C(Colors::child_bg));
        Text(d,p+ImVec2(10,-7),C(Colors::child_title),label);
        ImGui::SetCursorScreenPos(p+ImVec2(0,12));
        ImGui::BeginChild(label,sz-ImVec2(0,12),false,ImGuiWindowFlags_NoScrollbar);
        ImGui::SetCursorPos({10,8}); fn();
        ImGui::EndChild();
    }

    bool KcSubtab(const char* label, int idx, int& cur)
    {
        ImDrawList* d=ImGui::GetWindowDrawList();
        ImVec2 p=ImGui::GetCursorScreenPos();
        float w=ImGui::CalcTextSize(label).x+20,h=22;
        ImGui::InvisibleButton(label,{w,h});
        bool hov=ImGui::IsItemHovered(),clk=ImGui::IsItemClicked();
        if(clk)cur=idx; bool act=(cur==idx);
        ImU32 tc=act?C(Colors::subtab_active):hov?C(Colors::text_active):C(Colors::subtab_inactive);
        Text(d,p+ImVec2(10,3),tc,label);
        if(act)d->AddLine(p+ImVec2(10,h-1),p+ImVec2(w-10,h-1),C(Colors::subtab_line),2);
        ImGui::SameLine(); return clk;
    }

    void RenderSidebar(ImDrawList* d, ImVec2 p)
    {
        d->AddRectFilled(p,p+ImVec2(SW,H),C(Colors::sidebar_bg),8,ImDrawFlags_RoundCornersLeft);
        d->AddLine(p+ImVec2(SW,0),p+ImVec2(SW,H),C(Colors::stroke));
        d->AddCircleFilled(p+ImVec2(35,30),18,C(Colors::accent_dark),60);
        d->AddCircle(p+ImVec2(35,30),19,C(Colors::accent),60,1.5f);
        Text(d,p+ImVec2(24,23),C(Colors::text_active),"KC");
        d->AddLine(p+ImVec2(15,176),p+ImVec2(55,176),IM_COL32(255,255,255,15));
        d->AddLine(p+ImVec2(15,294),p+ImVec2(55,294),IM_COL32(255,255,255,15));
        const char* icons[]={"Main","Map","</>","Hook","Set"};
        for(int i=0;i<5;i++){
            float y=58.f+i*72.f; bool act=(g_Page==i);
            ImVec2 tp=p+ImVec2(0,y), ts={SW,60};
            if(act){
                d->AddRectFilled(tp,tp+ts,C(Colors::sel_active_bg));
                d->AddRectFilled(tp,tp+ImVec2(3,60),C(Colors::accent));
            }
            ImVec2 is=ImGui::CalcTextSize(icons[i]);
            Text(d,tp+(ts-is)*0.5f,act?C(Colors::accent):C(Colors::sel_text_off),icons[i]);
            ImGui::SetCursorScreenPos(tp);
            std::string id="##t"+std::to_string(i);
            ImGui::InvisibleButton(id.c_str(),ts);
            if(ImGui::IsItemClicked()){g_Page=i;g_SubPage=0;}
        }
    }

    void RenderHeader(ImDrawList* d, ImVec2 p)
    {
        d->AddRectFilled(p+ImVec2(SW,0),p+ImVec2(W,HH),C(Colors::header_bg),8,ImDrawFlags_RoundCornersTopRight);
        d->AddLine(p+ImVec2(SW,HH),p+ImVec2(W,HH),C(Colors::stroke));
        ImGui::SetCursorScreenPos(p+ImVec2(SW+12,16));
        if(g_Page==0){KcSubtab("Patches",0,g_SubPage);KcSubtab("Cheats",1,g_SubPage);}
        else if(g_Page==1){KcSubtab("Minimap",0,g_SubPage);}
        else if(g_Page==2){KcSubtab("Scripts",0,g_SubPage);KcSubtab("Logs",1,g_SubPage);}
        else if(g_Page==4){KcSubtab("General",0,g_SubPage);KcSubtab("Debug",1,g_SubPage);}
        char tb[16]; SYSTEMTIME st; GetLocalTime(&st);
        sprintf_s(tb,"%02d:%02d",st.wHour,st.wMinute);
        ImVec2 ts=ImGui::CalcTextSize(tb);
        Text(d,p+ImVec2(W-ts.x-12,18),C(Colors::accent),tb);
    }

    void RenderPatches(ImVec2 o)
    {
        ImGui::SetCursorScreenPos(o);
        ImGui::BeginChild("##pat",{W-SW-20,H-HH-10},false,ImGuiWindowFlags_NoScrollbar);
        float cw=(ImGui::GetContentRegionAvail().x-20)*.5f;
        ImGui::BeginChild("##pl",{cw,0});
        KcGroupBox("Patches",{cw-10,210},[&]{
            KcCheckbox("Mod Fly",&f.mod_fly);ImGui::Spacing();
            KcCheckbox("Fast Fruits",&f.fast_fruits);ImGui::Spacing();
            KcCheckbox("Can See Ghost",&f.can_see_ghost);ImGui::Spacing();
            KcCheckbox("No Name",&f.no_name);
        });
        ImGui::EndChild();ImGui::SameLine(0,20);
        ImGui::BeginChild("##pr",{cw,0});
        KcGroupBox("More Patches",{cw-10,210},[&]{
            KcCheckbox("No Clip",&f.no_clip);ImGui::Spacing();
            KcCheckbox("Anti Portal",&f.anti_portal);ImGui::Spacing();
            KcCheckbox("Night Vision",&f.night_vision);ImGui::Spacing();
            KcCheckbox("No Block",&f.no_block);
        });
        ImGui::EndChild();ImGui::EndChild();
    }

    void RenderCheats(ImVec2 o)
    {
        ImGui::SetCursorScreenPos(o);
        ImGui::BeginChild("##che",{W-SW-20,H-HH-10},false,ImGuiWindowFlags_NoScrollbar);
        float cw=(ImGui::GetContentRegionAvail().x-20)*.5f;
        ImGui::BeginChild("##cl",{cw,0});
        KcGroupBox("Cheats",{cw-10,200},[&]{
            KcCheckbox("Draw Sign Label",&f.draw_sign);ImGui::Spacing();
            KcCheckbox("Draw Door Label",&f.draw_door);ImGui::Spacing();
            KcCheckbox("Entrance Check",&f.entrance);
        });
        ImGui::EndChild();ImGui::SameLine(0,20);
        ImGui::BeginChild("##cr",{cw,0});
        KcGroupBox("More Cheats",{cw-10,200},[&]{
            KcCheckbox("Path Finder",&f.path_finder);ImGui::Spacing();
            KcCheckbox("Dev Mode",&f.dev_mode);
        });
        ImGui::EndChild();ImGui::EndChild();
    }

    void RenderMinimap(ImVec2 o)
    {
        ImGui::SetCursorScreenPos(o);
        ImGui::BeginChild("##mm",{W-SW-20,H-HH-10});
        float cw=(ImGui::GetContentRegionAvail().x-20)*.5f;
        ImGui::BeginChild("##ml",{cw,0});
        KcGroupBox("Minimap",{cw-10,180},[&]{
            KcCheckbox("Render Minimap",&f.minimap);ImGui::Spacing();
            KcCheckbox("Render ESP",&f.esp);ImGui::Spacing();
            KcSliderFloat("Range",&f.range,5,50);
        });
        ImGui::EndChild();ImGui::EndChild();
    }

    void RenderSettings(ImVec2 o)
    {
        ImGui::SetCursorScreenPos(o);
        ImGui::BeginChild("##set",{W-SW-20,H-HH-10});
        float cw=(ImGui::GetContentRegionAvail().x-20)*.5f;
        ImGui::BeginChild("##sl",{cw,0});
        KcGroupBox("General",{cw-10,160},[&]{
            KcCheckbox("Show FPS",&f.show_fps);ImGui::Spacing();
            KcCheckbox("Skip Update",&f.skip_update);
        });
        ImGui::EndChild();ImGui::EndChild();
    }

    void RenderScript(ImVec2 o)
    {
        ImGui::SetCursorScreenPos(o);
        ImGui::BeginChild("##scr",{W-SW-20,H-HH-10});
        ImGui::TextColored(Colors::text_inactive,"Use in-game Script tab for Lua execution");
        ImGui::EndChild();
    }

    void Render()
    {
        if(!g_ShowMenu)return;
        ImGui::SetNextWindowSize({W,H},ImGuiCond_Always);
        ImGui::SetNextWindowBgAlpha(0);
        ImGui::Begin("##kc",nullptr,
            ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoResize|
            ImGuiWindowFlags_NoScrollbar|ImGuiWindowFlags_NoScrollWithMouse|
            ImGuiWindowFlags_NoBringToDisplayOnFocus);
        ImDrawList* d=ImGui::GetWindowDrawList();
        ImVec2 p=ImGui::GetWindowPos();
        d->AddRectFilled(p+ImVec2(SW,HH),p+ImVec2(W,H),C(Colors::window_bg),8,ImDrawFlags_RoundCornersBottomRight);
        for(int i=4;i>=1;i--){float o=(float)(i*3);d->AddRect(p-ImVec2(o,o),p+ImVec2(W+o,H+o),IM_COL32(0,0,0,30*i),10);}
        RenderSidebar(d,p);
        RenderHeader(d,p);
        ImVec2 co=p+ImVec2(SW+10,HH+8);
        switch(g_Page){
            case 0: g_SubPage==0?RenderPatches(co):RenderCheats(co); break;
            case 1: RenderMinimap(co); break;
            case 2: RenderScript(co); break;
            case 4: RenderSettings(co); break;
            default: ImGui::SetCursorScreenPos(co); ImGui::TextColored(Colors::text_inactive,"Coming soon..."); break;
        }
        ImGui::End();
    }
}
