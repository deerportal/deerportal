# Windows Troubleshooting Guide for DeerPortal

*A comprehensive guide for resolving Windows-specific issues with DeerPortal*

## 🚨 Critical Audio Error: "Failed to open sound file"

### **Problem:**
DeerPortal crashes immediately on startup with:
```
Failed to open sound file (couldn't open stream)
Provided path: assets/audio/dice.ogg
Absolute path: C:\Program Files\DeerPortal 0.9.1\assets\audio\dice.ogg
Failed to open sound buffer from file
Critical Asset Error: Failed to load dice sound effect
Failed to load: audio/dice.ogg
```

### **Root Cause:**
The Windows packaging was installing assets to `data/assets/` instead of `assets/` directory. This has been **FIXED** in the latest version. If you're still experiencing this issue, it means you have an older build.

### **Solutions (Try in Order):**

#### **Solution 1: Download Latest Version**
1. **Get the newest build** from GitHub releases (after this fix)
2. **Uninstall old version** completely
3. **Install fresh version** - assets should now be in correct location

#### **Solution 2: Manual Asset Fix (Temporary)**
1. **Navigate to DeerPortal installation directory**
2. **If you see** `data/assets/` folder but no `assets/` folder:
   ```
   DeerPortal.exe
   data/
   └── assets/  ← Assets are here (wrong location)
   ```
3. **Move the assets**:
   - Copy the entire `data/assets/` folder
   - Paste it as `assets/` in the same directory as `DeerPortal.exe`
   - Final structure should be:
   ```
   DeerPortal.exe
   assets/        ← Correct location
   ├── audio/
   │   ├── dice.ogg
   │   └── ...
   ├── img/
   └── ...
   ```

