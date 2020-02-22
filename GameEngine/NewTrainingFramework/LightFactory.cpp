#include "stdafx.h"
#include "SpotLight.h"
#include "PointLight.h"
#include "LightFactory.h"
#include "AmbientLight.h"
#include "DirectionalLight.h"

std::shared_ptr<Light> LightFactory::newInstance(const char* lightType, const GLint id, const Vector3& diff, const Vector3& sp, const GLfloat spow, const Vector3& dir, const GLint aObj)
{
	if (strcmp(lightType, "point") == 0)
	{
		return std::make_shared<PointLight>(id, aObj, diff, sp, spow, dir);
	} 
	else if (strcmp(lightType, "directional") == 0)
	{
		return std::make_shared<DirectionalLight>(id, diff, sp, spow, dir);
	}
	else if (strcmp(lightType, "spotlight") == 0)
	{
		return std::make_shared<SpotLight>(id, aObj, diff, sp, spow, dir);
	}
	else
	{
		throw std::invalid_argument{ "Light type is invalid." };
	}
}

std::shared_ptr<Light> LightFactory::newInstance(const char* lightType, const Vector3& color, const GLfloat ratio)
{
	if (strcmp(lightType, "ambientalLight") == 0)
	{
		return std::make_shared<AmbientLight>(color, ratio);
	}
	else
	{
		throw std::invalid_argument{ "Light type is invalid." };
	}
}