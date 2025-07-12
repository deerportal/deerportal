# CI/CD Troubleshooting Guide

## 🚨 **Common CI Issues & Solutions**

### **SFML 3.0 Not Found in Linux CI**

**Problem**: 
```
CMake Error: Could not find a package configuration file provided by "SFML" (requested version 3)
```

**Root Cause**: GitHub Actions Ubuntu runners only have older SFML versions (2.x) or no SFML at all.

**Solution**: Install SFML 3.0 from source in CI workflow.

#### **Fixed GitHub Actions Workflow**
```yaml
- name: Install SFML 3.0 Dependencies
  run: |
    sudo apt update
    sudo apt install -y \
      build-essential \
      cmake \
      git \
      libfreetype6-dev \
      libopenal-dev \
      libvorbis-dev \
      libflac-dev \
      libudev-dev \
      libgl1-mesa-dev \
      libxrandr-dev \
      libxcursor-dev \
      libxi-dev \
      libx11-dev

- name: Build and Install SFML 3.0.1 from Source
  run: |
    git clone --depth 1 --branch 3.0.1 https://github.com/SFML/SFML.git sfml-source
    cd sfml-source
    cmake -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr/local
    cmake --build build -j$(nproc)
    sudo cmake --install build
    cd ..
    rm -rf sfml-source
```

### **Windows Shell Syntax Errors**

**Problem**:
```
The syntax of the command is incorrect.
Error: Process completed with exit code 255.
```

**Root Cause**: Mixed CMD and Bash syntax in Windows packaging steps.

**Solution**: Use consistent bash shell across all platforms:
```yaml
# CORRECT: Use bash shell
- name: Create Windows package
  shell: bash  # ← Key fix
  run: |
    cpack -G NSIS --verbose || {
      echo "NSIS packaging failed"
      cpack -G ZIP --verbose
    }

# INCORRECT: CMD shell with bash syntax
- name: Create Windows package  
  shell: cmd   # ← Causes syntax errors
  run: |
    cpack -G NSIS --verbose || (
      echo NSIS packaging failed &&
      cpack -G ZIP --verbose
    )
```

### **CMake Deprecation Warnings**

**Problem**:
```
CMake Deprecation Warning: Compatibility with CMake < 3.10 will be removed
```

**Solution**: Update minimum CMake version in `CMakeLists.txt`:
```cmake
# OLD: cmake_minimum_required(VERSION 3.5)
cmake_minimum_required(VERSION 3.16)
```

### **Missing Dependencies in CI**

**Common Issues & Solutions**:

| Missing Package | Error Symptom | Ubuntu Install Command |
|----------------|---------------|------------------------|
| OpenGL | `fatal error: GL/gl.h` | `libgl1-mesa-dev` |
| X11 | `fatal error: X11/Xlib.h` | `libx11-dev libxrandr-dev` |
| Audio | SFML audio linking errors | `libopenal-dev libvorbis-dev` |
| Fonts | Font rendering issues | `libfreetype6-dev` |

### **Checkout Action Issues (Fixed July 2025)**

**Problem**: 
```
Error: actions/checkout@v4 manifest parsing errors
The user doesn't want to proceed with this tool use
```

**Root Cause**: Shallow checkout causing git history issues.

**Solution**: Add `fetch-depth: 0` to checkout actions:
```yaml
- name: Checkout code
  uses: actions/checkout@v4
  with:
    fetch-depth: 0  # ← Critical fix
```

### **Release Creation Conflicts (Fixed July 2025)**

**Problem**: 
```
Error: Release already exists
```

**Root Cause**: Attempting to create duplicate GitHub releases.

**Solution**: Add release existence check:
```yaml
- name: Check if release exists
  id: check_release
  run: |
    if gh release view ${{ github.ref_name }} >/dev/null 2>&1; then
      echo "exists=true" >> $GITHUB_OUTPUT
    else
      echo "exists=false" >> $GITHUB_OUTPUT
    fi
  env:
    GITHUB_TOKEN: ${{ secrets.GITHUB_TOKEN }}

- name: Upload to Release
  if: steps.check_release.outputs.exists == 'false'
  uses: softprops/action-gh-release@v2
```

