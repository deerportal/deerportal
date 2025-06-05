# AI Instructions for Deer Portal Game Development

## Project Overview
Deer Portal is a multiplayer board game built with C++ and SFML, featuring four classical elements (Water, Earth, Fire, Air) and strategic card-based gameplay.

## Development Guidelines

### Code Organization
- Always create reusable and modular code
- Split functionalities into separate files/classes
- Follow systematic approach considering all impacted files
- Store temporary data in `.data` folder

### Documentation Standards
- Maintain `ai-instructions.md` as memory bank
- Create appropriate `.gitignore` for language and files
- Document all major features and mechanics

### Git Workflow Guidelines (Updated 2024-06-02)

**AI Permissions for Git Operations:**
- ✅ **CAN EDIT**: `.gitignore` files directly
- ✅ **CAN CHECK**: Read-only git commands (`git status`, `git log`, `git diff`, `git branch`)
- ❓ **MUST ASK FIRST**: Any modification commands:
  - `git add`, `git commit`, `git push`, `git pull`
  - `git merge`, `git rebase`, `git branch -D`
  - `git reset`, `git checkout`, `git switch`
  - Any command that changes repository state

**Workflow Process:**
1. AI edits files as requested
2. AI asks permission before committing changes
3. User reviews and approves git operations
4. User maintains full control over repository state

**Branch Management:**
- Development happens on feature branches (e.g., `cards`)
- Master branch contains stable releases
- AI should always check current branch before making suggestions
- Branch switching requires explicit user permission

**Collaboration Best Practices:**
- AI documents changes clearly for commit messages
- User retains authority over release timing
- Version control decisions remain with project maintainer
- Emergency rollbacks handled by user only

## Game Architecture Analysis

### macOS Packaging (Fixed 2024-06-05)

**Final Pure CMake Solution:**
Successfully resolved all CMake/CPack packaging issues for macOS with pure CMake approach:

**Issues Fixed:**
1. **Invalid escape sequences**: Fixed regex patterns in `CPACK_SOURCE_IGNORE_FILES` 
2. **Missing icon file**: Created `packaging/DeerPortal.icns` from existing `assets/img/deerportal.png`
3. **Nested app structure**: Replaced CPack Bundle generator with custom DMG creation
4. **Background image**: Added `packaging/dmg_background.png` from game assets
5. **Project name**: Changed from "myproject" to "DeerPortal" in CMakeLists.txt
6. **Icon not displaying**: Fixed Info.plist template and added post-build icon copy command
7. **"Damaged or incomplete" error**: Bundled SFML libraries and fixed paths for distribution
8. **macOS Gatekeeper blocking**: Added automatic code signing with ad-hoc signature
9. **SFML class conflicts**: Fixed library identities to prevent system/bundled conflicts
10. **install_name_tool warnings**: Suppressed with `2>/dev/null || true`
11. **Package target conflicts**: Use `make dmg` instead of `make package` for macOS
12. **SFML library conflicts (RESOLVED)**: Fixed @rpath dependencies and bundled all required libraries

**SFML Library Conflict Resolution (2024-06-05):**
The "Class SFApplication is implemented in both" errors were caused by:
- Bundled SFML libraries still had @rpath dependencies that loaded system libraries
- Missing bundling of freetype and libpng dependencies
- Incomplete library path fixes

**Final Solution:**
- Bundle all SFML dependencies: freetype, libpng
- Fix all @rpath references to use @executable_path/../Frameworks/
- Fix all system library paths to use bundled versions
- Apply fixes to both app bundle and DMG creation
- Complete library identity fixes prevent conflicts

**Library Bundling Process:**
- SFML 3.0 libraries: graphics, audio, network, window, system (~1.6MB)
- Dependencies: freetype, libpng (~500KB)
- All paths fixed to use bundled versions exclusively
- No system library conflicts or warnings

**Icon Creation Process:**
- Source: `assets/img/deerportal.png` (256x256 RGBA PNG)
- Created iconset with multiple sizes: 16x16, 32x32, 64x64, 128x128, 256x256 + retina versions
- Generated using macOS tools: `sips` for resizing, `iconutil` for .icns conversion
- Final result: `packaging/DeerPortal.icns` (105KB)

