#include "utils.h"
#include <fstream>
#include <iostream>

std::string fileToString(const char *filename)
{
	std::ifstream fs(filename);
	if (!fs.is_open())
	{
		std::cerr << "Unable to open shader " << filename << '.'
		  << std::endl;
		return 0; // invalid id
	}

	std::string fileString;
	std::string line;
	while (std::getline(fs, line))
	{
		fileString += line;
		fileString += '\n';
	}

	return fileString;
}

