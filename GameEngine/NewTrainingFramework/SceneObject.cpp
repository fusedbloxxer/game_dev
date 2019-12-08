#include "stdafx.h"
#include "SceneManager.h"
#include "VertexNfg.h"
#include "AxisResource.h"
#include <algorithm>

SceneObject::SceneObject(GLint id)
	:SceneObject{ id, Type::NORMAL } {}

SceneObject::SceneObject(GLint id, Type type)
	: id{ id }, name{}, model{ nullptr }, shader{ nullptr }, textures{}, wiredFormat{ false }, type{ type }, modified{ false }, followingCamera{}, offset{} {}

void SceneObject::draw()
{
	sendCommonData();

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

void SceneObject::sendCommonData()
{
	glUseProgram(shader->getProgramId());
	glBindBuffer(GL_ARRAY_BUFFER, model->getVboId());

	if (SceneManager::getInstance()->debug())
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->getWiredboId());
	}
	else if (!wiredFormat)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->getIboId());
	}
	else
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->getWiredboId());
	}

	// Get common attributes and uniforms
	Fields fields = shader->getFields();

	if (fields.positionAttribute != -1)
	{
		glEnableVertexAttribArray(fields.positionAttribute);
		glVertexAttribPointer(fields.positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(VertexNfg), 0);
	}

	if (fields.modelUniform != -1)
	{
		glUniformMatrix4fv(fields.modelUniform, 1, GL_FALSE, (float*)(getModelMatrix()).m);
	}

	if (fields.viewUniform != -1)
	{
		glUniformMatrix4fv(fields.viewUniform, 1, GL_FALSE, (float*)(SceneManager::getInstance()->getActiveCamera()->getViewMatrix()).m);
	}

	if (fields.projectionUniform != -1)
	{
		glUniformMatrix4fv(fields.projectionUniform, 1, GL_FALSE, (float*)(SceneManager::getInstance()->getActiveCamera()->getProjMatrix()).m);
	}

	if (fields.fogColorUniform != -1)
	{
		Vector3 fog = SceneManager::getInstance()->getFog().getFogColor();
		glUniform3f(fields.fogColorUniform, fog.x, fog.y, fog.z);
	}

	if (fields.fogClarityUniform != -1)
	{
		GLfloat clarity = SceneManager::getInstance()->getFog().getFogClarityRadius();
		glUniform1f(fields.fogClarityUniform, clarity);
	}

	if (fields.fogTransitionUniform != -1)
	{
		GLfloat transition = SceneManager::getInstance()->getFog().getFogTransitionRadius();
		glUniform1f(fields.fogTransitionUniform, transition);
	}

	if (fields.cameraUniform != -1)
	{
		auto cameraPosition = SceneManager::getInstance()->getActiveCamera()->getPosition();
		glUniform3f(fields.cameraUniform, cameraPosition.x, cameraPosition.y, cameraPosition.z);
	}

	if (fields.uvAttribute != -1)
	{
		glEnableVertexAttribArray(fields.uvAttribute);
		glVertexAttribPointer(fields.uvAttribute, 2, GL_FLOAT, GL_FALSE, sizeof(VertexNfg), (void*)(5 * sizeof(Vector3)));
	}

	for (GLuint index = 0; index < std::min<GLuint>(Fields::MAX_TEXTURES, textures.size()); ++index)
	{
		glActiveTexture(index + GL_TEXTURE0);
		glBindTexture(textures[index]->getTextureResource()->type, textures[index]->getTextureId());

		if (fields.textureUniform[index] != -1)
		{
			glUniform1i(fields.textureUniform[index], index);
		}
	}

	if (fields.colorAttribute != -1)
	{
		glDisableVertexAttribArray(fields.colorAttribute);
		glVertexAttrib3f(fields.colorAttribute, color.x, color.y, color.z);
	}
}

void SceneObject::update()
{
	auto cameraPos = SceneManager::getInstance()->getActiveCamera()->getPosition();

	if (followingCamera.x == 1)
	{
		position.x = offset.x + cameraPos.x;
		modified = true;
	}

	if (followingCamera.y == 1)
	{
		position.y = offset.y + cameraPos.y;
		modified = true;
	}

	if (followingCamera.z == 1)
	{
		position.z = offset.z + cameraPos.z;
		modified = true;
	}
}

SceneObject::~SceneObject() = default;

SceneObject::Type SceneObject::atot(const char* type)
{
	if (strcmp(type, "normal") == 0) {
		return NORMAL;
	}
	else if (strcmp(type, "terrain") == 0) {
		return TERRAIN;
	}
	else if (strcmp(type, "skybox") == 0) {
		return SKYBOX;
	}
	throw std::runtime_error{ "INVALID CONST CHAR * - SceneObject::Type CONVERSION" };
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
	}
	return modelMatrix;
}

Vector3& SceneObject::getFollowingCamera()
{
	return followingCamera;
}

void SceneObject::setFollowingCamera(Vector3& followingCamera)
{
	this->followingCamera = followingCamera;
}

Vector3& SceneObject::getPosition()
{
	return position;
}

void SceneObject::setPosition(Vector3& position)
{
	this->position = position;
	offset = position;
	modified = true;
}

Vector3& SceneObject::getRotation()
{
	return rotation;
}

void SceneObject::setRotation(Vector3& rotation)
{
	this->rotation.x = GLfloat(TO_RAD(rotation.x));
	this->rotation.y = GLfloat(TO_RAD(rotation.y));
	this->rotation.z = GLfloat(TO_RAD(rotation.z));
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

void SceneObject::drawAxis()
{
	// Draws axis
	glUseProgram(axisShader->getProgramId());
	glBindBuffer(GL_ARRAY_BUFFER, model->getAxisModel().getId());

	auto fields = axisShader->getFields();
	if (fields.positionAttribute != -1)
	{
		glEnableVertexAttribArray(fields.positionAttribute);
		glVertexAttribPointer(fields.positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(VertexAxis), 0);
	}

	if (fields.colorAttribute != -1)
	{
		glEnableVertexAttribArray(fields.colorAttribute);
		glVertexAttribPointer(fields.colorAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(VertexAxis), (void*)sizeof(Vector3));
	}

	if (fields.modelUniform != -1)
	{
		glUniformMatrix4fv(fields.modelUniform, 1, GL_FALSE, (float*)(getModelMatrix()).m);
	}

	if (fields.viewUniform != -1)
	{
		glUniformMatrix4fv(fields.viewUniform, 1, GL_FALSE, (float*)(SceneManager::getInstance()->getActiveCamera()->getViewMatrix()).m);
	}

	if (fields.projectionUniform != -1)
	{
		glUniformMatrix4fv(fields.projectionUniform, 1, GL_FALSE, (float*)(SceneManager::getInstance()->getActiveCamera()->getProjMatrix()).m);
	}

	glDrawArrays(GL_LINES, 0, 6);
}