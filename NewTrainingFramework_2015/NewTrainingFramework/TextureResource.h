#pragma once
#include "Resource.h"
#include <string>

struct TextureResource : public Resource {
	std::string file;
	GLint type, minFilter, magFilter, wrapS, wrapT;
	TextureResource(GLint id, std::string type, std::string file, std::string min, std::string mag, std::string ws, std::string wt);
};