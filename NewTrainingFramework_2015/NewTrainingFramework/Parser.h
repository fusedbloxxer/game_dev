#pragma once
#include "Vertex_NFG.h"
#include <vector>

class Parser
{
public:
	static std::pair<std::vector<Vertex_NFG>, std::vector<GLushort>> parseFile(const char* filePath);
	static void xmlParse(const char* filePath);
};

