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

### Game States and Flow
- Multiple game states managed through `Game::currentState`
- Turn-based gameplay with dice rolling and movement
- Card activation integrated into field processing system

### Resource Management
- Players collect diamonds for victory condition
- Cards provide strategic interaction between players
- Meditation system regenerates player areas

## Documentation System

### Documentation Files Created
- `CARDS.md` - Comprehensive card reference with images and mechanics
- `handbook.tex` - Professional LaTeX handbook with card documentation
- `ai-docs/handbook-generation.md` - PDF generation instructions

### LaTeX Handbook Features (Created 2024)
**Structure Organization:**
- Cards grouped by type (Stop, Remove, Diamond, Diamond x2) instead of by element color
- Professional formatting with custom colors for each element
- Strategic analysis and technical implementation details
- Comprehensive table of contents and cross-references

**LaTeX Features Used:**
- Custom color definitions for elements (`\definecolor`)
- Professional table formatting (`booktabs`, `tabularx`)
- Enhanced headers/footers (`fancyhdr`)
- Hyperlinks and bookmarks (`hyperref`)
- Structured sections with proper LaTeX hierarchy

**PDF Generation Process:**
- Command line: `pdflatex handbook.tex` (run twice for TOC)
- Build scripts provided for Linux/macOS/Windows
- CMake integration available
- CI/CD workflows documented for automated building

### Documentation Maintenance
- LaTeX build artifacts added to `.gitignore`
- Optional PDF tracking (commented out in gitignore)
- Systematic approach to documentation updates
- Version control integration for handbook releases

## Key Technical Patterns
- SFML for graphics and audio
- State management pattern for game flow
- Component-based entity system for players
- Texture management system for efficient asset loading
- Professional documentation with LaTeX typesetting

## Development History
- Game evolved from Buddhist temple inspiration
- Multi-platform support (CMake build system)
- Active development with version tracking
- Handbook exists but needed card documentation supplement
- Enhanced with professional LaTeX handbook (2024) 