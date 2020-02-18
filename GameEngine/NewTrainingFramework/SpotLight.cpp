#include "stdafx.h"
#include "SceneManager.h"
#include "SpotLight.h"
#include <algorithm>
#include <numeric>

SpotLight::SpotLight(const GLint id, const GLint aObj, const Vector3& diffuseColor, const Vector3& specularColor, const GLfloat specPower, const Vector3& direction, const GLfloat in, const GLfloat out)
	:NormalLight{ SPOT_LIGHT, id, diffuseColor, specularColor, specPower, direction } 
{
	setAObj(aObj); setInnerCutoff(in); setOuterCutoff(out);
}

void SpotLight::print(std::ostream& os) const
{
	std::cout << "SpotLight: [";
	NormalLight::print(os);
	std::cout << "]";
}

const GLint SpotLight::getAObj() const
{
	return aObj;
}

inline void SpotLight::setAObj(const GLint aObj)
{
	auto objs = SceneManager::getInstance()->getSceneObjects();
	auto exists = std::count_if(objs.cbegin(), objs.cend(), [&aObj](const auto& o) { return aObj == o->getId(); }) != 0;

	if (exists)
	{
		this->aObj = aObj;
	}
	else
	{
		throw std::runtime_error{ "Associated object id does not exist." };
	}
}

inline constexpr const GLfloat& checkCutoff(const GLfloat& cutoff)
{
	if (cutoff < 0 || cutoff > 1)
	{
		throw std::runtime_error("Cutoff cannot be lower than 0 or higher than one.");
	}
	return cutoff;
}

const GLfloat SpotLight::getInnerCutoff() const
{
	return innerCutoff;
}

void SpotLight::setInnerCutoff(const GLfloat innerCutoff)
{
	if (innerCutoff < outerCutoff)
	{
		throw std::runtime_error{ "Inner cutoff cannot be lower than the outer cutoff" };
	}
	this->innerCutoff = checkCutoff(innerCutoff);
}

const GLfloat SpotLight::getOuterCutoff() const
{
	return outerCutoff;
}

void SpotLight::setOuterCutoff(const GLfloat outerCutoff)
{
	if (innerCutoff < outerCutoff)
	{
		throw std::runtime_error{ "Inner cutoff cannot be lower than the outer cutoff" };
	}
	this->outerCutoff = checkCutoff(outerCutoff);
}

void SpotLight::setCutoffs(const GLfloat innerCutoff, const GLfloat outerCutoff)
{
	if (checkCutoff(innerCutoff) < checkCutoff(outerCutoff))
	{
		throw std::runtime_error{ "Inner cutoff cannot be lower than the outer cutoff" };
	}

	this->innerCutoff = innerCutoff;
	this->outerCutoff = outerCutoff;
}
