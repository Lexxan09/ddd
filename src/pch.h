#pragma once

// Windows
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <TlHelp32.h>
#include <psapi.h>

// STL
#include <string>
#include <vector>
#include <array>
#include <map>
#include <functional>
#include <algorithm>
#include <chrono>
#include <thread>
#include <mutex>
#include <memory>
#include <sstream>
#include <fstream>

// OpenGL
#include <gl/GL.h>
#pragma comment(lib, "opengl32.lib")

// ImGui
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_win32.h"
#include "../imgui/imgui_impl_opengl3.h"
#include "../imgui/imgui_internal.h"

// MinHook
#include "../minhook/include/MinHook.h"
#pragma comment(lib, "minhook/lib/MinHook.x64.lib")
