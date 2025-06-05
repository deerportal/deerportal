# DeerPortal Packaging

This directory contains packaging configuration files for creating distributable packages across different platforms.

## Files

### macOS DMG Packaging

- **`dmg_setup.scpt`** - AppleScript for configuring DMG window appearance, icon positioning, and Applications symlink
- **`Info.plist`** - macOS app bundle property list with app metadata and system requirements
- **`dmg_background.png`** - Background image for DMG window (copied from `assets/img/background_land.png`)
- **`DeerPortal.icns`** - macOS app icon in ICNS format (generated from `assets/img/deerportal.png`)
- **`DeerPortal.iconset/`** - Iconset directory used to generate the .icns file

## Icon Creation Process (Completed)

The DeerPortal.icns file was created from the existing `assets/img/deerportal.png` (256x256):

```bash
# Create iconset directory
mkdir packaging/DeerPortal.iconset

# Generate different sizes using sips
cd packaging
sips -z 16 16 ../assets/img/deerportal.png --out DeerPortal.iconset/icon_16x16.png
sips -z 32 32 ../assets/img/deerportal.png --out DeerPortal.iconset/icon_16x16@2x.png
sips -z 32 32 ../assets/img/deerportal.png --out DeerPortal.iconset/icon_32x32.png
sips -z 64 64 ../assets/img/deerportal.png --out DeerPortal.iconset/icon_32x32@2x.png
sips -z 128 128 ../assets/img/deerportal.png --out DeerPortal.iconset/icon_128x128.png
sips -z 256 256 ../assets/img/deerportal.png --out DeerPortal.iconset/icon_128x128@2x.png
cp ../assets/img/deerportal.png DeerPortal.iconset/icon_256x256.png

# Convert to .icns
iconutil -c icns DeerPortal.iconset
```

## Building Packages

### ⚠️ Important: Use Pure CMake Solution

**The DeerPortal macOS packaging now uses a complete pure CMake solution - no bash scripts needed!**

### Automated Build (macOS) - FINAL SOLUTION
```bash
# Clean build
make clean

# Build app bundle
make

# Create DMG package (recommended for macOS)
make dmg

# Alternative: Use CPack for other platforms
make package  # Creates STGZ/TGZ on macOS, but DMG is better
```

**What `make dmg` does:**
- ✅ Creates proper app bundle structure
- ✅ Bundles SFML libraries with fixed identities
- ✅ Signs app with ad-hoc signature  
- ✅ Fixes all library conflicts (no more "Class SFApplication" warnings)
- ✅ Suppresses install_name_tool warnings
- ✅ Creates compressed DMG with background and Applications symlink
- ✅ Pure CMake - no bash scripts required

### Build Process Summary

**For reliable DMG creation:**
1. `make clean` (clean previous build)
2. `make` (build app bundle)
3. `make dmg` (create DMG package)
4. Result: `DeerPortal-0.9.0-macOS.dmg` (~13MB) ready for distribution

**Key Differences:**
- ✅ `make dmg`: Creates proper macOS DMG (recommended)
- ⚠️ `make package`: Uses CPack, creates STGZ/TGZ on macOS (works but not ideal)

### No More Manual Fixes Needed

The following are **automatically handled** by `make dmg`:
- ✅ Library identity fixing  
- ✅ Code signing
- ✅ Warning suppression
- ✅ Proper app bundle structure
- ✅ DMG creation and compression

### Manual DMG Fixing (if needed)
If you have an existing DMG that shows "damaged or incomplete" errors:

```bash
# Fix an existing DMG
./scripts/fix_dmg.sh DeerPortal-0.9.0-macOS.dmg
```

The `fix_dmg.sh` script will:
1. Convert DMG to writable format
2. Mount it and sign the app inside with ad-hoc signature
3. Remove quarantine attributes
4. Convert back to compressed read-only format
5. Replace the original DMG

## Package Outputs

- **macOS**: `DeerPortal-0.9.0-macOS.dmg` (~12MB with bundled libraries)
- **Windows**: `DeerPortal-0.9.0-Windows.exe` (NSIS installer)
- **Linux**: `DeerPortal-0.9.0-Linux.tar.gz` and `DeerPortal-0.9.0-Linux.deb`

## DMG Signing Fix

### The Problem
CPack creates DMGs where the app inside is not code-signed, causing macOS Gatekeeper to show "damaged or incomplete" errors.

### The Solution
The `scripts/fix_dmg.sh` script automatically:
- Converts DMG to writable format
- Signs the app with ad-hoc signature: `codesign --force --deep --sign -`
- Removes quarantine attributes: `xattr -rd com.apple.quarantine`
- Converts back to compressed read-only format

### Integration
- **Automated**: Run `make fix_dmg` after `make package`
- **Manual**: Run `./scripts/fix_dmg.sh <dmg_file>`

## Files Status

- ✅ **DeerPortal.icns** - Created from game assets
- ✅ **dmg_background.png** - Copied from `background_land.png`
- ✅ **Info.plist** - Existing configuration
- ✅ **dmg_setup.scpt** - Exists but currently disabled
- ✅ **CMakeLists.txt** - Updated with Bundle generator and proper paths
- ✅ **fix_dmg.sh** - Script to fix code signing in DMG

## Troubleshooting

If packaging fails:
1. Clean all build artifacts first
2. Use out-of-source build
3. Use `Bundle` generator (never `DragNDrop`)
4. Check that all required files exist in packaging directory
5. Run `make fix_dmg` after successful package creation

### macOS Security Issues

If you get "damaged or incomplete" errors:

1. **Use the automated fix** (RECOMMENDED):
   ```bash
   make fix_dmg
   # or manually:
   ./scripts/fix_dmg.sh DeerPortal-0.9.0-macOS.dmg
   ```

2. **Remove quarantine attributes** (for installed apps):
   ```bash
   sudo xattr -rd com.apple.quarantine /Applications/DeerPortal.app
   ```

3. **Sign the app** (for local development):
   ```bash
   codesign --force --deep --sign - DeerPortal.app
   ```

4. **Allow app from unidentified developer**:
   - Go to System Preferences → Security & Privacy → General
   - Click "Allow Anyway" next to the DeerPortal warning
   - Or try right-clicking the app and selecting "Open"

5. **For distribution**: Consider getting an Apple Developer ID for proper code signing

### Build Process Summary

**For reliable DMG creation:**
1. `cmake . -DCMAKE_BUILD_TYPE=Release`
2. `make package` (creates DMG with unsigned app)
3. `make fix_dmg` (signs app inside DMG)
4. Result: Working DMG without "damaged or incomplete" errors

The `fix_dmg` target automatically calls `scripts/fix_dmg.sh` with the correct DMG filename. 