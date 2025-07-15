# Fullscreen & AppImage Fix Plan

## 🛡️ Issue Analysis

After comprehensive investigation, I've identified the root causes of both problems:

### 1. Fullscreen F/F11 Keys Not Working

**Root Cause:** The main event loop in `src/game.cpp` (lines 679-723) handles events directly and **never calls** `GameInput::processEvents()`. This means the professionally implemented fullscreen handler in `game-input.cpp:42` is never executed!

**Current State:**
- ✅ Fullscreen implementation EXISTS and is professional (WindowManager + SFML 3.0.1)
- ✅ F11 key handler EXISTS in `GameInput::handleKeyboardInput()` 
- ❌ Main event loop bypasses the input system entirely
- ❌ GameInput methods are never called during runtime

### 2. AppImage Asset Loading Issues

**Root Cause:** The asset path resolution in `src/filetools.h` has excellent macOS app bundle detection but **no AppImage/APPDIR detection**. Additionally, our CI workflow copies assets to the wrong location.

**Current Issues:**
- ✅ Asset path resolution system EXISTS and is robust
- ❌ No APPDIR environment variable detection 
- ❌ AppImage CI copies assets to `usr/bin/assets/` instead of proper location
- ❌ No fallback for executable-relative asset paths

## 🏹 Fix Plan

### Phase 1: Fix Fullscreen Input Processing

**Problem:** Main event loop bypasses GameInput system entirely

**Solution:** Integrate GameInput::processEvents() into main event loop

**Files to Modify:**
- `src/game.cpp` - Integrate input->processEvents() call
- Remove duplicate event handling that bypasses input system

**Implementation:**
```cpp
// In main event loop (game.cpp ~line 679)
while (const std::optional<sf::Event> eventOpt = window.pollEvent()) {
  const sf::Event& event = *eventOpt;
  
  // Let GameInput handle ALL events (including F11)
  input->processEvents(window);
  
  // Keep only essential direct handlers (window close)
  if (event.is<sf::Event::Closed>()) {
    window.close();
  }
  
  // Remove duplicate Escape handling - let GameInput handle it
}
```

### Phase 2: Fix AppImage Asset Path Resolution

**Problem:** No AppImage environment detection in path resolution

**Solution:** Add APPDIR detection to filetools.h

**Files to Modify:**
- `src/filetools.h` - Add AppImage APPDIR detection
- `.github/workflows/*.yml` - Fix asset copying location

**Implementation:**
```cpp
static std::string get_full_path(std::string path) {
  // 1. Check DP_DIR environment variable first (existing)
  if (getenv("DP_DIR") != NULL) {
    return std::string(getenv("DP_DIR")) + path;
  }
  
  // 2. NEW: Check for AppImage APPDIR environment variable
  if (getenv("APPDIR") != NULL) {
    // Try multiple asset locations within AppImage
    std::string appdir = std::string(getenv("APPDIR"));
    std::vector<std::string> appimage_paths = {
      appdir + "/usr/share/deerportal/" + path,  // Standard location
      appdir + "/usr/bin/" + path,               // Current location
      appdir + "/" + path                        // Root fallback
    };
    
    for (const auto& test_path : appimage_paths) {
      if (file_exists(test_path)) {
        return test_path;
      }
    }
  }
  
  // 3. macOS app bundle detection (existing)
  // 4. Fallback to relative path (existing)
}
```

### Phase 3: Fix AppImage CI Asset Bundling

**Problem:** Assets copied to wrong location in AppImage

**Solution:** Copy assets to standard Linux location

**CI Workflow Changes:**
```bash
# Current (WRONG):
cp -r ../assets DeerPortal.AppDir/usr/bin/

# Fixed (CORRECT):
mkdir -p DeerPortal.AppDir/usr/share/deerportal/
cp -r ../assets DeerPortal.AppDir/usr/share/deerportal/
```

## 🛡️ Implementation Priority

### Critical (Fix Immediately):
1. **Fix event loop integration** - Fullscreen F11 functionality
2. **Add APPDIR detection** - AppImage asset loading

### Important (Next Release):
3. **Update CI asset locations** - Proper AppImage asset bundling
4. **Add file existence helper** - Robust path validation

## 🏹 Testing Strategy

### Fullscreen Testing:
1. Build game locally
2. Test F, F11, and Cmd+Enter keys
3. Verify window recreation works properly
4. Test on all platforms (macOS, Windows, Linux)

### AppImage Testing:
1. Create test AppImage with current CI
2. Test asset loading inside AppImage environment
3. Verify APPDIR environment variable detection
4. Test fallback paths work correctly

## 🔧 Technical Details

### Event Flow Fix:
```
BEFORE: window.pollEvent() → Direct handling → Bypass GameInput
AFTER:  window.pollEvent() → input->processEvents() → Proper handlers
```

### Asset Resolution Flow:
```
BEFORE: DP_DIR → macOS bundle → relative path
AFTER:  DP_DIR → APPDIR (multiple paths) → macOS bundle → relative path
```

### AppImage Structure:
```
DeerPortal.AppDir/
├── usr/
│   ├── bin/
│   │   └── DeerPortal (executable)
│   └── share/
│       └── deerportal/
│           └── assets/ (assets here!)
```

## 🎯 Expected Results

After fixes:
- ✅ F11 and F keys work for fullscreen toggle
- ✅ AppImage finds assets correctly in APPDIR environment
- ✅ Maintains compatibility with existing deployment methods
- ✅ Professional error handling and fallbacks preserved

Both issues are **architectural integration problems** rather than missing functionality - the systems exist but aren't properly connected!