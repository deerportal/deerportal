# DeerPortal Packaging

This directory contains packaging configuration files for creating distributable packages across different platforms.

## Files

### macOS DMG Packaging

- **`dmg_setup.scpt`** - AppleScript for configuring DMG window appearance, icon positioning, and Applications symlink
- **`Info.plist`** - macOS app bundle property list with app metadata and system requirements
- **`dmg_background.png`** *(optional)* - Background image for DMG window (500x300 recommended)
- **`DeerPortal.icns`** *(optional)* - macOS app icon in ICNS format

## DMG Customization

### Adding a Background Image
1. Create a 500x300 PNG image
2. Save it as `dmg_background.png` in this directory
3. The image will be automatically used during DMG creation

### Adding an App Icon
1. Create or convert your icon to ICNS format (macOS icon format)
2. Save it as `DeerPortal.icns` in this directory
3. Update the CMakeLists.txt `CPACK_BUNDLE_ICON` path if needed

### Customizing DMG Layout
Edit `dmg_setup.scpt` to:
- Change window size: Modify `bounds` values
- Reposition icons: Change `position` coordinates
- Adjust icon size: Modify `icon size of viewOptions`

## Building Packages

### Local Build (macOS)
```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make
cpack -G DragNDrop
```

### GitHub Actions
The release workflow automatically creates packages for all platforms when you:
1. Push a version tag: `git tag v0.9.0 && git push origin v0.9.0`
2. Or manually trigger the workflow in GitHub Actions

## Package Outputs

- **macOS**: `DeerPortal-0.9.0-macOS.dmg`
- **Windows**: `DeerPortal-0.9.0-Windows.exe` (NSIS installer)
- **Linux**: `DeerPortal-0.9.0-Linux.tar.gz` and `DeerPortal-0.9.0-Linux.deb` 