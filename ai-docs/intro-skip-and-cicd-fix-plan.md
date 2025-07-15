# Intro Skip and CI/CD Artifacts Fix Plan

## 🎯 Issue Summary

Two critical issues identified for immediate resolution:

1. **Intro Mouse Click Skip Broken**: Users cannot skip intro with mouse clicks
2. **CI/CD Artifact Upload Failures**: TGZ (Linux) and ZIP (Windows) artifacts not uploading to GitHub releases

## 🛡️ Issue 1: Intro Mouse Click Skip

### **Root Cause Analysis**

**Problem**: GameInput modular system doesn't handle `state_intro_shader`
**Discovery**: During the great input system unification, intro skipping was lost

**Current State**:
- ✅ **Old Code**: `Game::handleLeftClick()` has intro skip logic (lines 476-480)
- ❌ **New Code**: `GameInput::handleLeftClick()` missing `state_intro_shader` case
- ❌ **Result**: Mouse clicks during intro are completely ignored

**Technical Details**:
```cpp
// OLD WORKING CODE (Game::handleLeftClick())
if (currentState == state_intro_shader) {
  stateManager->showMenu();  // Skip to menu
}

// NEW CODE ISSUE (GameInput::handleLeftClick())
switch (game->currentState) {
  case Game::state_game: ...
  case Game::state_setup_players: ...
  // MISSING: case Game::state_intro_shader:
}
```

### **Solution Implementation**

**Fix Location**: `src/game-input.cpp` - `GameInput::handleLeftClick()` method

**Required Changes**:
```cpp
// Add to switch statement in GameInput::handleLeftClick()
case Game::state_intro_shader:
  // Skip intro and go to main menu
  game->stateManager->showMenu();
  break;
```

**Files to Modify**:
- `src/game-input.cpp` - Add intro skip case to switch statement

**Expected Result**:
- ✅ Mouse clicks during intro will skip to main menu
- ✅ Maintains existing 8-second auto-skip functionality
- ✅ Consistent with original game behavior

## 🏗️ Issue 2: CI/CD Artifact Upload Failures

### **Root Cause Analysis**

**Problem**: Specific artifact types (TGZ, ZIP) failing to upload to GitHub releases
**GitHub Actions Run**: 16253618428 - TGZ and ZIP uploads failed

**Potential Causes Identified**:

1. **File Pattern Mismatches**:
   ```yaml
   # Current patterns might not match actual generated files
   file: build/*.tar.gz  # Might not exist
   file: build/*.zip     # Might not exist
   ```

2. **Conditional Logic Issues**:
   ```yaml
   # Conditions might be failing
   if: steps.check_linux_files.outputs.tgz_exists == 'true'
   if: steps.check_windows_files.outputs.zip_exists == 'true'
   ```

3. **File Generation Problems**:
   - CPack might not be generating expected file formats
   - Files might be in different directories
   - Naming conventions might have changed

4. **GitHub Actions Upload Issues**:
   - upload-release-action@v2 might have compatibility issues
   - File paths might be incorrect
   - Permissions or timing issues

### **Diagnostic Strategy**

**Phase 1: Enhanced Logging and Debugging**
```yaml
# Add comprehensive file listing before upload attempts
- name: Debug Package Files
  run: |
    echo "=== Build directory contents ==="
    find build -type f -name "*DeerPortal*" -o -name "*.tar.gz" -o -name "*.deb" -o -name "*.zip" -o -name "*.exe" | sort
    echo "=== Working directory contents ==="
    ls -la
    echo "=== CPack configuration ==="
    cd build && make help | grep -E "(package|cpack)" || echo "No package targets found"
```

**Phase 2: File Pattern Validation**
```yaml
# Verify files exist before upload attempts
- name: Validate Artifact Files
  id: validate_files
  run: |
    cd build
    echo "Searching for generated packages..."
    
    # Find all possible package files
    TGZ_FILES=$(find . -name "*.tar.gz" | head -5)
    ZIP_FILES=$(find . -name "*.zip" | head -5)
    DEB_FILES=$(find . -name "*.deb" | head -5)
    EXE_FILES=$(find . -name "*.exe" | head -5)
    
    echo "TGZ_FILES=${TGZ_FILES}" >> $GITHUB_OUTPUT
    echo "ZIP_FILES=${ZIP_FILES}" >> $GITHUB_OUTPUT
    echo "DEB_FILES=${DEB_FILES}" >> $GITHUB_OUTPUT
    echo "EXE_FILES=${EXE_FILES}" >> $GITHUB_OUTPUT
```

### **Solution Implementation**

