#pragma once
#include "Light.h"
#include <string>

class NormalLight : public Light
{
protected:
	Vector3 specularColor, diffuseColor;
	GLfloat specPower;
	GLint id;

	NormalLight(const GLint id, const Vector3& diffuseColor = { 0.0f, 0.0f, 0.0f }, const Vector3& specularColor = { 0.0f, 0.0f, 0.0f }, const GLfloat specPower = 0);

public:

	virtual void print(std::ostream& os) const override;

	// Getters and setters

	const Vector3& getSpecularColor() const;
	void setSpecularcolor(const Vector3& specularColor);

	const Vector3& getDiffuseColor() const;
	void setDiffuseColor(const Vector3& diffuseColor);

	GLfloat getSpecPower() const;
	void setSpecPower(const GLfloat specPower);

	const GLint getId() const;
	void setId(const GLint id);

	virtual ~NormalLight() = 0;
};

