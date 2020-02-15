#include "stdafx.h"
#include "SceneManager.h"
#include "VertexNfg.h"
#include "AxisResource.h"
#include <algorithm>
#include "DirectionalLight.h"
#include "SpotLight.h"

SceneObject::SceneObject(GLint id)
	:SceneObject{ id, Type::NORMAL } {}

SceneObject::SceneObject(GLint id, Type type)
	: id{ id }, name{}, model{ nullptr }, shader{ nullptr }, textures{}, wiredFormat{ false }, type{ type }, modified{ false }, followingCamera{}, offset{}, reflection{ false } {}

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
		glBindTexture(textures[index]->getTextureResources()->type, textures[index]->getTextureId());

		if (fields.textureUniform[index] != -1)
		{
			glUniform1i(fields.textureUniform[index], index);
		}
	}

	if (fields.isReflectedUniform != -1)
	{
		glUniform1f(fields.isReflectedUniform, reflection ? 1.0f : 0.0f);
	}

	if (fields.skyboxUniform != -1)
	{
		for (auto x : SceneManager::getInstance()->getSceneObjects())
		{
			if (x->getType() == Type::SKYBOX)
			{
				GLuint index = std::min<GLuint>(Fields::MAX_TEXTURES, textures.size());
				glActiveTexture(index + GL_TEXTURE0);
				glBindTexture(x->textures[0]->getTextureResources()->type, x->textures[0]->getTextureId());
				glUniform1i(fields.skyboxUniform, index);
				break;
			}
		}
	}

	if (fields.normAttribute != -1)
	{
		glEnableVertexAttribArray(fields.normAttribute);
		glVertexAttribPointer(fields.normAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(VertexNfg), (void*)(2 * sizeof(Vector3)));
	}

	if (fields.colorAttribute != -1)
	{
		glDisableVertexAttribArray(fields.colorAttribute);
		glVertexAttrib3f(fields.colorAttribute, color.x, color.y, color.z);
	}

	// Send ambiental light
	if (fields.ambientalLightUniform != -1)
	{
		const auto& [x, y, z] = SceneManager::getInstance()->getAmbientalLight()->getColor();
		glUniform3f(fields.ambientalLightUniform, x, y, z);
	}

	if (fields.ambientalRatioUniform != -1)
	{
		glUniform1f(fields.ambientalRatioUniform, SceneManager::getInstance()->getAmbientalLight()->getRatio());
	}

	if (fields.kdifUniform != -1)
	{
		glUniform1f(fields.kdifUniform, kdif);
	}

	if (fields.kspecUniform != -1)
	{
		glUniform1f(fields.kspecUniform, kspec);
	}

	{
		// Other lights scope
		const auto& lights = SceneManager::getInstance()->getLights();
		const auto& count = std::min<std::size_t>(Fields::MAX_LIGHT_SOURCES, lights.size());

		// Send first directional light source
		for (auto index = 0; index < count; ++index)
		{
			if (const auto & normLight = dynamic_cast<NormalLight*>(lights.at(index).get()))
			{
				if (fields.lights[index].lightTypeUniform != -1)
				{
					glUniform1f(fields.lights[index].lightTypeUniform, normLight->getLightType());
				}

				if (fields.lights[index].diffuseColorUniform != -1)
				{
					const auto& [x, y, z] = normLight->getDiffuseColor();
					glUniform3f(fields.lights[index].diffuseColorUniform, x, y, z);
				}

				if (fields.lights[index].specularColorUniform != -1)
				{
					const auto& [x, y, z] = normLight->getSpecularColor();
					glUniform3f(fields.lights[index].specularColorUniform, x, y, z);
				}

				if (fields.lights[index].specularPowerUniform != -1)
				{
					glUniform1f(fields.lights[index].specularPowerUniform, normLight->getSpecPower());
				}

				if (fields.lights[index].lightDirectionUniform != -1)
				{
					const auto& [x, y, z] = normLight->getDirection();
					glUniform3f(fields.lights[index].lightDirectionUniform, x, y, z);
				}

				if (const auto & spotLight = dynamic_cast<SpotLight*>(lights.at(index).get()))
				{
					if (fields.lights[index].associatedObjectPositionUniform != -1)
					{
						const auto& objects = SceneManager::getInstance()->getSceneObjects();
						const auto& [x, y, z, w] =  Vector4((*std::find_if(objects.cbegin(), objects.cend(),
							[&spotLight](const std::shared_ptr<SceneObject> o) { return o->getId() == spotLight->getAObj(); }))->getPosition());
						glUniform3f(fields.lights[index].associatedObjectPositionUniform, x, y, z);
					}

					if (fields.lights[index].innerCutoffUniform != -1)
					{
						glUniform1f(fields.lights[index].innerCutoffUniform, spotLight->getInnerCutoff());
					}

					if (fields.lights[index].outercutoffUniform != -1)
					{
						glUniform1f(fields.lights[index].innerCutoffUniform, spotLight->getOuterCutoff());
					}
				}
			}
		}
	}

}

