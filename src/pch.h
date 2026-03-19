#pragma once

// Windows
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <TlHelp32.h>

// STL
#include <string>
#include <vector>
#include <functional>
#include <algorithm>
#include <sstream>

// OpenGL
#include <gl/GL.h>
#pragma comment(lib, "opengl32.lib")

// ImGui
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_opengl3.h"

// MinHook
#include "minhook/include/MinHook.h"
#pragma comment(lib, "minhook/lib/MinHook.x64.lib")

// Our colors
#include "frontend/colors.h"
