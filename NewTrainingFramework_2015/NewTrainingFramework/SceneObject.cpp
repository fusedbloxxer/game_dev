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
	: id{ id }, name{}, model{ nullptr }, shader{ nullptr }, textures{}, wiredFormat{ false }, type{ type }
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

	// Get field attributes and uniforms
	Fields fields = shader->getFields();

	if (fields.positionAttribute != -1)
	{
		glEnableVertexAttribArray(fields.positionAttribute);
		glVertexAttribPointer(fields.positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex_NFG), 0);
	}

	if (fields.unifMatrix != -1)
	{
		auto camera = SceneManager::getInstance()->getActiveCamera();
		// TODO;
		// Add rot, scale, position - matrices for object 
		glUniformMatrix4fv(fields.unifMatrix, 1, GL_FALSE, (float*)(getModelMatrix() * camera->getViewMatrix() * camera->getProjMatrix()).m);
	}

	// TEXTURE/COLOR WORK
	if (fields.textureUniform != -1)
	{
		// In loc de 32 fa get si preia nr de texturi.
		for (GLint index = 0; index < std::min<GLint>(32, textures.size()); ++index)
		{
			glActiveTexture(index + GL_TEXTURE0);
			glBindTexture(textures[index]->getTextureResource()->type, textures[index]->getTextureId());

			if (fields.uvAttribute != -1)
			{
				glEnableVertexAttribArray(fields.uvAttribute);
				glVertexAttribPointer(fields.uvAttribute, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex_NFG), (void*)(5 * sizeof(Vector3)));
			}

			if (fields.textureUniform != -1)
			{
				glUniform1i(fields.textureUniform, index); // Id = 0 deoarece este implicit o singura textura.
			}
		}
	}
	
	if (fields.colorAttribute != -1)
	{
		glVertexAttrib3f(fields.colorAttribute, color.x, color.y, color.z);
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

Matrix& SceneObject::getModelMatrix()
{
	// Lazy updates
	if (modified) {
		auto tran = modelMatrix.SetScale(scale);
		tran = tran * modelMatrix.SetRotationX(rotation.x);
		tran = tran * modelMatrix.SetRotationY(rotation.y);
		tran = tran * modelMatrix.SetRotationZ(rotation.z);
		tran = tran * modelMatrix.SetTranslation(position);
		modelMatrix = tran;
		modified = false;
		// std::cout << modelMatrix << '\n'; // Checker
	}
	return modelMatrix;
}

Vector3& SceneObject::getPosition()
{
	return position;
}

void SceneObject::setPosition(Vector3& position)
{
	this->position = position;
	modified = true;
}

Vector3& SceneObject::getRotation()
{
	return rotation;
}

void SceneObject::setRotation(Vector3& rotation)
{
	this->rotation.x = TO_RAD(rotation.x);
	this->rotation.y = TO_RAD(rotation.y);
	this->rotation.z = TO_RAD(rotation.z);
	modified = true;
}

Vector3& SceneObject::getScale()
{
	return scale;
}

void SceneObject::setScale(Vector3& scale)
{
	this->scale = scale;
	modified = true;
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
