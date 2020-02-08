#include "stdafx.h"
#include "NormalLight.h"

NormalLight::NormalLight(const GLint id, const Vector3& diffuseColor, const Vector3& specularColor, const GLfloat specPower)
	:id{ id }, diffuseColor { diffuseColor }, specularColor{ specularColor }, specPower{ specPower } {}

void NormalLight::print(std::ostream& os) const
{
	os << "NormalLight: [Specular Color = " << specularColor << ", Diffuse Color = " << diffuseColor << ", Specular Power = " << specPower << "]";
}

const Vector3& NormalLight::getSpecularColor() const
{
	return specularColor;
}

void NormalLight::setSpecularcolor(const Vector3& specularColor)
{
	this->specularColor = specularColor;
}

const Vector3& NormalLight::getDiffuseColor() const
{
	return diffuseColor;
}

void NormalLight::setDiffuseColor(const Vector3& diffuseColor)
{
	this->diffuseColor = diffuseColor;
}

GLfloat NormalLight::getSpecPower() const
{
	return specPower;
}

void NormalLight::setSpecPower(const GLfloat specPower)
{
	this->specPower = specPower;
}

const GLint NormalLight::getId() const
{
	return id;
}

void NormalLight::setId(const GLint id)
{
	this->id = id;
}

NormalLight::~NormalLight() {}
