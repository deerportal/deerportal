# Deer Portal Handbook PDF Generation

## Overview

The Deer Portal handbook is written in LaTeX (`handbook.tex`) and can be compiled into a professional PDF document. This guide covers the setup and compilation process, as well as maintaining the Markdown version for players.

## Documentation Formats

### Two Handbook Versions
1. **`handbook.tex`** - Professional LaTeX version with full formatting
2. **`HANDBOOK.md`** - Player-focused Markdown version (derived from LaTeX)

### Content Focus
- **Both versions**: Player manual only (like traditional board game instructions)
- **No technical details**: Remove all developer implementation information
- **Game rules and strategy**: Focus on gameplay, cards, and winning conditions

## Prerequisites

### LaTeX Distribution

You need a LaTeX distribution installed on your system:

#### Linux (Ubuntu/Debian)
```bash
sudo apt-get update
sudo apt-get install texlive-full
```

#### macOS
```bash
# Using Homebrew
brew install --cask mactex

# Or install BasicTeX (smaller)
brew install --cask basictex
sudo tlmgr update --self
sudo tlmgr install collection-fontsrecommended
```

#### Windows
- Download and install [MiKTeX](https://miktex.org/) or [TeX Live](https://tug.org/texlive/)

### Required LaTeX Packages

The handbook uses these packages (usually included in full distributions):
- `inputenc` - Input encoding
- `fontenc` - Font encoding  
- `geometry` - Page layout
- `graphicx` - Image inclusion
- `hyperref` - Hyperlinks and bookmarks
- `titling` - Title page customization
- `fancyhdr` - Headers and footers
- `xcolor` - Color support
- `enumitem` - Enhanced lists
- `tabularx` - Advanced tables
- `booktabs` - Professional table formatting

## PDF Generation Methods

### Method 1: Command Line (Recommended)

```bash
# Navigate to project root
cd /path/to/deerportal

# Compile the handbook
pdflatex handbook.tex

# For complete references and table of contents, run twice
pdflatex handbook.tex
pdflatex handbook.tex
```

### Method 2: Using Make (If Available)

```bash
# If there's a Makefile target for handbook
make handbook

# Or create a custom target
make pdf-handbook
```

### Method 3: LaTeX IDE

Popular LaTeX editors with built-in compilation:
- **TeXmaker** - Cross-platform, user-friendly
- **TeXstudio** - Advanced features, syntax highlighting
- **Overleaf** - Online LaTeX editor (no local installation needed)
- **Visual Studio Code** - With LaTeX Workshop extension

## Markdown Generation from LaTeX

### Manual Conversion Process

When updating `handbook.tex`, also update `HANDBOOK.md` with the same content:

1. **Content Sync**: Ensure both versions have identical game information
2. **Player Focus**: Remove any technical implementation details
3. **Format Conversion**: Convert LaTeX formatting to Markdown

### Conversion Guidelines

#### LaTeX to Markdown Mapping
```latex
% LaTeX                    → # Markdown
\section{Title}            → ## Title
\subsection{Subtitle}      → ### Subtitle
\textbf{bold}             → **bold**
\textit{italic}           → *italic*
\begin{itemize}           → -
\begin{enumerate}         → 1.
\begin{tabular}           → | Table |
```

#### Element Colors
```latex
% LaTeX with colors
\textcolor{waterblue}{\textbf{Water}}

% Markdown equivalent  
**Water** (Blue)
```

### Semi-Automated Tools

#### Pandoc Conversion (Optional)
```bash
# Install pandoc
sudo apt-get install pandoc  # Linux
brew install pandoc          # macOS

# Convert LaTeX to Markdown (requires manual cleanup)
pandoc handbook.tex -o handbook-draft.md --from latex --to markdown

# Manual cleanup required:
# - Remove technical sections
# - Fix formatting issues
# - Adjust for player audience
```

#### Custom Conversion Script
Create `scripts/tex-to-md.sh`:
```bash
#!/bin/bash
# Semi-automated LaTeX to Markdown conversion for player handbook

echo "Converting LaTeX handbook to Markdown..."

# Use pandoc for initial conversion
pandoc handbook.tex -o temp-handbook.md --from latex --to markdown

# Remove technical sections (customize as needed)
sed -i '/Technical Implementation/,$d' temp-handbook.md
sed -i '/Asset Organization/d' temp-handbook.md
sed -i '/Core Implementation/d' temp-handbook.md

# Manual review required
echo "Conversion complete. Manual review and editing required:"
echo "1. Review temp-handbook.md"
echo "2. Remove any remaining technical content"  
echo "3. Adjust formatting for readability"
echo "4. Copy to HANDBOOK.md when satisfied"
```

## Build Scripts

### PDF Generation Script

#### Linux/macOS (`build-handbook.sh`)
```bash
#!/bin/bash
set -e

echo "Building Deer Portal Handbook..."

# Clean previous builds
rm -f *.aux *.log *.out *.toc *.fdb_latexmk *.fls *.synctex.gz

# Compile LaTeX (run twice for proper TOC and references)
echo "First compilation pass..."
pdflatex -interaction=nonstopmode handbook.tex

echo "Second compilation pass..."
pdflatex -interaction=nonstopmode handbook.tex

# Clean auxiliary files
rm -f *.aux *.log *.out *.toc *.fdb_latexmk *.fls *.synctex.gz

echo "Handbook generated: handbook.pdf"
```

#### Windows (`build-handbook.bat`)
```batch
@echo off
echo Building Deer Portal Handbook...

rem Clean previous builds
del *.aux *.log *.out *.toc *.fdb_latexmk *.fls *.synctex.gz 2>nul

rem Compile LaTeX
echo First compilation pass...
pdflatex -interaction=nonstopmode handbook.tex

echo Second compilation pass...
pdflatex -interaction=nonstopmode handbook.tex

rem Clean auxiliary files
del *.aux *.log *.out *.toc *.fdb_latexmk *.fls *.synctex.gz 2>nul

echo Handbook generated: handbook.pdf
```

## Integration with Build System

### CMake Integration

Add to `CMakeLists.txt`:

```cmake
# Find LaTeX
find_package(LATEX COMPONENTS PDFLATEX)

if(LATEX_PDFLATEX_FOUND)
    # Custom target for handbook
    add_custom_target(handbook
        COMMAND ${PDFLATEX_COMPILER} ${CMAKE_SOURCE_DIR}/handbook.tex
        COMMAND ${PDFLATEX_COMPILER} ${CMAKE_SOURCE_DIR}/handbook.tex
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Generating handbook PDF"
        SOURCES handbook.tex
    )
    
    # Optional: Add to main build
    # add_dependencies(deerportal handbook)
endif()
```

Then build with:
```bash
mkdir -p build && cd build
cmake ..
make handbook
```

### GitHub Actions (CI/CD)

Add to `.github/workflows/handbook.yml`:

```yaml
name: Build Handbook

on:
  push:
    paths:
      - 'handbook.tex'
      - 'HANDBOOK.md'
      - '.github/workflows/handbook.yml'
  pull_request:
    paths:
      - 'handbook.tex'
      - 'HANDBOOK.md'

jobs:
  build-handbook:
    runs-on: ubuntu-latest
    
    steps:
    - uses: actions/checkout@v3
    
    - name: Install LaTeX
      run: |
        sudo apt-get update
        sudo apt-get install texlive-latex-base texlive-fonts-recommended texlive-latex-extra
    
    - name: Build Handbook PDF
      run: |
        pdflatex -interaction=nonstopmode handbook.tex
        pdflatex -interaction=nonstopmode handbook.tex
    
    - name: Verify Markdown Sync
      run: |
        echo "Checking if HANDBOOK.md is up to date with handbook.tex"
        # Add manual verification or automated checks here
    
    - name: Upload PDF
      uses: actions/upload-artifact@v3
      with:
        name: deer-portal-handbook
        path: handbook.pdf
    
    - name: Upload Markdown
      uses: actions/upload-artifact@v3
      with:
        name: deer-portal-handbook-md
        path: HANDBOOK.md
```

## Troubleshooting

### Common Issues

1. **Missing Packages**
   ```bash
   # Install missing package (TeX Live)
   sudo tlmgr install <package-name>
   
   # Update all packages
   sudo tlmgr update --all
   ```

2. **Font Issues**
   ```bash
   # Rebuild font cache
   sudo fc-cache -fv
   ```

3. **Compilation Errors**
   - Check the `.log` file for detailed error messages
   - Ensure all required packages are installed
   - Verify LaTeX syntax in `handbook.tex`

### Output Files

After successful compilation:
- `handbook.pdf` - The final PDF document
- `HANDBOOK.md` - Player-focused Markdown version
- `handbook.aux` - Auxiliary file (can be deleted)
- `handbook.log` - Compilation log
- `handbook.out` - Hyperref output (can be deleted)
- `handbook.toc` - Table of contents data (can be deleted)

## Maintenance Workflow

### When Updating Game Rules

1. **Update LaTeX First**: Modify `handbook.tex` with new game information
2. **Generate PDF**: Run `pdflatex handbook.tex` twice
3. **Update Markdown**: Manually sync changes to `HANDBOOK.md`
4. **Remove Technical Content**: Ensure no developer details in either version
5. **Test Both Versions**: Verify content consistency
6. **Commit Both Files**: Keep LaTeX and Markdown in sync

### Content Guidelines

#### Include in Player Handbook:
- Game setup and rules
- Card effects and strategy
- Victory conditions
- Turn structure
- Tips and advanced strategy

#### Exclude from Player Handbook:
- Source code examples
- Implementation details
- Asset file paths
- Developer technical information
- Build instructions

## Distribution

### Release Integration
```bash
# Include both formats in release script
cp handbook.pdf releases/deer-portal-v${VERSION}-handbook.pdf
cp HANDBOOK.md releases/deer-portal-v${VERSION}-handbook.md
```

### Documentation Website
- Upload PDF to project website
- Display Markdown on GitHub/documentation site
- Include in downloadable assets
- Embed in online documentation

## Continuous Integration

For automated handbook building on code changes:

1. **Local Development**: Use file watchers to rebuild on changes
2. **CI/CD**: Build handbook in GitHub Actions/GitLab CI
3. **Documentation Sites**: Deploy both PDF and Markdown
4. **Release Process**: Include both formats in release artifacts
5. **Sync Verification**: Check that both versions contain same game rules

### Version Control
- Track both `handbook.tex` and `HANDBOOK.md` in Git
- Ignore generated files (`*.pdf`, `*.aux`, etc.) 
- Tag handbook versions with game releases
- Maintain content parity between formats 