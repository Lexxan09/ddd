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

// OpenGL - only link, no direct includes needed.
// imgui_impl_opengl3 uses its own internal loader for GL 2.0+ functions.
#pragma comment(lib, "opengl32.lib")

// ImGui - core
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"

// ImGui - backends (these pull in their own GL declarations)
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_opengl3.h"

// MinHook
#include "minhook/include/MinHook.h"
#pragma comment(lib, "minhook/lib/MinHook.x64.lib")
