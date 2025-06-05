# DeerPortal Visual Studio 2019/2022 Setup Guide

## Overview

Visual Studio provides the most integrated Windows development experience for DeerPortal with excellent CMake support, IntelliSense, and debugging capabilities.

**Recommended for:**
- Windows developers new to DeerPortal
- Teams preferring IDE-based development
- Projects requiring advanced debugging features

## Prerequisites Installation

### 1. Visual Studio Installation

#### Download Visual Studio
- **Visual Studio 2022 Community** (Free): https://visualstudio.microsoft.com/vs/community/
- **Visual Studio 2019** (Alternative): Still supported but 2022 recommended

#### Required Workloads
During installation, select:
- ✅ **"Desktop development with C++"**
- ✅ **"Game development with C++"** (optional, includes additional tools)

#### Individual Components (Verify These Are Selected)
- ✅ **Windows 10/11 SDK** (latest version)
- ✅ **CMake tools for Visual Studio**
- ✅ **Git for Windows** (if not already installed)
- ✅ **IntelliCode**

### 2. Git for Windows (If Not Installed)
```cmd
# Download from: https://git-scm.com/download/win
# Or use winget:
winget install Git.Git
```

### 3. CMake Installation
Visual Studio includes CMake, but you can also install standalone:
```cmd
# Via winget (recommended)
winget install Kitware.CMake

# Or download from: https://cmake.org/download/
```

## SFML 3.0 Installation Methods

### Method 1: vcpkg (Recommended)

#### Install vcpkg
```cmd
# Clone vcpkg to C:\ for system-wide use
cd C:\
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
.\bootstrap-vcpkg.bat

# Integrate with Visual Studio
.\vcpkg integrate install
```

#### Install SFML 3.0
```cmd
# Install SFML for x64 (recommended)
.\vcpkg install sfml:x64-windows

# Optional: Install for x86 as well
.\vcpkg install sfml:x86-windows

# Verify installation
.\vcpkg list sfml
```

### Method 2: Pre-compiled Binaries (Alternative)

#### Download SFML 3.0
1. Visit: https://www.sfml-dev.org/download.php
2. Download **SFML 3.0.0 - Visual C++ 15 (2017) - 64-bit**
3. Extract to `C:\SFML-3.0.0\`

#### Environment Variables
```cmd
# Add to system PATH (optional)
set SFML_ROOT=C:\SFML-3.0.0
```

## DeerPortal Build Process

### 1. Clone Repository
```cmd
# Using Git Bash or Command Prompt
git clone https://github.com/deerportal/deerportal.git
cd deerportal
```

### 2. Visual Studio CMake Integration

#### Option A: Visual Studio GUI
1. **Open Visual Studio 2022**
2. **File** → **Open** → **CMake...**
3. Select `CMakeLists.txt` in the deerportal directory
4. Visual Studio will automatically configure the project

#### Option B: Command Line Setup
```cmd
# Create build directory
mkdir build
cd build

# Configure with vcpkg (if using vcpkg)
cmake -G "Visual Studio 17 2022" -A x64 ^
      -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake ^
      -DCMAKE_BUILD_TYPE=Release ..

# Configure with manual SFML (if using pre-compiled)
cmake -G "Visual Studio 17 2022" -A x64 ^
      -DSFML_DIR="C:/SFML-3.0.0/lib/cmake/SFML" ^
      -DCMAKE_BUILD_TYPE=Release ..
```

### 3. Build Configuration

#### CMake Generation Messages
Look for these success messages:
```
-- SFML 3.0 found successfully
-- Configuring done
-- Generating done
-- Build files have been written to: .../build
```

#### Build the Project
```cmd
# Command line build
cmake --build . --config Release

