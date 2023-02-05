#pragma warning( disable : 4996 )
#ifndef FILETOOLS_H
#define FILETOOLS_H

#include <iostream>
#include <string>
static std::string get_full_path(std::string path)
{
    std::string new_path;
    if ( getenv ( "DP_DIR" ) != NULL ) 
    {
        new_path = std::string(getenv("DP_DIR")) + path ;
        return new_path;
    }
    return path;
}
#endif // FILETOOLS_H
