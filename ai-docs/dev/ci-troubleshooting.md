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

- name: Build and Install SFML 3.0 from Source
  run: |
    git clone --depth 1 --branch 3.0.0 https://github.com/SFML/SFML.git sfml-source
    cd sfml-source
    cmake -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr/local
    cmake --build build -j$(nproc)
    sudo cmake --install build
    cd ..
    rm -rf sfml-source
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

### **Performance Optimization**

**Speed up CI builds**:
```yaml
# Use cached dependencies
- name: Cache SFML
  uses: actions/cache@v3
  with:
    path: /usr/local
    key: sfml-3.0.0-${{ runner.os }}

# Parallel builds
cmake --build build -j$(nproc)
```

## 🔧 **Platform-Specific Issues**

### **macOS vs Linux Differences**

| Aspect | macOS (Homebrew) | Linux (Ubuntu CI) |
|--------|------------------|-------------------|
| SFML Install | `brew install sfml` | Build from source |
| Dependencies | Auto-resolved | Manual apt packages |
| Bundle | App bundle creation | Standard binary |
| Asset Path | Bundle resources | Install prefix paths |

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