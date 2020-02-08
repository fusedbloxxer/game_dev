#pragma once
#include "NormalLight.h"

class LightFactory
{
	LightFactory() = delete;
	LightFactory(const LightFactory&) = delete;
	LightFactory(LightFactory&&) noexcept = delete;
	LightFactory& operator=(const LightFactory&) = delete;
	LightFactory& operator=(LightFactory&&) noexcept = delete;

public:
	static std::shared_ptr<Light> newInstance(const char* lightType, const GLint id, const Vector3& diff, const Vector3& sp, const GLfloat spow, const Vector3& dir, const Vector3& pos, const GLint aObj);
	static std::shared_ptr<Light> newInstance(const char* lightType, const GLint id, const Vector3& diff, const Vector3& sp, const GLfloat spow, const Vector3& pos, const GLint aObj);
	static std::shared_ptr<Light> newInstance(const char* lightType, const GLint id, const Vector3& diff, const Vector3& sp, const GLfloat spow, const Vector3& dir);
	static std::shared_ptr<Light> newInstance(const char* lightType, const Vector3& color = { 0.0f, 0.0f, 0.0f }, const GLfloat ratio = 0.0f);
};