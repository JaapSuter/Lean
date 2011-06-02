// Copyright 2005, Jaap Suter - MIT License

#include "lean/pch.hpp"
#include "lean/read.hpp"
#include <fstream>

namespace lean {

std::string read(const char* filename)
{
	std::ifstream f(filename);

	std::string ret;
	std::string line;
	
	while (std::getline(f, line))
	{
		ret += line;
		ret += "\n";		
	}
	
	return ret;
}

}
