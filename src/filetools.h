#ifndef FILETOOLS_H
#define FILETOOLS_H

#include <iostream>
#include <string>

static std::string get_full_path(std::string path)
{
	std::string new_path;
	if ( getenv ( "DP_DIR" ) != NULL )
	{
		std::cout << "PATH " << new_path;
		new_path = std::string(getenv("DP_DIR")) + path ;
		std::cout << "NEW_PATH " << new_path << "\n";
		return new_path;
	}
		std::cout << "PATH " << path << "\n";

	return path;
}
#endif // FILETOOLS_H
