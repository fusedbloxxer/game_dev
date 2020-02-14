#include "stdafx.h"
#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(const GLint id, const Vector3& diffuseColor, const Vector3& specularColor, const GLfloat specPower, const Vector3& direction)
	:NormalLight{ DIRECTIONAL_LIGHT, id, diffuseColor, specularColor, specPower, direction } {}

void DirectionalLight::print(std::ostream& os) const
{
	std::cout << "DirectionalLight: [";
	NormalLight::print(os);
	std::cout << "]";
}

DirectionalLight::~DirectionalLight() {}
