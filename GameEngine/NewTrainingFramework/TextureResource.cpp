#include "stdafx.h"
#include "TextureResource.h"
#include <iostream>

TextureResource::TextureResource(GLuint id, std::string type, std::string file, std::string min, std::string mag, std::string ws, std::string wt)
	: Resource{ id },
	wrapS( ws == "GL_REPEAT" ? GL_REPEAT : ws == "GL_CLAMP_TO_EDGE" ? GL_CLAMP_TO_EDGE : GL_MIRRORED_REPEAT ),
	wrapT( wt == "GL_REPEAT" ? GL_REPEAT : wt == "GL_CLAMP_TO_EDGE" ? GL_CLAMP_TO_EDGE : GL_MIRRORED_REPEAT ),
	minFilter( min == "LINEAR" ? GL_LINEAR : GL_NEAREST ),
	magFilter( mag == "LINEAR" ? GL_LINEAR : GL_NEAREST ),
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