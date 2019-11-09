#pragma once
#include "NFG.h"
#include <vector>

class Parser
{
public:
	static std::pair<std::vector<NFG>, std::vector<pos>> parseFile(const char* filePath);
};

