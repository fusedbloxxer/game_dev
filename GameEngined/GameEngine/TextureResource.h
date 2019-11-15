#pragma once
#include <string>

struct TextureResource {
	int id;
	std::string type, file, minFilter, magFilter, wrapS, wrapT;
	TextureResource(int id, std::string type, std::string file, std::string min, std::string mag, std::string ws, std::string wt);
};
