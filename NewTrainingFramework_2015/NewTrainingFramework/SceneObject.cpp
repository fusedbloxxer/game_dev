#include "stdafx.h"
#include "SceneManager.h"
#include "SceneObject.h"
#include "Vertex_NFG.h"
#include <iostream>
#include <algorithm>

SceneObject::SceneObject(GLint id)
	:SceneObject{ id, Type::NORMAL }
{
}

SceneObject::SceneObject(GLint id, Type type)
	: id{ id }, name{}, model{ nullptr }, shader{ nullptr }, textures{}, wiredFormat{ false }, depthTest{ false }, type{ type }
{
}

void SceneObject::draw()
{
	// TODO; Generated case
	if (model == nullptr) {
		return;
	}

	glUseProgram(shader->getProgramId());

	glBindBuffer(GL_ARRAY_BUFFER, model->getVboId());

	if (!wiredFormat)
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->getIboId());
	else
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->getWiredboId());

	if (shader->positionAttribute != -1)
	{
		glEnableVertexAttribArray(shader->positionAttribute);
		glVertexAttribPointer(shader->positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex_NFG), 0);
	}

	if (shader->unifMatrix != -1)
	{
		auto camera = SceneManager::getInstance()->getActiveCamera();
		// TODO;
		// Add rot, scale, position - matrices for object
		glUniformMatrix4fv(shader->unifMatrix, 1, GL_FALSE, (float*)(camera->getViewMatrix() * camera->getProjMatrix()).m);
	}

	if (shader->isTextureUniform != -1)
	{
		glUniform1f(shader->isTextureUniform, (GLfloat) textures.size());

		// TEXTURE/COLOR WORK
		if (textures.size() > 0)
		{
			for (GLint index = 0; index < std::min<GLint>(32, textures.size()); ++index)
			{
				glActiveTexture(index + GL_TEXTURE0);
				glBindTexture(textures[index]->getTextureResource()->type, textures[index]->getTextureId());

				if (shader->uvAttribute != -1)
				{
					glEnableVertexAttribArray(shader->uvAttribute);
					glVertexAttribPointer(shader->uvAttribute, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex_NFG), (void*)(5 * sizeof(Vector3)));
				}

				if (shader->textureUniform != -1)
				{
					glUniform1i(shader->textureUniform, index); // Id = 0 deoarece este implicit o singura textura.
				}
			}
		}
		else if (shader->colorAttribute != -1)
		{
			// GLfloat colors[] = { color.x, color.y, color.z };
			// glDisableVertexAttribArray(shader->colorAttribute);
			// glVertexAttrib3fv(shader->colorAttribute, colors);
			// glVertexAttribPointer(shader->colorAttribute, 3, GL_FLOAT, GL_FALSE, 0, (float *)colors);
		}
	}

	if (!wiredFormat)
	{
		glDrawElements(GL_TRIANGLES, model->getNoInd(), GL_UNSIGNED_SHORT, 0);
	}
	else
	{
		glDrawElements(GL_LINES, model->getNoIndWired(), GL_UNSIGNED_SHORT, 0);
	}
}

void SceneObject::update()
{
	// TODO;
}

SceneObject::~SceneObject()
{
	// TODO;
}

SceneObject::Type SceneObject::atot(const char* type)
{
	if (strcmp(type, "normal") == 0) {
		return NORMAL;
	}
	else if (strcmp(type, "terrain") == 0) {
		//return TERRAIN;
	}
	else if (strcmp(type, "skybox") == 0) {
		//return SKYBOX;
	}
	else {
		std::cerr << "INVALID CONST CHAR * - SceneObject::Type CONVERSION: " << type << std::endl;
		abort();
	}
}

GLint SceneObject::getId() const
{
	return id;
}

void SceneObject::setId(GLint id)
{
	this->id = id;
}

SceneObject::Type SceneObject::getType() const
{
	return type;
}

std::string SceneObject::getName() const
{
	return name;
}

void SceneObject::setName(std::string name)
{
	this->name = name;
}

std::shared_ptr<Model> SceneObject::getModel()
{
	return model;
}

void SceneObject::setModel(std::shared_ptr<Model> model)
{
	this->model = model;
}

std::shared_ptr<Shader> SceneObject::getShader()
{
	return shader;
}

void SceneObject::setShader(std::shared_ptr<Shader> shader)
{
	this->shader = shader;
}

GLboolean SceneObject::getWiredFormat() const
{
	return wiredFormat;
}

void SceneObject::setWiredFormat(GLboolean wiredFormat)
{
	this->wiredFormat = wiredFormat;
}

GLboolean SceneObject::getDepthTest() const
{
	return depthTest;
}

void SceneObject::setDepthTest(GLboolean depthTest)
{
	this->depthTest = depthTest;
}

Vector3& SceneObject::getPosition()
{
	return position;
}

void SceneObject::setPosition(Vector3& position)
{
	this->position = position;
}

Vector3& SceneObject::getRotation()
{
	return rotation;
}

void SceneObject::setRotation(Vector3& rotation)
{
	this->rotation = rotation;
}

Vector3& SceneObject::getScale()
{
	return scale;
}

void SceneObject::setScale(Vector3& scale)
{
	this->scale = scale;
}

Vector3& SceneObject::getColor()
{
	return color;
}

void SceneObject::setColor(Vector3& color)
{
	this->color = color;
}

std::vector<std::shared_ptr<Texture>>& SceneObject::getTextures()
{
	return textures;
}

void SceneObject::setTextures(std::vector<std::shared_ptr<Texture>>& textures)
{
	this->textures = textures;
}
