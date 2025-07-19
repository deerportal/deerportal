#ifndef FILETOOLS_H
#define FILETOOLS_H

#include <iostream>
#include <string>

#ifdef __APPLE__
#include <libgen.h>
#include <unistd.h>
#include <mach-o/dyld.h>
#endif

#ifdef __linux__
#include <unistd.h>
#include <sys/stat.h>
#endif

static std::string get_full_path(std::string path) {
  std::string new_path;
  if (getenv("DP_DIR") != NULL) {
    new_path = std::string(getenv("DP_DIR")) + path;
    return new_path;
  }

  // Check for AppImage APPDIR environment variable
  if (getenv("APPDIR") != NULL) {
    std::string appdir = std::string(getenv("APPDIR"));
    // For AppImage, assets should be in usr/share/games/deerportal/
    new_path = appdir + "/usr/share/games/deerportal/" + path;
    return new_path;
  }

  // Check if we're running from a Linux installation (tar.gz package)
  // Look for assets in standard Linux location relative to binary
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

#ifdef __APPLE__
  // On macOS, detect if we're running from an app bundle
  char exec_path[1024];
  uint32_t size = sizeof(exec_path);
  if (_NSGetExecutablePath(exec_path, &size) == 0) {
    // Convert to absolute path
    char* abs_path = realpath(exec_path, nullptr);
    if (abs_path) {
      std::string exec_dir = std::string(abs_path);
      free(abs_path);

      // Check if we're in an app bundle (path contains .app/Contents/MacOS)
      if (exec_dir.find(".app/Contents/MacOS") != std::string::npos) {
        // Extract bundle path and point to Resources
        size_t pos = exec_dir.find(".app/Contents/MacOS");
        std::string bundle_path = exec_dir.substr(0, pos + 4); // include .app
        new_path = bundle_path + "/Contents/Resources/" + path;
        return new_path;
      }
    }
  }
#endif

  return path;
}
#endif // FILETOOLS_H
