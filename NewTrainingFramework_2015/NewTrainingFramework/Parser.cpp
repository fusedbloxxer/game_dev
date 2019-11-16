#include "stdafx.h"
#include "Parser.h"
#include "Parser.h"
#include <fstream>
#include <filesystem>
#include <string>
#include <regex>
#include <sstream>
#include "../Utilities/rapidxml-1.13/rapidxml.hpp"

template<typename Type>
void fillContainer(std::vector<Type>& container, const char* format, std::ifstream& fs, int lines)
{
	Type temp;
	bool ok = false;
	std::smatch matches; std::stringstream ss;
	std::regex pattern{ format }; std::string str;

	while (lines >= 0)
	{
		// Get new line.
		std::getline(fs, str);

		while (std::regex_search(str, matches, pattern))
		{
			if (matches.size() > 0)
			{
				ss << matches[0] << std::string(" ");
			}
			str = matches.suffix().str();
		}

		if (ok)
		{
			ss >> temp;
			container.push_back(temp);
		}
		else
		{
			ok = true;
		}
		ss.clear();

		--lines;
	}
}

template<>
void fillContainer<GLushort>(std::vector<GLushort>& container, const char* format, std::ifstream& fs, int lines)
{
	GLushort temp;
	bool ok = false;
	std::string str;
	std::stringstream ss;

	while (lines >= 0)
	{
		// Get new line.
		std::getline(fs, str);

		std::string const delims{ " .,:;!?" };

		size_t beg, pos = 0; int count = 1;
		GLint x = 0, y = 0, z = 0;

		while ((beg = str.find_first_not_of(delims, pos)) != std::string::npos)
		{
			pos = str.find_first_of(delims, beg + 1);
			std::string tem = str.substr(beg, pos - beg);

			if (count % 4 == 2)
			{
				ss << tem;
				ss >> x;
			}
			else if (count % 4 == 3)
			{
				ss << tem;
				ss >> y;
			}
			else if (count % 4 == 0)
			{
				ss << tem;
				ss >> z;
			}
			++count;
			ss.clear();
		}

		if (ok)
		{
			container.push_back(x);
			container.push_back(y);
			container.push_back(z);
		}
		else
		{
			ok = true;
		}
		--lines;
	}
}

std::pair<std::vector<Vertex_NFG>, std::vector<GLushort>> Parser::parseFile(const char* const filePath)
{
	std::ifstream input{ filePath };

	if (!input.is_open())
	{
		throw std::exception{ "Could not open file." };
	}

	auto pair = std::make_pair<std::vector<Vertex_NFG>, std::vector<GLushort>>(std::vector<Vertex_NFG>(), std::vector<GLushort>());

	std::string str; int lines; input >> str >> lines;
	fillContainer(pair.first, "-{0,1}\\d{1,}\\.\\d{1,}", input, lines);
	input >> str >> lines;
	fillContainer(pair.second, "\\d{1,}", input, lines / 3);
	input.close();

	//std::for_each(pair.first.begin(), pair.second.end(),
		// [](const auto& e) { std::cout << e << std::endl; });
	// std::for_each(pair.second.begin(), pair.second.end(),
		// [](const auto& e) { std::cout << e << std::endl; });
	
	return pair;
}