# Or use Visual Studio:
# Build → Build Solution (Ctrl+Shift+B)
```

## Visual Studio Development Workflow

### 1. Project Configuration

#### CMake Targets in Solution Explorer
- **DeerPortal** - Main executable target
- **ALL_BUILD** - Builds entire project
- **INSTALL** - Installation target
- **ZERO_CHECK** - CMake regeneration

#### Build Configurations
- **Debug**: Full debugging, unoptimized
- **Release**: Optimized for performance
- **RelWithDebInfo**: Optimized with debug symbols

### 2. Debugging Setup

#### Launch Configuration
Visual Studio should auto-detect the executable. If not:

**File** → **Add** → **Existing Project** → Select `DeerPortal.exe`

#### Debug Settings
1. **Right-click DeerPortal project** → **Properties**
2. **Debugging** → **Working Directory**: Set to `$(OutDir)`
3. **Environment**: Verify `PATH` includes SFML DLLs if using dynamic linking

#### Breakpoint Debugging
- Set breakpoints in source files
- Use **F5** to start debugging
- **F10** (Step Over), **F11** (Step Into)

### 3. IntelliSense Configuration

#### Include Paths
Visual Studio should automatically detect SFML headers through CMake. If IntelliSense shows errors:

1. **Tools** → **Options** → **Text Editor** → **C/C++** → **Advanced**
2. **IntelliSense Engine**: Set to "Default"
3. **Rescan Solution**: **Project** → **Rescan Solution**

## Advanced Visual Studio Features

### 1. CMake Integration

#### CMakeSettings.json
Visual Studio generates this automatically, but you can customize:
```json
{
  "configurations": [
    {
      "name": "x64-Release",
      "generator": "Visual Studio 17 2022 Win64",
      "configurationType": "Release",
      "buildRoot": "${projectDir}\\build\\${name}",
      "installRoot": "${projectDir}\\install\\${name}",
      "cmakeCommandArgs": "",
      "buildCommandArgs": "",
      "ctestCommandArgs": "",
      "variables": [
        {
          "name": "CMAKE_TOOLCHAIN_FILE",
          "value": "C:/vcpkg/scripts/buildsystems/vcpkg.cmake"
        }
      ]
    }
  ]
}
```

### 2. Package Management

#### vcpkg Integration Benefits
- Automatic library detection
- No manual PATH configuration
- Consistent builds across machines
- Easy dependency updates

#### Update SFML
```cmd
# Update vcpkg and packages
cd C:\vcpkg
git pull
.\vcpkg upgrade sfml
```

### 3. Performance Profiling

#### Visual Studio Diagnostic Tools
- **Debug** → **Windows** → **Show Diagnostic Tools**
- Monitor CPU usage, memory allocation
- Analyze frame rate and performance bottlenecks

## Packaging & Distribution

### 1. NSIS Installer

#### Prerequisites
```cmd
# Install NSIS
winget install NSIS.NSIS

# Or download from: https://nsis.sourceforge.io/
```

#### Generate Installer
```cmd
# After successful Release build
cd build
cpack -G NSIS -C Release

# Output: DeerPortal-0.9.0-Windows.exe
```

### 2. Standalone Distribution

#### Copy Dependencies
Visual Studio Command Prompt:
```cmd
# Create distribution folder
mkdir DeerPortal-Distribution
cd DeerPortal-Distribution

# Copy executable
copy ..\Release\DeerPortal.exe .

# Copy assets (handled by CMake)
xcopy ..\assets assets\ /E /I

# Copy SFML DLLs (if dynamic linking)
copy "C:\vcpkg\installed\x64-windows\bin\sfml-*.dll" .
```

## Troubleshooting

### Common Issues

#### "SFML not found"
```cmd
# Verify vcpkg installation
C:\vcpkg\vcpkg list sfml

# If empty, reinstall
C:\vcpkg\vcpkg install sfml:x64-windows
```

#### CMake Generation Failed
```cmd
# Clear CMake cache
cd build
del CMakeCache.txt
del -r CMakeFiles/

# Regenerate
cmake -G "Visual Studio 17 2022" -A x64 ^
      -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake ..
```

#### Build Errors
1. **Clean Solution**: **Build** → **Clean Solution**
2. **Rebuild**: **Build** → **Rebuild Solution**
3. **Check Output Window**: **View** → **Output** → **Build**

#### IntelliSense Issues
1. **Delete** `.vs` folder in project root
2. **Reopen** Visual Studio
3. **Let CMake** regenerate configuration

### Performance Issues

#### Slow Builds
- **Use** `/MP` flag for parallel compilation (auto-enabled in CMake)
- **Close** unnecessary programs during build
- **Use** SSD storage for build directory

#### Runtime Performance
- **Build Release** configuration for gaming
- **Enable LTO**: Already configured in CMakeLists.txt
- **Profile** with Visual Studio Diagnostic Tools

## Visual Studio Extensions (Optional)

### Recommended Extensions
- **CMake Tools** - Enhanced CMake support
- **GitHub Extension** - Git integration
- **SonarLint** - Code quality analysis
- **Visual Studio IntelliCode** - AI-assisted development

### Installation
**Extensions** → **Manage Extensions** → Search and install

## Next Steps

1. **Build successfully** in Release mode
2. **Test DeerPortal** executable
3. **Set up debugging** environment
4. **Create installer** with NSIS
5. **Explore** Visual Studio debugging features

---

**Visual Studio Versions**: 2019, 2022  
**SFML Version**: 3.0.x  
**vcpkg Integration**: Automatic  
**Debugging Support**: Full IntelliSense and breakpoints 