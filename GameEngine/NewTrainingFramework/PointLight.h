#pragma once
#include "NormalLight.h"

class PointLight : public NormalLight
{
public:
	PointLight(const GLint id, const GLint aObj, const Vector3& diffuseColor = { 0.0f, 0.0f, 0.0f }, const Vector3& specularColor = { 0.0f, 0.0f, 0.0f }, const GLfloat specPower = 0, const Vector3 & direction = { 0.0f, 0.0f, 0.0f });
	PointLight(const GLint id, const Vector3& diffuseColor = { 0.0f, 0.0f, 0.0f }, const Vector3& specularColor = { 0.0f, 0.0f, 0.0f }, const GLfloat specPower = 0, const Vector3& direction = { 0.0f, 0.0f, 0.0f });

	virtual void print(std::ostream& os) const override;

	virtual ~PointLight() = default;
};