# AI Documentation Directory

This directory contains comprehensive documentation for AI assistants working on the DeerPortal project. The documentation is organized to provide contextual information for different types of development tasks.

## 📋 **Quick Navigation**

### **🎯 Start Here**
- [`ai-instruction.md`](ai-instruction.md) - **Main project overview, status, and development guidelines**
- [`cursor-memory-bank.md`](cursor-memory-bank.md) - **Active memory bank for ongoing work**

### **📚 Current Files**
- `ai-gamelogic.md` (16KB) - Game mechanics and logic documentation
- `ai-performance.md` (39KB) - Performance analysis and optimization plans
- `ai-todo.md` (16KB) - TODO items and modernization tasks
- `ai-upgrade-to-8.2.md` (17KB) - Version upgrade documentation  
- `ai-formatting.md` (7KB) - Code formatting guidelines
- `ai-modularization-plan.md` (12KB) - Code structure plans

## 🎯 **How to Use This Documentation**

### **For New AI Assistants:**
1. **Start with** `ai-instruction.md` for project overview
2. **Check** `cursor-memory-bank.md` for current status
3. **Load specific context** based on your task:
   - Performance work → `ai-performance.md`
   - Code improvements → `ai-todo.md` 
   - Game logic → `ai-gamelogic.md`

### **For Specific Tasks:**
- **SFML Migration**: `cursor-memory-bank.md` + `ai-upgrade-to-8.2.md`
- **Performance Optimization**: `ai-performance.md` + `ai-todo.md`
- **Code Quality**: `ai-formatting.md` + `ai-modularization-plan.md`
- **Game Development**: `ai-gamelogic.md` + `ai-instruction.md`

## 🔄 **Maintenance Notes**

### **Always Update:**
- `RELEASE_NOTES.md` when adding features
- `.gitignore` when adding new generated files
- Version numbers across project files
- Contact information and dates

### **File Ownership:**
- `ai-instruction.md` - Project status and guidelines
- `cursor-memory-bank.md` - Active migration memory
- `ai-performance.md` - Performance analysis (Sonnet)
- `ai-todo.md` - Task management
- Other files - Historical development context

## 📈 **Proposed Restructure**

See [`ai-docs-restructure-plan.md`](ai-docs-restructure-plan.md) for detailed analysis and recommendations for better modular organization of this documentation.

**Key Issues Identified:**
- Large files (39KB) difficult for AI context loading
- Content overlap between files
- Related information scattered across multiple files

**Proposed Solution:**
- Split into focused, smaller files organized by topic
- Create subdirectories for different contexts
- Reduce duplication and improve cross-referencing

## 🎮 **Project Context**

**DeerPortal** is a strategic board game with SFML 3.0 graphics:
- **Status**: SFML 3.0 migration completed (Version 0.9.0 "Welcome Back")
- **Platform**: C++17, CMake, cross-platform (macOS/Linux/Windows)
- **Architecture**: Game engine with AI players, animations, audio system
- **Current Focus**: Performance optimization and code modernization 