#### **Solution 3: Check Assets Folder Structure**
1. **Verify installation directory**:
   - Navigate to where DeerPortal is installed (usually `C:\Program Files\DeerPortal 0.9.1\`)
   - Ensure the `assets` folder exists in the same directory as `DeerPortal.exe`
   - Check that `assets\audio\dice.ogg` file exists
   ```
   DeerPortal.exe
   assets/
   ├── audio/
   │   ├── dice.ogg  ← This file must exist!
   │   ├── card.ogg
   │   ├── click.ogg
   │   └── ...
   ├── img/
   ├── fnt/
   └── pdf/
   ```

#### **Solution 4: Reinstall DeerPortal**
1. **Complete reinstallation**:
   - Uninstall DeerPortal from Control Panel
   - Delete any remaining DeerPortal folders
   - Download fresh copy from GitHub releases
   - Install to a path without spaces (e.g., `C:\Games\DeerPortal`)

#### **Solution 5: Install Microsoft Visual C++ Redistributables**
1. **Download and install all versions**:
   - [Visual C++ 2015-2022 Redistributable (x64)](https://aka.ms/vs/17/release/vc_redist.x64.exe)
   - [Visual C++ 2015-2022 Redistributable (x86)](https://aka.ms/vs/17/release/vc_redist.x86.exe)
2. **Restart your computer**
3. **Try running DeerPortal again**

#### **Solution 6: Audio Service Check**
1. **Press Win + R**, type `services.msc`
2. **Find "Windows Audio"** service
3. **Right-click → Properties**
4. **Set Startup type to "Automatic"**
5. **Click "Start" if not running**
6. **Restart your computer**

#### **Solution 7: Extract from Archive Properly**
1. **If using ZIP/archive**:
   - Right-click downloaded file → "Extract All"
   - Choose destination folder (avoid spaces in path)
   - Ensure "Show extracted files when complete" is checked
   - Verify all files extracted correctly

#### **Solution 8: Run as Administrator**
1. **Right-click DeerPortal.exe**
2. **Select "Run as administrator"**
3. **Allow permissions when prompted**

#### **Solution 9: Check Windows Installer Package**
1. **If using .exe installer**:
   - Verify installer completed successfully
   - Check Windows Event Viewer for installation errors
   - Try installing to different location (e.g., `C:\Games\DeerPortal`)

#### **Solution 10: DirectSound/Audio Driver Update (Secondary)**
1. **Update audio drivers**:
   - Right-click Start → Device Manager
   - Expand "Sound, video and game controllers"
   - Right-click your audio device → Update driver
2. **Install DirectX**:
   - Download DirectX End-User Runtime Web Installer from Microsoft
   - Install and restart

---

## 🔧 Common Windows Issues & Solutions

### **Issue: Missing DLL Files**

**Error Messages:**
- "MSVCP140.dll is missing"
- "VCRUNTIME140.dll is missing"
- "api-ms-win-crt-runtime-l1-1-0.dll is missing"

**Solution:**
1. Install Visual C++ Redistributables (see Solution 2 above)
2. If still failing, download the specific DLL from Microsoft or use Windows Update

### **Issue: Game Window Won't Open**

**Symptoms:**
- Process starts but no window appears
- Game appears in Task Manager but not on screen

**Solutions:**
1. **Check Multiple Monitors:**
   - Press Alt + Tab to see if window is on another monitor
   - Try Win + Shift + Arrow keys to move window
2. **Graphics Driver Update:**
   - Update your graphics card drivers (NVIDIA/AMD/Intel)
   - Restart computer after update
3. **Display Settings:**
   - Right-click desktop → Display settings
   - Ensure scaling is set to 100% or 125%
   - Try different resolution temporarily

### **Issue: Poor Performance/Low FPS**

**Solutions:**
1. **Disable Windows Game Mode:**
   - Settings → Gaming → Game Mode → Off
2. **Update Graphics Drivers:**
   - Visit manufacturer website (NVIDIA/AMD/Intel)
   - Download latest drivers
3. **Close Background Applications:**
   - Press Ctrl + Shift + Esc (Task Manager)
   - End unnecessary processes
4. **Power Settings:**
   - Control Panel → Power Options
   - Select "High performance" or "Balanced"

### **Issue: Antivirus False Positive**

**Symptoms:**
- Antivirus blocks/deletes DeerPortal.exe
- "Trojan" or "Malware" warnings

**Solutions:**
1. **Add Exception:**
   - Open your antivirus software
   - Add DeerPortal.exe to exceptions/whitelist
   - Add entire DeerPortal folder to exceptions
2. **Temporarily Disable:**
   - Disable real-time protection temporarily
   - Run DeerPortal
   - Re-enable protection after confirming it works

### **Issue: Assets Not Loading**

**Error Messages:**
- "Failed to load texture"
- "Font not found"
- "Asset path not found"

**Solutions:**
1. **Check Installation Path:**
   - Ensure `assets/` folder is in same directory as DeerPortal.exe
   - Verify folder structure:
     ```
     DeerPortal.exe
     assets/
     ├── img/
     ├── audio/
     ├── fnt/
     └── pdf/
     ```
2. **Reinstall:**
   - Delete entire DeerPortal folder
   - Download fresh copy
   - Extract to new location (avoid spaces in path)

### **Issue: Installer Problems**

**Solutions:**
1. **Run Installer as Administrator:**
   - Right-click installer → "Run as administrator"
2. **Disable Antivirus Temporarily:**
   - Some antivirus software blocks installers
3. **Check Disk Space:**
   - Ensure at least 500MB free space
4. **Try Different Location:**
   - Install to `C:\Games\DeerPortal` instead of Program Files

---

## 🎮 Gameplay Issues

### **Issue: Controls Not Responding**

**Solutions:**
1. **Check Keyboard Layout:**
   - Ensure English (US) keyboard layout is active
   - Some keys may be mapped differently
2. **Full Screen Mode:**
   - Press Alt + Enter to toggle full screen
   - Some controls work better in full screen
3. **Focus Window:**
   - Click on game window to ensure it has focus
   - Alt + Tab to switch between windows

### **Issue: Game Crashes During Play**

**Solutions:**
1. **Check System Requirements:**
   - Windows 10/11 (64-bit)
   - 4GB RAM minimum
   - DirectX 11 compatible graphics
2. **Update System:**
   - Windows Update
   - Install all available updates
3. **Memory Issues:**
   - Close other applications
   - Restart computer before playing

---

## 🔍 Diagnostic Information

### **Gathering System Information:**
1. **Press Win + R**, type `msinfo32`
2. **Note down:**
   - Windows version
   - Processor type
   - RAM amount
   - Graphics card model
3. **Check DirectX:**
   - Press Win + R, type `dxdiag`
   - Note DirectX version and any issues

### **Log Files Location:**
DeerPortal may create log files in:
- `%APPDATA%\DeerPortal\`
- Same directory as executable
- `%TEMP%\DeerPortal\`

---

## 📞 Getting Help

### **Before Reporting Issues:**
1. **Try all solutions above**
2. **Gather system information**
3. **Note exact error messages**
4. **Check if problem persists after restart**

### **Report Issues:**
- **GitHub Issues**: https://github.com/deerportal/deerportal/issues
- **Include:**
  - Windows version
  - Error messages (screenshots)
  - System specifications
  - Steps to reproduce

### **Community Support:**
- Check existing GitHub issues for similar problems
- Search for solutions in discussions
- Community members often share working configurations

---

## ⚡ Quick Fixes Checklist

**Before playing DeerPortal:**
- [ ] Install Visual C++ Redistributables
- [ ] Update graphics drivers
- [ ] Check Windows Audio service is running
- [ ] Disable antivirus temporarily if needed
- [ ] Run as administrator if necessary
- [ ] Verify assets folder is present
- [ ] Close unnecessary background applications
- [ ] Restart computer if experiencing issues

**Most common fix:** Download latest version (assets packaging fixed) or manually move `data/assets/` to `assets/` directory.

---

*This guide covers the most common Windows issues. For additional help, visit the [DeerPortal GitHub repository](https://github.com/deerportal/deerportal).* 