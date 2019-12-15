#pragma once
#include "..\Utilities\rapidxml-1.13\rapidxml.hpp"
#include <memory>
#include <string>

class Rapid
{
	std::string content;

	std::unique_ptr<rapidxml::xml_document<>> xmlParser;

protected:
	// First node from the file
	rapidxml::xml_node<>* root;

	// Contructor
	Rapid(const char* filePath);

public:
	// Virtual destructor
	virtual ~Rapid() = default;
};