#include "stdafx.h"
#include "../Utilities/rapidxml-1.13/rapidxml.hpp"
#include "Parser.h"
#include "Parser.h"
#include <filesystem>
#include <sstream>
#include <fstream>
#include <string>
#include <regex>

std::pair<std::vector<Vertex_NFG>, std::vector<GLushort>> Parser::parseFile(const char* const filePath)
{
	std::ifstream input{ filePath };

	if (!input.is_open())
	{
		throw std::exception{ "Could not open file." };
	}

	auto pair = std::make_pair<std::vector<Vertex_NFG>, std::vector<GLushort>>(std::vector<Vertex_NFG>(), std::vector<GLushort>());

	std::string str; int lines; input >> str >> lines; 
	GLchar ch; input.get(ch); // Read new line
	
	Vertex_NFG v;
	for (GLint i = 0; i < lines; ++i)
	{
		input >> v;
		pair.first.push_back(v);
	}

	input >> str >> lines; input.get(ch); // Read new line
	for (GLint i = 0, j = 0; i < lines / 3; ++i, j = 0)
	{
		std::getline(input, str);
		str = str.substr(str.find('.') + 1);
		GLchar* buff = new GLchar[str.size() + 1];
		strcpy_s(buff, str.size() + 1, str.c_str());
		str.clear();

		find_num(buff, j);
		pair.second.push_back(static_cast<GLushort>(extr_num(buff, j)));
		find_num(buff, j);
		pair.second.push_back(static_cast<GLushort>(extr_num(buff, j)));
		find_num(buff, j);
		pair.second.push_back(static_cast<GLushort>(extr_num(buff, j)));

		delete[] buff;
	}

	/*fillContainer(pair.first, "-{0,1}\\d{1,}\\.\\d{1,}", input, lines);
	input >> str >> lines;
	fillContainer(pair.second, "\\d{1,}", input, lines / 3);*/

	//std::for_each(pair.first.begin(), pair.second.end(),
		// [](const auto& e) { std::cout << e << std::endl; });
	// std::for_each(pair.second.begin(), pair.second.end(),
		// [](const auto& e) { std::cout << e << std::endl; });

	input.close();
	// exit(0);

	return pair;
}