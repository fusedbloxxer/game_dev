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
	: Collidable{ true }, id{ id }, name{}, model{ nullptr }, shader{ nullptr }, textures{}, wiredFormat{ false }, type{ type }, modified{ false }, followingCamera{}, offset{}, reflection{ false } {}

void SceneObject::draw()
{
	// Template Pattern
	sendCommonData();

	sendSpecificData(shader->getFields());

	callDrawFunctions();
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
	const Fields& fields = shader->getFields();

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
			if (const auto& normLight = dynamic_cast<NormalLight*>(lights.at(index).get()))
			{
				if (associatedLight == STATE_NOT_ASSOCIATED || normLight->getId() == associatedLight)
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
						if (const auto& pointLight = dynamic_cast<PointLight*>(lights.at(index).get()); pointLight&& pointLight->getAObj() != NormalLight::STATE_NOT_ASSOCIATED)
						{
							const auto& objects = SceneManager::getInstance()->getSceneObjects();
							const auto& [x, y, z] = (*std::find_if(objects.cbegin(), objects.cend(),
								[&pointLight](const std::shared_ptr<SceneObject> o) { return o->getId() == pointLight->getAObj(); }))->getPosition();
							glUniform3f(fields.lights[index].lightDirectionUniform, x, y, z);
						}
						else
						{
							const auto& [x, y, z] = normLight->getDirection();
							glUniform3f(fields.lights[index].lightDirectionUniform, x, y, z);
						}
					}

					if (const auto& spotLight = dynamic_cast<SpotLight*>(lights.at(index).get()))
					{
						if (fields.lights[index].associatedObjectPositionUniform != -1)
						{
							const auto& objects = SceneManager::getInstance()->getSceneObjects();
							const auto& [x, y, z, w] = Vector4((*std::find_if(objects.cbegin(), objects.cend(),
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
				else
				{
					if (fields.lights[index].diffuseColorUniform != -1)
					{
						glUniform3f(fields.lights[index].diffuseColorUniform, 0.0f, 0.0f, 0.0f);
					}

					if (fields.lights[index].specularColorUniform != -1)
					{
						glUniform3f(fields.lights[index].specularColorUniform, 0.0f, 0.0f, 0.0f);
					}
				}
			}
		}
	}

	// Normal Mapping
	if (fields.binormAttribute != -1)
	{
		glEnableVertexAttribArray(fields.binormAttribute);
		glVertexAttribPointer(fields.binormAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(VertexNfg), (void*)(1 * sizeof(Vector3)));
	}

	if (fields.tgtAttribute != -1)
	{
		glEnableVertexAttribArray(fields.tgtAttribute);
		glVertexAttribPointer(fields.tgtAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(VertexNfg), (void*)(3 * sizeof(Vector3)));
	}

	if (fields.normalMapUniform != -1 && normalMap != nullptr)
	{
		// Send boolean value
		glUniform1f(fields.hasNormalMapUniform, 1.0f);

		// Send normal map texture
		glActiveTexture(Fields::NORMAL_MAP_TEXTURE + GL_TEXTURE0);
		glBindTexture(normalMap->getTextureResources()->type, normalMap->getTextureId());
		glUniform1i(fields.normalMapUniform, Fields::NORMAL_MAP_TEXTURE);
	}
	else
	{
		// Send boolean value
		glUniform1f(fields.hasNormalMapUniform, 0.0f);
		glUniform1i(fields.normalMapUniform, Fields::NORMAL_MAP_TEXTURE);
	}
}

void SceneObject::sendSpecificData(const Fields& fields)
{
	// Nothing specific to be sent.
}

void SceneObject::callDrawFunctions()
{
	if (SceneManager::getInstance()->debug())
	{
		// Draws wires
		glDrawElements(GL_LINES, model->getNoIndWired(), GL_UNSIGNED_SHORT, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		if (type != Type::SKYBOX)
		{
			if (type != Type::TERRAIN)
			{
				// Draw collision box 
				drawCollisionBox();

				// Draw normals
				drawVertexNormals();
			}

			// Draw axis
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

void SceneObject::drawCollisionBox()
{
	// Use shader
	glUseProgram(axisShader->getProgramId());

	// Calculate the collision box and update the buffer
	Matrix aux, collisionBox = Matrix().SetScale(scale);
	collisionBox = collisionBox * aux.SetRotationX(rotation.x);
	collisionBox = collisionBox * aux.SetRotationY(rotation.y);
	collisionBox = collisionBox * aux.SetRotationZ(rotation.z);
	model->updateCollisionBox(collisionBox);

	// Open buffers
	glBindBuffer(GL_ARRAY_BUFFER, model->getCollisionVboId());
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->getCollisionIboId());

	// Select and send data from buffers
	sendLineData(axisShader);

	if (axisShader->getFields().modelUniform != -1)
	{
		glUniformMatrix4fv(axisShader->getFields().modelUniform, 1, GL_FALSE, (float*)(Matrix().SetTranslation(position)).m);
	}

	// Draw on screen
	glDrawElements(GL_LINES, model->getNoCollisionIndices(), GL_UNSIGNED_SHORT, 0);

	// Close buffers
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void SceneObject::drawAxis()
{
	// Use the program
	glUseProgram(axisShader->getProgramId());

	// Open the buffer
	glBindBuffer(GL_ARRAY_BUFFER, model->getAxisModel().getId());

	// Select and send data from buffers
	sendLineData(axisShader);

	if (axisShader->getFields().modelUniform != -1)
	{
		glUniformMatrix4fv(axisShader->getFields().modelUniform, 1, GL_FALSE, (float*)(getModelMatrix()).m);
	}

	// Draw on screen
	glDrawArrays(GL_LINES, 0, 6);

	// Close buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void SceneObject::drawVertexNormals()
{
	// Use the program
	glUseProgram(axisShader->getProgramId());

	model->updateNormals(getModelMatrix());

	// Open the buffer
	glBindBuffer(GL_ARRAY_BUFFER, model->getNormalVboId());

	// Select and send data from buffers
	sendLineData(axisShader);

	if (axisShader->getFields().modelUniform != -1)
	{
		glUniformMatrix4fv(axisShader->getFields().modelUniform, 1, GL_FALSE, (float*)(Matrix().SetIdentity()).m);
	}

	// Draw on screen
	glDrawArrays(GL_LINES, 0, model->getNoNormalInd());

	// Close buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void SceneObject::sendLineData(const std::shared_ptr<Shader>& shader)
{
	// Select data from the buffer
	auto fields = shader->getFields();

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

	if (fields.viewUniform != -1)
	{
		glUniformMatrix4fv(fields.viewUniform, 1, GL_FALSE, (float*)(SceneManager::getInstance()->getActiveCamera()->getViewMatrix()).m);
	}

	if (fields.projectionUniform != -1)
	{
		glUniformMatrix4fv(fields.projectionUniform, 1, GL_FALSE, (float*)(SceneManager::getInstance()->getActiveCamera()->getProjMatrix()).m);
	}
}

bool SceneObject::collides(Collidable* object) const
{
	if (const auto& sceneObject = dynamic_cast<SceneObject*>(object))
	{
		// Used in algebra
		Vector4 auxiliaryVector;

		// Extract extreme points
		const auto& localCoordsOb1 = this->model->getCollisionCoordinates();
		const auto& localCoordsOb2 = sceneObject->model->getCollisionCoordinates();

		// Compute model matrix for the first object
		Matrix matrix = Matrix().SetTranslation(const_cast<Vector3&>(position));

		// Calculate new extreme points for the first object, in world space
		auxiliaryVector = Vector4(localCoordsOb1[0][0], localCoordsOb1[0][1], localCoordsOb1[0][2], 1.0f) * matrix;
		const Vector3& maxCoordsOb1 = { auxiliaryVector.x, auxiliaryVector.y, auxiliaryVector.z };
		auxiliaryVector = Vector4(localCoordsOb1[1][0], localCoordsOb1[1][1], localCoordsOb1[1][2], 1.0f) * matrix;
		const Vector3& minCoordsOb1 = { auxiliaryVector.x, auxiliaryVector.y, auxiliaryVector.z };

		// Compute model matrix for the second object
		matrix = Matrix().SetTranslation(const_cast<Vector3&>(sceneObject->position));

		// Calculate new extreme points for the second object, in world space
		auxiliaryVector = Vector4(localCoordsOb2[0][0], localCoordsOb2[0][1], localCoordsOb2[0][2], 1.0f) * matrix;
		const Vector3& maxCoordsOb2 = { auxiliaryVector.x, auxiliaryVector.y, auxiliaryVector.z };
		auxiliaryVector = Vector4(localCoordsOb2[1][0], localCoordsOb2[1][1], localCoordsOb2[1][2], 1.0f) * matrix;
		const Vector3& minCoordsOb2 = { auxiliaryVector.x, auxiliaryVector.y, auxiliaryVector.z };

		return // max(ob1) >= min(ob2) && min(ob1) <= max(ob2)
			maxCoordsOb1.x >= minCoordsOb2.x && maxCoordsOb1.y >= minCoordsOb2.y && maxCoordsOb1.z >= minCoordsOb2.z &&
			minCoordsOb1.x <= maxCoordsOb2.x && minCoordsOb1.y <= maxCoordsOb2.y && minCoordsOb1.z <= maxCoordsOb2.z;
	}
	return false;
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

	Matrix aux, collisionBox = Matrix().SetScale(scale);
	collisionBox = collisionBox * aux.SetRotationX(rotation.x);
	collisionBox = collisionBox * aux.SetRotationY(rotation.y);
	collisionBox = collisionBox * aux.SetRotationZ(rotation.z);
	model->updateCollisionBox(collisionBox);

	// Check if the object collides with something else
	for (const auto& object : SceneManager::getInstance()->getSceneObjects())
	{
		if (this != object.get())
		{
			this->collideWith(object.get());
		}
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

void SceneObject::setFollowingCamera(const Vector3 & followingCamera)
{
	this->followingCamera = followingCamera;
}

Vector3& SceneObject::getPosition()
{
	return position;
}

void SceneObject::setPosition(const Vector3 & position)
{
	this->position = position;
	offset = position;
	modified = true;
}

Vector3& SceneObject::getRotation()
{
	return rotation;
}

void SceneObject::setRotation(const Vector3 & rotation)
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

void SceneObject::setScale(const Vector3 & scale)
{
	this->scale = scale;
	modified = true;
}

Vector3& SceneObject::getColor()
{
	return color;
}

void SceneObject::setColor(const Vector3 & color)
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

void SceneObject::setTextures(const std::vector<std::shared_ptr<Texture>> & textures)
{
	this->textures = textures;
}

std::shared_ptr<Trajectory> SceneObject::getTrajectory()
{
	return trajectory;
}

std::shared_ptr<Texture> SceneObject::getNormalMap()
{
	return normalMap;
}

void SceneObject::setNormalMap(const std::shared_ptr<Texture> & normalMap)
{
	this->normalMap = normalMap;
}

void SceneObject::setAssociatedLight(const GLint associatedLight)
{
	this->associatedLight = associatedLight;
}

const GLint SceneObject::getAssociatedLight() const
{
	return associatedLight;
}

void SceneObject::setTrajectory(const std::shared_ptr<Trajectory> & trajectory)
{
	this->trajectory = trajectory;
}

const Vector3& SceneObject::getCollisionBoxColor() const
{
	return collisionBoxColor;
}

void SceneObject::setCollisionBoxColor(const Vector3 & collisionBoxColor)
{
	this->collisionBoxColor = collisionBoxColor;
}

const Vector3& SceneObject::getDefaultCollisionBoxColor()
{
	return SceneObject::defaultCollisionBoxColor;
}

void SceneObject::setDefaultCollisionBoxColor(const Vector3 & collisionBoxColor)
{
	SceneObject::defaultCollisionBoxColor = collisionBoxColor;
}
