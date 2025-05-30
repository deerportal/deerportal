#ifndef FILETOOLS_H
#define FILETOOLS_H

#include <iostream>
#include <string>

#ifdef __APPLE__
#include <mach-o/dyld.h>
#include <libgen.h>
#include <unistd.h>
#endif

static std::string get_full_path(std::string path)
{
    std::string new_path;
    if ( getenv ( "DP_DIR" ) != NULL )
    {
        new_path = std::string(getenv("DP_DIR")) + path ;
        return new_path;
    }
    
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
