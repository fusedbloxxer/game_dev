#pragma once
#include "SceneManager.h"
#include "NormalLight.h"
#include <algorithm>

template<typename AssociatedObjectType>
class Associated
{
public:
	inline static constexpr GLint STATE_NOT_ASSOCIATED = -1;

protected:
	GLint aObj;

public:
	const GLint getAObj() const;
	void setAObj(const GLint aObj);

	virtual ~Associated() = 0;

protected:
	Associated(GLint aObj = STATE_NOT_ASSOCIATED);

	// Checks if the associated object exists
	bool contract(const GLint aObj) const;

private:
	bool checkContract(const GLint aObj);
};

template<typename AssociatedObjectType>
Associated<AssociatedObjectType>::Associated(GLint aObj)
{
	if (aObj != STATE_NOT_ASSOCIATED)
	{
		setAObj(aObj);
	}
	else
	{
		this->aObj = aObj;
	}
}

template<typename AssociatedObjectType>
bool Associated<AssociatedObjectType>::contract(const GLint aObj) const
{
	throw std::runtime_error{ "Association contract is not implemented for this type." };
}

template<>
bool Associated<NormalLight>::contract(const GLint aObj) const
{
	const auto& objects = SceneManager::getInstance()->getSceneObjects();
	return std::find_if(objects.cbegin(), objects.cend(),
		[=](const auto& object) { return object->getId() == aObj; }) != objects.cend();
}

template<>
bool Associated<SceneObject>::contract(const GLint aObj) const
{
	const auto& lights = SceneManager::getInstance()->getLights();
	return std::find_if(lights.cbegin(), lights.cend(),
		[=](const auto& light) { const auto& normalLight = dynamic_cast<NormalLight*>(light.get()); return normalLight && normalLight->getId() == aObj; }) != lights.cend();
}

template<typename AssociatedObjectType>
bool Associated<AssociatedObjectType>::checkContract(const GLint aObj)
{
	if (!contract(aObj))
	{
		throw std::runtime_error{ "Associated object doesn't exist. Contract is false." };
	}
	return true;
}

template<typename AssociatedObjectType>
const GLint Associated<AssociatedObjectType>::getAObj() const
{
	return aObj;
}

template<typename AssociatedObjectType>
inline void Associated<AssociatedObjectType>::setAObj(const GLint aObj)
{
	this->aObj = checkContract(aObj) ? aObj : STATE_NOT_ASSOCIATED;
}

template<typename AssociatedObjectType>
Associated<AssociatedObjectType>::~Associated() = default;

