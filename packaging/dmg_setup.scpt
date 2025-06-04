tell application "Finder"
    tell disk "DeerPortal"
        open
        set current view of container window to icon view
        set toolbar visible of container window to false
        set statusbar visible of container window to false
        set the bounds of container window to {400, 100, 900, 400}
        set viewOptions to the icon view options of container window
        set arrangement of viewOptions to not arranged
        set icon size of viewOptions to 96
        set background picture of viewOptions to file ".background:dmg_background.png"
        
        -- Position the DeerPortal app icon
        set position of item "DeerPortal.app" of container window to {150, 200}
        
        -- Create and position Applications alias
        make new alias file at container window to POSIX file "/Applications" with properties {name:"Applications"}
        set position of item "Applications" of container window to {350, 200}
        
        close
        open
        update without registering applications
        delay 2
    end tell
end tell 