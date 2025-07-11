# DeerPortal Development Guide

## Version Management System

DeerPortal uses a comprehensive version management system with both automated and manual components to ensure consistent versioning across all platforms and distribution channels.

### Single Source of Truth

**File**: `version.txt`  
**Format**: Semantic versioning (e.g., `0.9.1`, `0.9.1-pre.1`, `0.9.1-rc.1`)  
**Purpose**: Primary version source for all automated systems

### Automated Version Distribution

The CMake build system automatically reads from `version.txt` and distributes the version to:

```cmake
# CMakeLists.txt lines 5-13: Version parsing
file (STRINGS "version.txt" DEERPORTAL_VERSION)
string(REGEX MATCH "^([0-9]+)" DEERPORTAL_VERSION_MAJOR ${DEERPORTAL_VERSION})
string(REGEX REPLACE "^([0-9]+)\\.([0-9]+)\\.([0-9]+).*" "\\2" DEERPORTAL_VERSION_MINOR ${DEERPORTAL_VERSION})
string(REGEX REPLACE "^([0-9]+)\\.([0-9]+)\\.([0-9]+.*)" "\\3" DEERPORTAL_VERSION_PATCH ${DEERPORTAL_VERSION})

# Runtime version definition (line 47)
add_definitions(-DDEERPORTAL_VERSION="${DEERPORTAL_VERSION}.${CMAKE_BUILD_TYPE}")

# CPack packaging (lines 394-398)
set(CPACK_PACKAGE_VERSION "${DEERPORTAL_VERSION}")
```

**Automatically Updated:**
- Runtime version display (`src/game.cpp:693`)
- CMake build metadata
- CPack package names and metadata
- GitHub Actions artifact naming
- DMG/installer package names

### Manual Version Updates Required

**⚠️ CRITICAL: These files must be manually updated for each release:**

#### 1. `snapcraft.yaml` (Line 3)
```yaml
version: "0.9.1-pre.1"
```

#### 2. `debian/changelog` (Top entry)
```
deerportal (0.9.1-pre.1-test-release-ubuntu1) trusty; urgency=medium

  * Release description
  * Key changes and improvements

 -- Rafal Zawadzki <bluszcz@gmail.com>  Thu, 11 Jul 2025 10:00:00 +0200
```

#### 3. `net.devcarpet.deerportal.appdata.xml` (Line 35)
```xml
<release version="0.9.1-pre.1" date="2025-07-11">
  <description>
    <p>Release description.</p>
  </description>
</release>
```

#### 4. `README.md` (Line 10) - Optional for releases
```markdown
**Current Version: 0.9.1-pre.1 "Pre-Release Testing" (July 2025)**
```

### Version Consistency Validation

Before any release, verify version consistency:

```bash
# Check version.txt
cat version.txt

# Verify CMake parsing
grep -n "DEERPORTAL_VERSION" CMakeLists.txt

# Check manual update files
grep -n "version.*0\.9\.1" snapcraft.yaml
head -1 debian/changelog
grep -n "release version" net.devcarpet.deerportal.appdata.xml
grep -n "Current Version" README.md
```

### Release Process Workflow

#### 1. Pre-Release Testing
```bash
# Update to pre-release version
echo "0.9.1-pre.1" > version.txt

# Update manual files (see sections above)
# - snapcraft.yaml
# - debian/changelog  
# - net.devcarpet.deerportal.appdata.xml

# Test build locally
cmake -DCMAKE_BUILD_TYPE=Release .
make

# Test GitHub Actions via test-release workflow
# (Manual dispatch on GitHub)
```

#### 2. Production Release
```bash
# Update to final version
echo "0.9.1" > version.txt

# Update manual files to final version
# Create git tag
git tag v0.9.1
git push origin v0.9.1

# Triggers production release workflow
```

#### 3. Post-Release
```bash
# Update to next development version
echo "0.9.2-dev" > version.txt
```

### Version Naming Conventions

- **Stable Release**: `0.9.1`
- **Pre-release**: `0.9.1-pre.1`, `0.9.1-pre.2`
- **Release Candidate**: `0.9.1-rc.1`, `0.9.1-rc.2`
- **Alpha**: `0.9.1-alpha.1`
- **Beta**: `0.9.1-beta.1`
- **Development**: `0.9.2-dev`

### Runtime Version Display

The game displays version information at runtime:
- **Format**: `version: {VERSION}.{BUILD_TYPE}-{BASE_PATH}`
- **Example**: `version: 0.9.1-pre.1.Release-/usr/local/share/deerportal`
- **Location**: Main menu bottom-right corner
- **Source**: `src/game.cpp:693`

### Troubleshooting Version Issues

#### CMake Version Parsing Errors
```bash
# Test version parsing manually
VERSION=$(cat version.txt)
echo "Full version: $VERSION"
echo "Major: $(echo $VERSION | cut -d. -f1)"
echo "Minor: $(echo $VERSION | cut -d. -f2)"
echo "Patch: $(echo $VERSION | cut -d. -f3 | cut -d- -f1)"
```

