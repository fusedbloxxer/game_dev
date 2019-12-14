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

void FireObject::draw()
{
	sendCommonData();

	Fields fields = shader->getFields();
	
	if (fields.timeUniform != -1)
	{
		glUniform1f(fields.timeUniform, time);
	}

	if (fields.dispMaxUniform != -1)
	{
		glUniform1f(fields.dispMaxUniform, dispMax);
	}

	if (SceneManager::getInstance()->debug())
	{
		// Draws wires
		glDrawElements(GL_LINES, model->getNoIndWired(), GL_UNSIGNED_SHORT, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		if (type != Type::SKYBOX)
		{
			drawAxis();
		}
	}
	else if (wiredFormat)
	{
		glDrawElements(GL_LINES, model->getNoIndWired(), GL_UNSIGNED_SHORT, 0);
	}
	else
	{
		glDrawElements(GL_TRIANGLES, model->getNoInd(), GL_UNSIGNED_SHORT, 0);
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

FireObject::~FireObject() = default;

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
