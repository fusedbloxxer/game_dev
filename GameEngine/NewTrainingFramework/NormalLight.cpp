#include "stdafx.h"
#include "NormalLight.h"

NormalLight::NormalLight(NormalLight::Type type)
	:type{ type } {}

void NormalLight::print(std::ostream& os) const
{
}

NormalLight::Type NormalLight::getLightType() const
{
	return type;
}

void NormalLight::setLightType(Type type)
{
	this->type = type;
}

void NormalLight::fts(const NormalLight::Type& first, std::string& second) const
{
}
