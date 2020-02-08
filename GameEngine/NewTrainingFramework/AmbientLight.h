#pragma once
#include "NormalLight.h"

class AmbientLight : public Light
{
	Vector3 color;
	GLfloat ratio;

public:
	AmbientLight(const Vector3& color = { 0.0f, 0.0f, 0.0f }, const GLfloat ratio = 1.0f);

	virtual void print(std::ostream& os) const override;

	virtual ~AmbientLight() = default;

	// Getters and setters
	Vector3 getColor() const;
	void setColor(const Vector3& color);

	GLfloat getRatio() const;
	void setRatio(GLfloat ratio);
};

