#include "stdafx.h"
#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(const GLint id, const Vector3& diffuseColor, const Vector3& specularColor, const GLfloat specPower, const Vector3& direction)
	:NormalLight{ id, diffuseColor, specularColor, specPower }, direction{ direction } {}

void DirectionalLight::print(std::ostream& os) const
{
	std::cout << "DirectionalLight: [";
	NormalLight::print(os);
	std::cout << ", direction = " << direction << "]";
}

const Vector3& DirectionalLight::getDirection() const
{
	return direction;
}

void DirectionalLight::setDirection(const Vector3& direction)
{
	this->direction = direction;
}

DirectionalLight::~DirectionalLight() {}
