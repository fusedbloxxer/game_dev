#include "stdafx.h"
#include "AmbientLight.h"

AmbientLight::AmbientLight(const Vector3& color, const GLfloat ratio)
	:color{ color }, ratio{ ratio } {}

void AmbientLight::print(std::ostream& os) const
{
	os << "AmbientalLight: " << "[color = " << color << ", ratio = " << ratio << "]";
}

Vector3 AmbientLight::getColor() const
{
	return color;
}

void AmbientLight::setColor(const Vector3& color)
{
	this->color = color;
}

GLfloat AmbientLight::getRatio() const
{
	return ratio;
}

void AmbientLight::setRatio(GLfloat ratio)
{
	this->ratio = ratio;
}
