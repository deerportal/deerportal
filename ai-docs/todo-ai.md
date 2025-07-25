# TODO List - Post Version 0.9.5 Development

## Current Version: 0.9.7-pre.2

### Documentation Update Status - July 2025
- **Code Analysis**: Complete - Source structure analyzed
- **Architecture Changes**: Documented - Particle system removal, new utility systems
- **File Structure**: Updated - Reflects current ~70 files with modular organization
- **Version Sync**: Updated - Documentation aligned with 0.9.7-pre.2
- **Particle System**: Documented - Enhanced visual effects with doubled particle counts
- **Linux Compatibility**: Documented - Asset path resolution for AppImage and tar.gz distributions

## Completed in 0.9.5
- [x] Fixed game state reset bug when returning from menu (Escape→Menu→Play)
- [x] Applied LLVM code formatting to entire codebase
- [x] Updated version management across all packaging files
- [x] Released stable version 0.9.5 "Code Formatting and Quality"
- [x] Implemented animated board initialization system as per FINAL-plan.md

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