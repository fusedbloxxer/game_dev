#pragma once
#include "Resource.h"

struct SoundResource : public Resource
{
	std::string file;
	
	SoundResource(const GLuint id, const std::string& file);
	
	virtual ~SoundResource() = default;
};