#include "stdafx.h"
#include "TextureResource.h"
#include <iostream>

TextureResource::TextureResource(GLuint id, std::string type, std::string file, std::string min, std::string mag, std::string ws, std::string wt)
	: Resource{ id },
	wrapS(ws == "REPEAT" ? GL_REPEAT
		: ws == "CLAMP_TO_EDGE" ? GL_CLAMP_TO_EDGE
		: wt == "MIRRORED_REPEAT" ? GL_MIRRORED_REPEAT : throw std::runtime_error("Invalid wt value in texture resource")),
	
	wrapT(wt == "REPEAT" ? GL_REPEAT
		: wt == "CLAMP_TO_EDGE" ? GL_CLAMP_TO_EDGE
		: wt == "MIRRORED_REPEAT" ? GL_MIRRORED_REPEAT : throw std::runtime_error("Invalid wt value in texture resource")),
	
	minFilter(min == "LINEAR" ? GL_LINEAR
		: min == "NEAREST" ? GL_NEAREST : throw std::runtime_error("Invalid minFilter.")),
	
	magFilter(mag == "LINEAR" ? GL_LINEAR
		: min == "NEAREST" ? GL_NEAREST : throw std::runtime_error("Invalid minFilter.")),
	
	file{ file }
{
	if (type == "2d") {
		this->type = GL_TEXTURE_2D;
	}
	else if (type == "cube") {
		this->type = GL_TEXTURE_CUBE_MAP;
	}
	else {
		throw std::runtime_error{ "Invalid Texture Type" };
	}
}