**Final Working Configuration:**
- **Build Process**: Pure CMake, no bash scripts needed
- **App Bundle**: Proper structure with `Contents/MacOS/DeerPortal` executable
- **Library Bundling**: SFML 3.0 + dependencies in `Contents/Frameworks/` with fixed paths and identities
- **Code Signing**: Automatic ad-hoc signature for macOS compatibility
- **DMG Creation**: Custom CMake target that creates properly signed DMG
- **Warning Suppression**: All `install_name_tool` warnings suppressed
- **Output**: `DeerPortal-0.9.0-macOS.dmg` (~13MB with bundled libraries)
- **No Library Conflicts**: App runs without SFML class conflict warnings

**Correct Build Process:**
```bash
# Clean build
make clean

# Build app bundle
make

# Create DMG package (macOS)
make dmg

# Alternative: Regular CPack for other platforms
make package  # Creates STGZ/TGZ on macOS, proper packages on other platforms
```

**App Bundle Structure Achieved:**
```
DeerPortal.app/
├── Contents/
│   ├── MacOS/DeerPortal (executable)
│   ├── Frameworks/ (bundled SFML + dependencies with fixed identities)
│   ├── Resources/ (game assets + DeerPortal.icns)
│   └── Info.plist (custom template with metadata)
```

**Library Identity Fixes:**
- Bundled libraries use `@executable_path/../Frameworks/` identities
- Internal dependencies fixed to use bundled paths, not system `@rpath`
- All @rpath references replaced with explicit bundled paths
- Prevents conflicts between system SFML and bundled SFML
- Eliminates "Class SFApplication is implemented in both" errors completely

**macOS Security Notes:**
- Apps may need quarantine attributes removed: `sudo xattr -rd com.apple.quarantine /Applications/DeerPortal.app`
- For distribution, consider Apple Developer ID for proper code signing
- Users may need to allow "unidentified developer" in Security preferences

**Key Targets:**
- `make dmg`: Creates macOS DMG package (recommended for macOS)
- `make package`: Uses CPack for non-macOS platforms
- Pure CMake solution eliminates bash script dependencies

**Final Status: ✅ FULLY RESOLVED**
- No SFML class conflicts
- App runs successfully on macOS
- Complete library bundling and isolation
- Professional packaging ready for distribution

### Card System (Analyzed 2024)
The game features a sophisticated card system with 128 total cards:

**Card Distribution:**
- 4 elements × 32 cards each = 128 total cards
- Each element deck: 8 Stop + 8 Remove Card + 8 Diamond + 8 Diamond x2

**Card Mechanics:**
- Own element runes: Card discarded, advance to next
- Other element runes: Card effect executed against target element
- Cards cannot target own element area
- Decks become inactive when exhausted

**Card Types and Effects:**
1. **Stop Cards**: Freeze target player for 1 turn (`frozenLeft += 1`)
2. **Remove Card**: Randomly removes diamond/card from target area  
3. **Diamond Cards**: Steal 1 diamond + 1 cash from target area
4. **Diamond x2 Cards**: Steal 2 diamonds + 2 cash from target area

**Visual Assets:**
- Located in `assets/img/cards/`
- Naming: `card-{element}-{type}.small.png`
- Elements: water(blue), earth(green), fire(red), air(yellow)
- Types: stop, remove-card, diam, 2-diam

**Key Implementation Files:**
- `src/card.h` - Card data structure and types array
- `src/cardsdeck.h/.cpp` - Deck management and rendering
- `src/cardslist.h/.cpp` - Card distribution and shuffling
- `src/command.cpp::processCard()` - Card effect execution
- `src/textureholder.cpp` - Card texture loading

### Card Notification System

A modular overlay system that displays contextual information when cards are picked up:

- **Purpose**: Shows clear explanations of card effects to help players understand what happened
- **Visual Design**: Semi-transparent background with white text, centered over game board portal area
- **Size Constraints**: Maximum 75% of game board area with dynamic font scaling (30px default, minimum 14px)
- **Positioning**: Centers on game board portal area, not full screen (better focus)
- **Dismissal**: Disappears when next player clicks dice to throw (natural game flow)
- **Messages**: Context-aware text for all card types ("diamond", "stop", "card", "diamond x 2")
- **Integration**: Triggered from `Command::processCard()`, dismissed in `Game::throwDiceMove()`
- **Modular**: Easy to replace with graphics-based implementation in future

### Game States and Flow
- Multiple game states managed through `