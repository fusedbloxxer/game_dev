#pragma once
#include "Resource.h"
#include <string>

struct TextureResource : public Resource {
	std::string file;
	GLuint type, minFilter, magFilter, wrapS, wrapT;
	TextureResource(GLuint id, std::string type, std::string file, std::string min, std::string mag, std::string ws, std::string wt);
};