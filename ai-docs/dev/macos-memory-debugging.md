# macOS Memory Debugging Guide

## 🎯 **Overview**
Since valgrind doesn't work well on macOS, this guide covers the best alternatives for memory debugging and profiling C++ applications on macOS.

## 🔧 **Tool Comparison**

| Tool | Best For | Ease of Use | Performance Impact |
|------|----------|-------------|-------------------|
| **AddressSanitizer** | Memory errors, leaks | ⭐⭐⭐⭐⭐ | Medium |
| **Xcode Instruments** | Comprehensive profiling | ⭐⭐⭐ | Low |
| **Static Analysis** | Code quality | ⭐⭐⭐⭐ | None |
| **Guard Malloc** | Buffer overflows | ⭐⭐ | High |

## 1. 🥇 **AddressSanitizer (ASan) - Recommended**

### **What it detects:**
- Buffer overflows/underflows
- Use-after-free
- Double-free
- Memory leaks
- Use of uninitialized memory

### **How to use with DeerPortal:**

```bash
# Build with AddressSanitizer
make clean
cmake -DCMAKE_BUILD_TYPE=Debug \
      -DCMAKE_CXX_FLAGS="-fsanitize=address -fsanitize=undefined -fno-omit-frame-pointer -g" .
make -j4

# Run with basic memory error detection (leak detection often unsupported on newer macOS)
ASAN_OPTIONS=abort_on_error=0:halt_on_error=0 \
./DeerPortal.app/Contents/MacOS/DeerPortal

# Alternative: Use macOS MallocStackLogging for leak detection
export MallocStackLogging=1
export MallocStackLoggingNoCompact=1
./DeerPortal.app/Contents/MacOS/DeerPortal
# Then use: leaks PID or heap PID

# For CI/automated testing (without leak detection)
ASAN_OPTIONS=abort_on_error=1:halt_on_error=1 \
./DeerPortal.app/Contents/MacOS/DeerPortal
```

### **⚠️ Known Limitations on macOS:**
- **Leak detection**: Often unsupported on ARM Macs and newer macOS versions
- **Workaround**: Use `MallocStackLogging` + `leaks` command instead
- **Still detects**: Buffer overflows, use-after-free, double-free, initialization issues

### **Interpreting Results:**
- **No output**: ✅ No memory issues detected
- **Stack traces**: 🚨 Memory errors found - fix immediately
- **Leak report**: ⚠️ Memory leaks detected - investigate

### **Example Output:**
```
=================================================================
==12345==ERROR: AddressSanitizer: heap-buffer-overflow on address 0x...
READ of size 4 at 0x... thread T0:
    #0 0x... in SomeFunction src/somefile.cpp:123
    #1 0x... in main src/main.cpp:45
```

## 2. 🎨 **Xcode Instruments**

### **Setup:**
1. Open Instruments.app (comes with Xcode)
2. Choose template:
   - **Leaks**: Memory leak detection
   - **Allocations**: Memory usage patterns
   - **Time Profiler**: CPU performance

### **Usage:**
```bash
# Open Instruments from command line
open -a Instruments

# Or launch directly with leaks template
instruments -t "Leaks" DeerPortal.app
```

### **Best Practices:**
- Run for 5-10 minutes of gameplay
- Exercise different game modes
- Look for growth patterns in memory usage

## 3. 🔍 **Static Analysis**

### **Clang Static Analyzer:**
```bash
# Analyze during build
scan-build cmake -DCMAKE_BUILD_TYPE=Debug .
scan-build make

# View results
scan-view /tmp/scan-build-*
```

### **Cppcheck:**
```bash
# Basic analysis
cppcheck --enable=all --std=c++17 src/

# Focused on memory issues
cppcheck --enable=warning,style,performance --std=c++17 \
         --suppress=missingIncludeSystem src/
```

## 4. 🛡️ **Guard Malloc (Built-in macOS)**

### **Usage:**
```bash
# Enable Guard Malloc
export MallocGuardEdges=1
export MallocScribble=1
export MallocCheckHeapStart=1
export MallocCheckHeapEach=100

./DeerPortal.app/Contents/MacOS/DeerPortal
```

### **What it does:**
- Fills freed memory with 0x55
- Detects buffer overruns
- High performance impact - use for short tests

## 🚀 **DeerPortal-Specific Testing Strategy**

### **Quick Memory Test (5 minutes):**
```bash
# 1. Build with AddressSanitizer
make clean && cmake -DCMAKE_BUILD_TYPE=Debug \
-DCMAKE_CXX_FLAGS="-fsanitize=address -g" . && make

# 2. Run basic test
ASAN_OPTIONS=detect_leaks=1 ./DeerPortal.app/Contents/MacOS/DeerPortal
# - Start game
# - Play a few rounds
# - Exit cleanly
```

### **Comprehensive Memory Analysis (30 minutes):**
```bash
# 1. AddressSanitizer deep test
ASAN_OPTIONS=detect_leaks=1:check_initialization_order=1:strict_init_order=1 \
./DeerPortal.app/Contents/MacOS/DeerPortal

# 2. Instruments profiling
instruments -t "Leaks" -D instruments_trace DeerPortal.app

# 3. Static analysis
cppcheck --enable=all --xml src/ 2> cppcheck_results.xml
```

## 📊 **CI/Automated Testing**

### **GitHub Actions Example:**
```yaml
- name: Memory Testing
  run: |
    cmake -DCMAKE_BUILD_TYPE=Debug \
          -DCMAKE_CXX_FLAGS="-fsanitize=address -fsanitize=undefined" .
    make
    ASAN_OPTIONS=detect_leaks=1:abort_on_error=1 \
    timeout 300 ./DeerPortal.app/Contents/MacOS/DeerPortal || true
```

## ⚠️ **Common Issues & Solutions**

### **AddressSanitizer Issues:**
- **Slow performance**: Normal, use only for testing
- **False positives**: Rare, usually legitimate issues
- **SFML conflicts**: Use `ASAN_OPTIONS=detect_odr_violation=0` if needed

### **Instruments Issues:**
- **Requires Xcode**: Install Xcode Command Line Tools
- **Permissions**: Run with `sudo` if needed
- **Large traces**: Use shorter test sessions

## 🎯 **Recommended Workflow for DeerPortal**

### **Daily Development:**
1. **Quick ASan test**: Before each commit
2. **Static analysis**: Weekly with cppcheck
3. **Instruments**: Before releases

### **Pre-Release Testing:**
1. **Full ASan suite**: All game modes
2. **Instruments leaks**: 30+ minute session
3. **Performance profiling**: Frame rate analysis

### **Pre-Merge Checklist:**
- [ ] ASan clean run (no errors/leaks)
- [ ] Basic Instruments leaks test
- [ ] Cppcheck analysis reviewed

---

**Bottom Line**: AddressSanitizer is your best bet for day-to-day memory debugging on macOS. It's built into clang, easy to use, and catches most issues that valgrind would find on Linux. 