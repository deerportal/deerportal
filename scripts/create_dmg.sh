#!/bin/bash

# Script to create a proper macOS DMG package
# This replaces CPack Bundle generator which creates incorrect nested structures

set -e

PROJECT_NAME="DeerPortal"
VERSION=$(cat ../version.txt)
DMG_NAME="${PROJECT_NAME}-${VERSION}-macOS"
APP_NAME="${PROJECT_NAME}.app"

echo "🔧 Creating macOS DMG: ${DMG_NAME}.dmg"

# Paths
APP_PATH="${APP_NAME}"
DMG_TEMP_DIR="/tmp/${DMG_NAME}"
DMG_PATH="${DMG_NAME}.dmg"

# Check that app bundle exists
if [[ ! -d "$APP_PATH" ]]; then
    echo "❌ App bundle not found: $APP_PATH"
    echo "   Make sure to run 'make' first to build the app"
    exit 1
fi

echo "📱 Found app bundle: $APP_PATH"

# Clean up any previous temp directory
rm -rf "$DMG_TEMP_DIR"
mkdir -p "$DMG_TEMP_DIR"

# Copy app to temp directory
echo "📁 Copying app to temporary directory..."
cp -R "$APP_PATH" "$DMG_TEMP_DIR/"

# Sign the app
echo "✍️  Signing app bundle..."
codesign --force --deep --sign - "$DMG_TEMP_DIR/$APP_NAME"

# Copy background image
echo "🖼️  Adding DMG background..."
cp "../packaging/dmg_background.png" "$DMG_TEMP_DIR/.background.png"

# Create Applications symlink
echo "🔗 Creating Applications symlink..."
ln -sf /Applications "$DMG_TEMP_DIR/Applications"

# Create temporary DMG
echo "📦 Creating temporary DMG..."
TEMP_DMG="/tmp/${DMG_NAME}-temp.dmg"
hdiutil create -volname "$DMG_NAME" -srcfolder "$DMG_TEMP_DIR" -ov -format UDRW "$TEMP_DMG"

# Mount the DMG for customization
echo "💿 Mounting DMG for customization..."
MOUNT_POINT=$(hdiutil attach "$TEMP_DMG" | grep "/Volumes" | awk '{print $3}')

if [[ -z "$MOUNT_POINT" ]]; then
    echo "❌ Failed to mount temporary DMG"
    exit 1
fi

echo "📁 Mounted at: $MOUNT_POINT"

# Set DMG window properties using AppleScript
echo "🎨 Configuring DMG window appearance..."
osascript <<EOF
tell application "Finder"
    tell disk "$DMG_NAME"
        open
        set current view of container window to icon view
        set toolbar visible of container window to false
        set statusbar visible of container window to false
        set the bounds of container window to {400, 100, 920, 420}
        set viewOptions to the icon view options of container window
        set arrangement of viewOptions to not arranged
        set icon size of viewOptions to 72
        set background picture of viewOptions to file ".background.png"
        set position of item "$APP_NAME" of container window to {160, 205}
        set position of item "Applications" of container window to {360, 205}
        close
        open
        update without registering applications
        delay 2
    end tell
end tell
EOF

# Unmount the DMG
echo "📤 Unmounting DMG..."
hdiutil detach "$MOUNT_POINT"

# Convert to final compressed DMG
echo "🗜️  Converting to final compressed DMG..."
hdiutil convert "$TEMP_DMG" -format UDBZ -o "$DMG_PATH"

# Clean up
rm -rf "$DMG_TEMP_DIR"
rm "$TEMP_DMG"

echo "✅ DMG created successfully: $DMG_PATH"
echo "📊 DMG size:"
ls -lh "$DMG_PATH"

echo ""
echo "🎉 macOS package ready!"
echo "   File: $DMG_PATH"
echo "   The app inside is properly signed and should work without errors." 