//readFile.cpp

#include "readFile.h"

#include <fstream>
#include <sstream>

std::string readFromFile(const std::string &filePath)
{
	std::ostringstream readText;
	std::ifstream fileStream(filePath, std::ios::in);
	if (fileStream.is_open())
	{
		std::string Line;
		while (getline(fileStream, Line))
			readText << Line << std::endl;
		fileStream.close();
	}

	return readText.str();
}