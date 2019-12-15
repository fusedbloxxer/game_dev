#include "stdafx.h"
#include "Rapid.h"
#include <sstream>
#include <fstream>

Rapid::Rapid(const char* filePath)
	:xmlParser{ std::make_unique<rapidxml::xml_document<>>() }
{
	std::ifstream input{ filePath };

	if (!input.is_open()) {
		throw std::runtime_error{ "Could not open file: " + std::string(filePath) };
	}

	std::stringstream ss; ss << input.rdbuf();
	input.close(); content = ss.str();
	xmlParser->parse<0>(&content[0]);
	root = xmlParser->first_node();
}