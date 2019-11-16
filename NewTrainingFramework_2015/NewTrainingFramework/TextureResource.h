#pragma once
#include "..\\Utilities\utilities.h"
#include <string>
#include "stdafx.h"

struct TextureResource {
	std::string file;
	GLint id, type, minFilter, magFilter, wrapS, wrapT;
	TextureResource(GLint id, std::string type, std::string file, std::string min, std::string mag, std::string ws, std::string wt);
};