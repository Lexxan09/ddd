# KatieCute - Build via GitHub Actions

## How to get your DLL in 5 minutes (no installs needed)

---

### Step 1 — Create a GitHub account
Go to **https://github.com** → Sign up (free)

---

### Step 2 — Create a new repository
1. Click the **+** button top right → **New repository**
2. Name it anything, e.g. `katiecutebuild`
3. Set to **Private** ✓
4. Click **Create repository**

---

### Step 3 — Upload these files
Click **uploading an existing file** on the repo page, then upload **everything** from this ZIP maintaining the folder structure:

```
.github/
  workflows/
    build.yml          ← THE BUILD SCRIPT
src/
  pch.h
  dllmain.cpp
  hooks/
    hook.h
    hook.cpp
  frontend/
    menu.h
    menu.cpp
    colors.h
  imgui/               ← empty folder (auto-filled by build)
  minhook/             ← empty folder (auto-filled by build)
```

> **Tip:** Zip this whole folder and drag-drop it into GitHub's upload page

---

### Step 4 — Watch it build
1. Go to your repo → click **Actions** tab
2. You'll see **"Build KatieCute DLL"** running
3. Wait ~3 minutes for it to finish (green checkmark ✓)

---

### Step 5 — Download your DLL
1. Click the completed workflow run
2. Scroll down to **Artifacts**
3. Click **KatieCute-DLL** → downloads a ZIP
4. Extract it → `KatieCute.dll` inside!

---

### How to inject
1. Open Growtopia
2. Use any DLL injector (Extreme Injector, Process Hacker, etc.)
3. Inject `KatieCute.dll`
4. Press **INSERT** to toggle menu
5. Press **END** to unload

---

### If build fails
Click the failed run → click the failed step → read the error message.
Most common fix: make sure all files were uploaded with correct folder structure.
