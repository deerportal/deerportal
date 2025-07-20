# Linux Asset Path Resolution Implementation

## Overview

Cross-platform asset loading system for Linux distributions supporting both AppImage and standard tar.gz installation formats.

## Implementation Details

### File: `src/filetools.h`

The `get_full_path()` function implements multi-platform asset path resolution with the following hierarchy:

1. **Environment Variable Override**: `DP_DIR` - Manual path specification
2. **AppImage Support**: `APPDIR` environment variable detection
3. **Linux Binary Detection**: `/proc/self/exe` executable path analysis
4. **macOS App Bundle**: `.app/Contents/Resources/` path resolution
5. **Fallback**: Relative path from working directory

### Linux-Specific Implementation

```cpp
#ifdef __linux__
  // Get executable directory
  char exec_path[1024];
  ssize_t count = readlink("/proc/self/exe", exec_path, sizeof(exec_path) - 1);
  if (count != -1) {
    exec_path[count] = '\0';
    std::string exec_dir = std::string(exec_path);
    
    // Remove executable name to get directory
    size_t pos = exec_dir.find_last_of('/');
    if (pos != std::string::npos) {
      exec_dir = exec_dir.substr(0, pos);
      
      // Check if we're in /usr/bin or similar - look for ../share/games/deerportal/
      std::string assets_path = exec_dir + "/../share/games/deerportal/" + path;
      
      // Verify the assets directory exists
      struct stat st;
      std::string assets_dir = exec_dir + "/../share/games/deerportal/assets";
      if (stat(assets_dir.c_str(), &st) == 0 && S_ISDIR(st.st_mode)) {
        return assets_path;
      }
    }
  }
#endif
```

### AppImage Support

AppImage environments are detected via the `APPDIR` environment variable:

```cpp
if (getenv("APPDIR") != NULL) {
  std::string appdir = std::string(getenv("APPDIR"));
  // For AppImage, assets should be in usr/share/games/deerportal/
  new_path = appdir + "/usr/share/games/deerportal/" + path;
  return new_path;
}
```

### CI/CD Integration

The following CI/CD workflows were updated to install assets in the correct location:

- `.github/workflows/cmake.yml`
- `.github/workflows/test-release.yml` 
- `.github/workflows/release.yml`

Asset installation path changed from:
```bash
mkdir -p DeerPortal.AppDir/usr/share/deerportal/
cp -r ../assets DeerPortal.AppDir/usr/share/deerportal/
```

To:
```bash
mkdir -p DeerPortal.AppDir/usr/share/games/deerportal/
cp -r ../assets DeerPortal.AppDir/usr/share/games/deerportal/
```

## Platform Compatibility

### macOS
- **App Bundle**: `DeerPortal.app/Contents/Resources/assets/`
- **Detection**: `_NSGetExecutablePath()` and `.app/Contents/MacOS` path analysis

### Linux
- **Standard Installation**: `/usr/share/games/deerportal/assets/`
- **AppImage**: `$APPDIR/usr/share/games/deerportal/assets/`
- **Detection**: `/proc/self/exe` readlink and stat() verification

### Windows
- **Installation**: `./assets/` (relative to executable)
- **Detection**: Fallback to relative path

## Dependencies

### Linux-specific includes:
```cpp
#ifdef __linux__
#include <unistd.h>
#include <sys/stat.h>
#endif
```

### Functions used:
- `readlink()` - Resolve executable path via `/proc/self/exe`
- `stat()` - Verify directory existence
- `S_ISDIR()` - Check if path is directory

## Error Handling

The implementation uses graceful fallbacks:
1. If Linux detection fails, fall through to platform-specific logic
2. If stat() verification fails, continue to next resolution method  
3. Ultimate fallback to original relative path

## Testing

Verification requires building and testing actual Linux packages:
- **AppImage**: Test with generated `.AppImage` file
- **Tar.gz**: Extract and test from installation directory structure
- **Directory structure**: Ensure `/usr/share/games/deerportal/assets/` exists

## Version History

- **v0.9.6**: Initial implementation with Linux asset path resolution
- **v0.9.7-pre.2**: Enhanced with improved error handling and documentation