#pragma once
#include "NormalLight.h"

class SpotLight : public NormalLight
{
	GLfloat innerCutoff = 0.9f;
	GLfloat outerCutoff = 0.81f;

public:
	SpotLight(const GLint id, const GLint aObj, const Vector3& diffuseColor = { 0.0f, 0.0f, 0.0f }, const Vector3& specularColor = { 0.0f, 0.0f, 0.0f }, const GLfloat specPower = 0, const Vector3 & direction = { 0.0f, 0.0f, 0.0f }, const GLfloat in = 0.9, const GLfloat out = 0.81);
	
	virtual void print(std::ostream& os) const override;

	// Getters and setters
	const GLfloat getInnerCutoff() const;
	void setInnerCutoff(const GLfloat innerCutoff);

	const GLfloat getOuterCutoff() const;
	void setOuterCutoff(const GLfloat outerCutoff);

	void setCutoffs(const GLfloat innerCutoff, const GLfloat outerCutoff);

	virtual ~SpotLight() = default;
};