**1. Fix File Detection Logic**
```yaml
# Replace static pattern checks with dynamic file discovery
- name: Check for Linux packages
  id: check_linux_files
  run: |
    cd build
    # Use more robust file detection
    TGZ_COUNT=$(find . -name "*.tar.gz" -type f | wc -l)
    DEB_COUNT=$(find . -name "*.deb" -type f | wc -l)
    APPIMAGE_COUNT=$(find . -name "*.AppImage" -type f | wc -l)
    
    echo "tgz_exists=$([[ $TGZ_COUNT -gt 0 ]] && echo 'true' || echo 'false')" >> $GITHUB_OUTPUT
    echo "deb_exists=$([[ $DEB_COUNT -gt 0 ]] && echo 'true' || echo 'false')" >> $GITHUB_OUTPUT
    echo "appimage_exists=$([[ $APPIMAGE_COUNT -gt 0 ]] && echo 'true' || echo 'false')" >> $GITHUB_OUTPUT
    
    # Debug output
    echo "Found $TGZ_COUNT TGZ files, $DEB_COUNT DEB files, $APPIMAGE_COUNT AppImage files"
    find . -name "*.tar.gz" -o -name "*.deb" -o -name "*.AppImage" | sort
```

**2. Improve Upload Reliability**
```yaml
# Add fallback upload strategies
- name: Upload Linux TGZ to Release
  if: github.event_name == 'push' && !inputs.test_mode && steps.check_linux_files.outputs.tgz_exists == 'true'
  uses: svenstaro/upload-release-action@v2
  with:
    repo_token: ${{ secrets.GITHUB_TOKEN }}
    file: build/*.tar.gz
    tag: ${{ github.ref }}
    overwrite: true
    file_glob: true
  continue-on-error: true  # Don't fail entire workflow

- name: Upload TGZ Fallback (if primary fails)
  if: github.event_name == 'push' && !inputs.test_mode && steps.check_linux_files.outputs.tgz_exists == 'true'
  run: |
    # Alternative upload method using gh CLI
    for file in build/*.tar.gz; do
      if [ -f "$file" ]; then
        gh release upload ${{ github.ref_name }} "$file" --clobber || echo "Failed to upload $file"
      fi
    done
  env:
    GITHUB_TOKEN: ${{ secrets.GITHUB_TOKEN }}
```

**3. Enhanced Error Handling**
```yaml
# Add comprehensive error reporting
- name: Report Upload Status
  if: always()
  run: |
    echo "=== Upload Status Summary ==="
    echo "Linux TGZ: ${{ steps.upload_tgz.outcome }}"
    echo "Linux DEB: ${{ steps.upload_deb.outcome }}"
    echo "Windows ZIP: ${{ steps.upload_zip.outcome }}"
    echo "Windows EXE: ${{ steps.upload_exe.outcome }}"
    
    # List what actually got uploaded
    gh release view ${{ github.ref_name }} --json assets --jq '.assets[].name' || echo "Could not list release assets"
  env:
    GITHUB_TOKEN: ${{ secrets.GITHUB_TOKEN }}
```

### **Files to Modify**

**1. Release Workflow Enhancement**:
- `.github/workflows/release.yml` - Add debugging and improve upload logic

**2. Test Release Workflow**:
- `.github/workflows/test-release.yml` - Apply same fixes for testing

**3. CI Workflow** (if needed):
- `.github/workflows/cmake.yml` - Ensure artifact uploads work in CI

## 🎯 Implementation Priority

### **Phase 1: Quick Fixes (Immediate)**
1. ✅ **Fix intro mouse skip** - Simple switch case addition
2. ✅ **Add CI/CD debugging** - Enhanced logging for diagnosis

### **Phase 2: CI/CD Robustness (Next)**
3. ✅ **Improve file detection** - Dynamic file discovery
4. ✅ **Add upload fallbacks** - Multiple upload strategies
5. ✅ **Enhanced error handling** - Comprehensive status reporting

### **Phase 3: Testing and Validation**
6. ✅ **Test intro skip** - Verify mouse clicking works
7. ✅ **Test artifact uploads** - Run test-release workflow
8. ✅ **Validate full release** - Complete end-to-end testing

## 🔧 Testing Strategy

### **Intro Skip Testing**
```bash
# Local testing approach
1. Build game locally
2. Launch game and wait for intro
3. Click mouse during intro animation
4. Verify immediate transition to main menu
```

### **CI/CD Testing**
```bash
# Test release workflow validation
1. Use test-release.yml with debugging enabled
2. Check artifact generation and file patterns
3. Validate upload success for all platforms
4. Verify GitHub release has all expected files
```

## 🚀 Expected Results

### **Intro Skip Fix**
- ✅ Mouse clicks during intro skip to main menu
- ✅ Maintains keyboard skip functionality (if any)
- ✅ Preserves automatic 8-second timeout
- ✅ Consistent user experience across input types

### **CI/CD Artifact Fix**
- ✅ All artifact types upload successfully (TGZ, ZIP, DEB, EXE, DMG, AppImage)
- ✅ Robust error handling prevents workflow failures
- ✅ Enhanced debugging for future troubleshooting
- ✅ Reliable release automation for all platforms

Both fixes represent critical user experience improvements and infrastructure reliability enhancements for professional-grade software delivery.