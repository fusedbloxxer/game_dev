#include "stdafx.h"
#include "SceneManager.h"
#include "FireObject.h"
#include <ctime>

FireObject::FireObject(GLint id)
	:FireObject{id, Type::FIRE}
{
}

FireObject::FireObject(GLint id, Type type)
	: SceneObject{ id, type }, dispMax{ 0.0f }, time{ 0.0f } {}

void FireObject::update()
{
	SceneObject::update();

	GLfloat deltaTime = SceneManager::getInstance()->getActiveCamera()->getDeltaTime() / 2;
	time = (time + deltaTime < 1) ? time + deltaTime : time - 1 + deltaTime;
}

void FireObject::sendSpecificData(const Fields& fields)
{
	if (fields.timeUniform != -1)
	{
		glUniform1f(fields.timeUniform, time);
	}

	if (fields.dispMaxUniform != -1)
	{
		glUniform1f(fields.dispMaxUniform, dispMax);
	}
}

GLfloat FireObject::getTime() const
{
	return time;
}

void FireObject::setTime(GLfloat time)
{
	this->time = time;
}

GLfloat FireObject::getDispMax() const
{
	return dispMax;
}

void FireObject::setDispMax(GLfloat dispMax)
{
	this->dispMax = dispMax;
}

FireObject::~FireObject() = default;
