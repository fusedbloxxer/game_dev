#include "stdafx.h"
#include "LightFactory.h"
#include "AmbientLight.h"
#include "DirectionalLight.h"

std::shared_ptr<Light> LightFactory::newInstance(const char* lightType, const GLint id, const Vector3& diff, const Vector3& sp, const GLfloat spow, const Vector3& dir, const Vector3& pos, const GLint aObj)
{
	if (strcmp(lightType, "point") == 0)
	{
		return nullptr;
	} 
	else if (strcmp(lightType, "directional") == 0)
	{
		return std::make_shared<DirectionalLight>(id, diff, sp, spow, dir);
	}
	else
	{
		throw std::invalid_argument{ "Light type is invalid." };
	}
}

std::shared_ptr<Light> LightFactory::newInstance(const char* lightType, const GLint id, const Vector3& diff, const Vector3& sp, const GLfloat spow, const Vector3& pos, const GLint aObj)
{
	if (strcmp(lightType, "point") == 0)
	{
		return nullptr;
	}
	else
	{
		throw std::invalid_argument{ "Light type is invalid." };
	}
}

std::shared_ptr<Light> LightFactory::newInstance(const char* lightType, const GLint id, const Vector3& diff, const Vector3& sp, const GLfloat spow, const Vector3& dir)
{
	if (strcmp(lightType, "directional") == 0)
	{
		return std::make_shared<DirectionalLight>(id, diff, sp, spow, dir);
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