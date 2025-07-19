# TODO List - Post Version 0.9.5 Development

## Completed in 0.9.5
- [x] Fixed game state reset bug when returning from menu (Escape→Menu→Play)
- [x] Applied LLVM code formatting to entire codebase
- [x] Updated version management across all packaging files
- [x] Released stable version 0.9.5 "Code Formatting and Quality"

## Current Development Priorities

### Fullscreen Feature Implementation
- [ ] Implement fullscreen toggle functionality with F key (see ai-docs/fullscreen-implementation-plan.md)
- [ ] Create window-manager module for handling fullscreen transitions
- [ ] Add configuration persistence for fullscreen preference
- [ ] Test fullscreen on Windows, macOS, and Linux platforms
- [ ] Update documentation with fullscreen controls

### Platform Improvements
- [ ] Test multiple solutions for PowerShell execution policy workarounds
- [ ] Consider adding Windows-specific execution instructions to README
- [ ] Verify grid reveal intro shader functionality in Windows environment

### Code Quality Maintenance
- [ ] Monitor code quality metrics post-formatting implementation
- [ ] Continue memory management best practices
- [ ] Maintain LLVM formatting standards in new code

### Future Enhancements
- [ ] Consider adding "Resume Game" vs "New Game" options to menu
- [ ] Implement proper save/load game state functionality
- [ ] Add confirmation dialog for abandoning current game 