#### Inconsistent Versions Across Files
```bash
# Quick consistency check
echo "version.txt: $(cat version.txt)"
echo "snapcraft.yaml: $(grep 'version:' snapcraft.yaml)"
echo "debian/changelog: $(head -1 debian/changelog | cut -d'(' -f2 | cut -d')' -f1)"
echo "appdata.xml: $(grep 'release version' net.devcarpet.deerportal.appdata.xml | head -1)"
```

#### Runtime Version Not Updating
- Ensure clean rebuild: `make clean && make`
- Check CMake cache: `rm CMakeCache.txt && cmake .`
- Verify DEERPORTAL_VERSION definition in compile output

## Build System

### CMake Configuration
```bash
# Release build (recommended)
cmake -DCMAKE_BUILD_TYPE=Release .
make

# Debug build with FPS counter
cmake -DCMAKE_BUILD_TYPE=Debug -DSHOW_FPS_COUNTER=ON .
make

# Create platform packages
make package         # Cross-platform packages
make dmg            # macOS DMG installer
make install        # Install to system
```

### Dependencies
- **SFML 3.0.1** (automatically built from source in CI)
- **CMake 3.16+**
- **C++17 compatible compiler**

### Platform-Specific Notes

#### macOS
```bash
# Install dependencies
brew install cmake

# Build with app bundle
cmake -DCMAKE_BUILD_TYPE=Release .
make
make dmg  # Creates DMG installer
```

#### Linux
```bash
# Install dependencies
sudo apt install build-essential cmake git libfreetype6-dev libopenal-dev \
  libvorbis-dev libflac-dev libudev-dev libgl1-mesa-dev libxrandr-dev \
  libxcursor-dev libxi-dev libx11-dev pkg-config

# Build
cmake -DCMAKE_BUILD_TYPE=Release .
make
make package  # Creates DEB and TGZ packages
```

#### Windows
```bash
# Install dependencies via vcpkg or build SFML from source
# Use Visual Studio or MinGW

cmake -DCMAKE_BUILD_TYPE=Release .
cmake --build . --config Release
```

## Testing

### Local Testing
```bash
# Basic functionality test
./DeerPortal --test

# Version verification
./DeerPortal --version  # (if implemented)
```

### CI/CD Testing
- **cmake.yml**: Continuous integration builds on push/PR
- **test-release.yml**: Manual testing of release pipeline
- **release.yml**: Production release on version tags

### Test Coverage
- Cross-platform compilation (Linux, Windows, macOS)
- Package generation and integrity
- SFML 3.0.1 integration
- Runtime version display
- Asset loading verification

## Code Quality

### Current Grade: B+ (85/100)
See `ai-docs/code-quality-analysis.md` for detailed assessment and improvement tracking.

### Key Improvements Made
- Professional error handling patterns
- Modern C++ smart pointer usage
- SFML 3.0.1 API compliance
- Constructor anti-pattern fixes

### Coding Standards
- **C++17**: Modern C++ features, smart pointers, RAII
- **SFML 3.0**: Follow SFML 3.0 patterns and API
- **Memory Safety**: Prefer `std::unique_ptr` for SFML objects
- **Header Dependencies**: Forward declarations where possible
- **Code Formatting**: LLVM style via clang-format (see `.clang-format`)

### Code Formatting

**Automatic Formatting**
```bash
# Format all C++ files
clang-format -i src/*.cpp src/*.h

# Format specific file
clang-format -i src/game.cpp

# Check formatting without modifying
clang-format --dry-run --Werror src/*.cpp src/*.h
```

**Style Guidelines (LLVM-based)**
- **Indentation**: 2 spaces, no tabs
- **Line Length**: 100 characters max
- **Braces**: Attach style (`if (condition) {`)
- **Pointers**: Left alignment (`int* ptr`)
- **Includes**: Sorted and grouped (standard, SFML, project)
- **Functions**: camelCase for functions and variables
- **Classes**: PascalCase for classes and types

## Debugging and Troubleshooting

### Common Issues
- **Asset Loading**: Check BASE_PATH configuration and asset paths
- **SFML Linking**: Ensure correct SFML 3.0 installation
- **Version Mismatches**: Verify all manual version files updated
- **Build Failures**: Check dependencies and CMake configuration

### Debug Builds
```bash
cmake -DCMAKE_BUILD_TYPE=Debug -DSHOW_FPS_COUNTER=ON .
make
```

### Verbose Builds
```bash
cmake -DCMAKE_VERBOSE_MAKEFILE=ON .
make VERBOSE=1
```

---

For comprehensive CI/CD pipeline documentation, see `ai-docs/github-actions.md`.  
For code quality analysis and improvement tracking, see `ai-docs/code-quality-analysis.md`.