void SceneObject::update()
{
	auto camera = SceneManager::getInstance()->getActiveCamera();

	if (followingCamera.x == 1)
	{
		position.x = offset.x + camera->getPosition().x;
		modified = true;
	}

	if (followingCamera.y == 1)
	{
		position.y = offset.y + camera->getPosition().y;
		modified = true;
	}

	if (followingCamera.z == 1)
	{
		position.z = offset.z + camera->getPosition().z;
		modified = true;
	}

	if (trajectory != nullptr)
	{
		trajectory->move(this, camera->getDeltaTime());
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
	else if (strcmp(type, "fire") == 0) {
		return FIRE;
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

GLboolean SceneObject::getReflection() const
{
	return reflection;
}

void SceneObject::setReflection(GLboolean reflection)
{
	this->reflection = reflection;
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

void SceneObject::setFollowingCamera(const Vector3& followingCamera)
{
	this->followingCamera = followingCamera;
}

Vector3& SceneObject::getPosition()
{
	return position;
}

void SceneObject::setPosition(const Vector3& position)
{
	this->position = position;
	offset = position;
	modified = true;
}

Vector3& SceneObject::getRotation()
{
	return rotation;
}

void SceneObject::setRotation(const Vector3& rotation)
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

void SceneObject::setScale(const Vector3& scale)
{
	this->scale = scale;
	modified = true;
}

Vector3& SceneObject::getColor()
{
	return color;
}

void SceneObject::setColor(const Vector3& color)
{
	this->color = color;
}

GLfloat SceneObject::getKDif() const
{
	return kdif;
}

void SceneObject::setKDif(GLfloat kdif)
{
	if (kdif < 0 || kdif > 1)
	{
		throw std::runtime_error{ "Value cannot be lower than zero or higher than one." };
	}
	this->kdif = kdif;
}

GLfloat SceneObject::getKSpec() const
{
	return kspec;
}

void SceneObject::setKSpec(GLfloat kspec)
{
	if (kspec < 0.0f || kspec > 1.0f)
	{
		throw std::runtime_error{ "Value cannot be lower than 0 or higher than 1." };
	}
	this->kspec = kspec;
}

std::vector<std::shared_ptr<Texture>>& SceneObject::getTextures()
{
	return textures;
}

void SceneObject::setTextures(const std::vector<std::shared_ptr<Texture>>& textures)
{
	this->textures = textures;
}

std::shared_ptr<Trajectory> SceneObject::getTrajectory()
{
	return trajectory;
}

void SceneObject::setTrajectory(const std::shared_ptr<Trajectory> trajectory)
{
	this->trajectory = trajectory;
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
		glVertexAttribPointer(fields.colorAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(VertexAxis), (void *)sizeof(Vector3));
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