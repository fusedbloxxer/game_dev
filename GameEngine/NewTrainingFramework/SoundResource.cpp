#include "stdafx.h"
#include "SoundResource.h"

SoundResource::SoundResource(const GLuint id, const std::string& file)
	:Resource{ id }, file{ file } {}
