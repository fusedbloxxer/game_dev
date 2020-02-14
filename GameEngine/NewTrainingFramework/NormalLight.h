#pragma once
#include "Light.h"
#include <string>

class NormalLight : public Light
{
protected:
	inline static constexpr GLuint DIRECTIONAL_LIGHT = 1;
	inline static constexpr GLuint POINT_LIGHT = 2;
	inline static constexpr GLuint SPOT_LIGHT = 3;

	Vector3 specularColor, diffuseColor;
	Vector3 direction;
	GLfloat specPower;
	GLuint lightType;
	GLint id;

	NormalLight(const GLuint lightType, const GLint id, const Vector3& diffuseColor = { 0.0f, 0.0f, 0.0f }, const Vector3& specularColor = { 0.0f, 0.0f, 0.0f }, const GLfloat specPower = 0, const Vector3 & direction = { 0.0f, 0.0f, 0.0f });

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

	const GLint getLightType() const;

	const Vector3& getDirection() const;
	void setDirection(const Vector3& direction);

	virtual ~NormalLight() = 0;
};

