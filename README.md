# KatieCute - GTInternal
## Setup Guide (Baca dulu sebelum compile!)

---

## 📁 Struktur Folder yang Harus Ada

```
KatieCute/
├── KatieCute.sln           ← buka ini di Visual Studio
├── KatieCute.vcxproj
├── src/
│   ├── pch.h
│   ├── dllmain.cpp
│   ├── hooks/
│   │   ├── hook.h
│   │   └── hook.cpp
│   ├── frontend/
│   │   ├── menu.h
│   │   ├── menu.cpp
│   │   └── colors.h
│   ├── imgui/              ← HARUS kamu isi sendiri (step 2)
│   └── minhook/            ← HARUS kamu isi sendiri (step 3)
```

---

## ⚙️ STEP 1 — Install Visual Studio 2022

Download: https://visualstudio.microsoft.com/downloads/
Pilih: **Community (gratis)**

Saat install, centang workload:
- ✅ Desktop development with C++
- ✅ Windows 10/11 SDK

---

## ⚙️ STEP 2 — Download ImGui

1. Buka: https://github.com/ocornut/imgui
2. Klik **Code** → **Download ZIP**
3. Extract, copy file-file ini ke folder `src/imgui/`:
   - `imgui.h`
   - `imgui.cpp`
   - `imgui_internal.h`
   - `imgui_draw.cpp`
   - `imgui_tables.cpp`
   - `imgui_widgets.cpp`
   - `imgui_impl_win32.h`
   - `imgui_impl_win32.cpp`  (ada di folder `backends/`)
   - `imgui_impl_opengl3.h`  (ada di folder `backends/`)
   - `imgui_impl_opengl3.cpp` (ada di folder `backends/`)
   - `imconfig.h`
   - `imstb_rectpack.h`
   - `imstb_textedit.h`
   - `imstb_truetype.h`

---

## ⚙️ STEP 3 — Download MinHook

1. Buka: https://github.com/TsudaKageyu/minhook
2. Download ZIP, extract
3. Copy ke `src/minhook/`:
   - Folder `include/` (berisi `MinHook.h`)
   - Folder `lib/` (berisi `MinHook.x64.lib`)

---

## ⚙️ STEP 4 — Compile

1. Buka `KatieCute.sln` dengan Visual Studio 2022
2. Pastikan config: **Release | x64**
3. Tekan **Ctrl+Shift+B** (Build Solution)
4. DLL output ada di folder `bin/KatieCute.dll`

---

## ⚙️ STEP 5 — Inject

1. Buka Growtopia
2. Inject `bin/KatieCute.dll` menggunakan injector (Process Hacker, Extreme Injector, dll)
3. Tekan **INSERT** untuk toggle menu

---

## 🎮 Controls

| Key | Fungsi |
|-----|--------|
| INSERT | Toggle menu on/off |
| END | Unload DLL |

---

## ❓ Kalau Error Saat Compile

**Error: cannot open file 'MinHook.x64.lib'**
→ Pastikan folder `src/minhook/lib/` ada dan berisi `MinHook.x64.lib`

**Error: cannot open include file 'imgui.h'**
→ Pastikan semua file ImGui sudah di-copy ke `src/imgui/`

**Error: identifier "ImGui_ImplOpenGL3_Init" undefined**
→ Pastikan `imgui_impl_opengl3.cpp` sudah ada di `src/imgui/` dan terdaftar di .vcxproj

---

## 📝 Notes

- Menu toggle: **INSERT key**
- DLL unload: **END key**  
- Semua fitur (Mod Fly, dll) ada di `src/frontend/menu.cpp` bagian `struct Features`
- Warna bisa diubah di `src/frontend/colors.h`
- Style ImGui diubah di fungsi `Menu::ApplyStyle()` di `menu.cpp`
