#include "stdafx.h"
#include "ModelResource.h"

ModelResource::ModelResource(GLint id, std::string file)
	:Resource{ id }, file{ file } {}