### **Permissions Issues (Fixed July 2025)**

**Problem**: 
```
Error: Resource not accessible by integration
```

**Root Cause**: Missing workflow permissions for GitHub API.

**Solution**: Add permissions block to workflows:
```yaml
permissions:
  contents: write    # For creating releases
  actions: read     # For workflow access
  packages: write   # For package uploads (if needed)
```

### **Performance Optimization**

**Speed up CI builds**:
```yaml
# Use cached dependencies
- name: Cache SFML
  uses: actions/cache@v4
  with:
    path: /usr/local
    key: sfml-3.0.1-${{ runner.os }}

# Parallel builds
cmake --build build -j$(nproc)
```

## 🔧 **Platform-Specific Issues**

### **Platform-Specific CI Differences**

| Aspect | macOS (Apple Silicon) | Linux (Ubuntu CI) | Windows (Git Bash) |
|--------|----------------------|-------------------|---------------------|
| SFML Install | Build from source | Build from source | Build from source |
| Dependencies | Homebrew + Xcode tools | Manual apt packages | Chocolatey + MSVC |
| Shell | bash | bash | bash (unified) |
| Bundle | App bundle creation | Standard binary | NSIS installer |
| Asset Path | Bundle resources | Install prefix paths | Relative paths |

### **Development vs CI Environment**

**Local Development (macOS)**:
- SFML 3.0 installed via Homebrew ✅
- All dependencies available ✅
- Native development tools ✅

**CI Environment (Linux)**:
- Minimal Ubuntu installation ❌
- No SFML installed by default ❌
- Need to install everything ❌

## 📋 **CI Debugging Checklist**

### **When CI Fails**:
1. **Check Error Type**:
   - [ ] Missing dependency packages
   - [ ] Version mismatch (e.g., SFML 2 vs 3)
   - [ ] CMake configuration issues
   - [ ] Compiler/linker errors

2. **Common Fixes**:
   - [ ] Add missing `apt install` packages
   - [ ] Update library versions to match local dev
   - [ ] Verify CMake find_package() calls
   - [ ] Check include paths and library linking

3. **Testing Strategy**:
   - [ ] Test CI changes in a fork first
   - [ ] Use Ubuntu Docker locally to reproduce
   - [ ] Compare working local vs failing CI

## 🚀 **Best Practices**

### **Future-Proof CI**:
1. **Pin Dependency Versions**: Use specific SFML tags (3.0.0)
2. **Cache Dependencies**: Speed up builds, reduce bandwidth
3. **Matrix Builds**: Test multiple platforms/compilers
4. **Documentation**: Document CI requirements

### **Example Multi-Platform CI**:
```yaml
strategy:
  matrix:
    os: [ubuntu-latest, windows-latest, macos-latest]
    
runs-on: ${{ matrix.os }}

steps:
- name: Install SFML (Ubuntu)
  if: matrix.os == 'ubuntu-latest'
  run: # Build from source

- name: Install SFML (macOS)  
  if: matrix.os == 'macos-latest'
  run: brew install sfml

- name: Install SFML (Windows)
  if: matrix.os == 'windows-latest'
  run: # vcpkg or pre-built binaries
```

## 📖 **Reference Links**

- [SFML 3.0 Migration Guide](https://www.sfml-dev.org/tutorials/3.0/getting-started/migrate/)
- [GitHub Actions Ubuntu Packages](https://github.com/actions/runner-images/blob/main/images/linux/Ubuntu2204-Readme.md)
- [CMake find_package Documentation](https://cmake.org/cmake/help/latest/command/find_package.html)

---

**Key Takeaway**: Always test CI configuration changes in a separate branch or fork before merging to main. CI environments are minimal and require explicit dependency installation. 