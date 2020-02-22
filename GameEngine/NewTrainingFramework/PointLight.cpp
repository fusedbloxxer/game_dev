#include "stdafx.h"
#include "PointLight.h"

PointLight::PointLight(const GLint id, const GLint aObj, const Vector3& diffuseColor, const Vector3& specularColor, const GLfloat specPower, const Vector3& direction)
	:NormalLight{ POINT_LIGHT, id, aObj, diffuseColor, specularColor, specPower, direction } {}

void PointLight::print(std::ostream& os) const
{
	std::cout << "PointLight: [";
	NormalLight::print(os);
	std::cout << "]";
}
