#!/bin/bash

# Script to fix DMG by signing the app inside it
# Usage: ./scripts/fix_dmg.sh DeerPortal-0.9.0-macOS.dmg

DMG_FILE="$1"
if [[ -z "$DMG_FILE" ]]; then
    echo "Usage: $0 <dmg_file>"
    exit 1
fi

if [[ ! -f "$DMG_FILE" ]]; then
    echo "Error: DMG file '$DMG_FILE' not found"
    exit 1
fi

echo "🔧 Fixing DMG: $DMG_FILE"

# Create a temporary writable DMG
TEMP_DMG="${DMG_FILE%.dmg}-temp.dmg"
echo "📦 Converting to writable DMG..."
hdiutil convert "$DMG_FILE" -format UDRW -o "$TEMP_DMG"

# Mount the writable DMG
echo "💿 Mounting writable DMG..."
MOUNT_POINT=$(hdiutil attach "$TEMP_DMG" | grep "/Volumes" | awk '{print $3}')

if [[ -z "$MOUNT_POINT" ]]; then
    echo "❌ Failed to mount DMG"
    exit 1
fi

echo "📁 Mounted at: $MOUNT_POINT"

# Sign the app inside the DMG
APP_PATH="$MOUNT_POINT/DeerPortal.app"
if [[ -d "$APP_PATH" ]]; then
    echo "✍️  Signing app at: $APP_PATH"
    codesign --force --deep --sign - "$APP_PATH"
    
    # Remove quarantine attributes
    echo "🔓 Removing quarantine attributes..."
    xattr -rd com.apple.quarantine "$APP_PATH" 2>/dev/null || true
    
    # Verify signing
    echo "✅ Verifying signature..."
    codesign -dv "$APP_PATH" 2>&1
else
    echo "❌ App not found at: $APP_PATH"
    hdiutil detach "$MOUNT_POINT"
    exit 1
fi

# Unmount the DMG
echo "📤 Unmounting DMG..."
hdiutil detach "$MOUNT_POINT"

# Convert back to read-only compressed format
echo "🗜️  Converting back to read-only compressed format..."
FIXED_DMG="${DMG_FILE%.dmg}-fixed.dmg"
hdiutil convert "$TEMP_DMG" -format UDBZ -o "$FIXED_DMG"

# Clean up temporary file
rm "$TEMP_DMG"

# Replace original with fixed version
mv "$FIXED_DMG" "$DMG_FILE"

echo "✅ DMG fixed successfully: $DMG_FILE"
echo "🎉 The app inside should now work without 'damaged or incomplete' errors!" 