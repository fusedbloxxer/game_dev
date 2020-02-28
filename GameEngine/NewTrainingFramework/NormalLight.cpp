#include "stdafx.h"
#include "SceneManager.h"
#include "NormalLight.h"
#include "VertexAxis.h"
#include <algorithm>

NormalLight::NormalLight(const GLuint lightType, const GLint id, const GLint aObj, const Vector3& diffuseColor, const Vector3& specularColor, const GLfloat specPower, const Vector3& direction)
	:lightType{ lightType }, id{ id }, diffuseColor{ diffuseColor }, specularColor{ specularColor }, specPower{ specPower }, direction{ direction }
{
	if (lightType > 3 || lightType < 1)
	{
		throw std::runtime_error("Invalid light type.");
	}

	setDirection(direction);
	setAObj(aObj);
}

NormalLight::NormalLight(const GLuint lightType, const GLint id, const Vector3& diffuseColor, const Vector3& specularColor, const GLfloat specPower, const Vector3& direction)
	:NormalLight{ lightType, id, STATE_NOT_ASSOCIATED, diffuseColor, specularColor, specPower, direction } {}

void NormalLight::draw()
{
	// Binding program
	glUseProgram(debugShader->getProgramId());

	// Binding data and index buffers.
	glBindBuffer(GL_ARRAY_BUFFER, debugModel->getVboId());
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, debugModel->getIboId());

	// Send data to the shaders.
	Fields fields = debugShader->getFields();

	if (fields.colorAttribute != -1)
	{
		glDisableVertexAttribArray(fields.colorAttribute);
		glVertexAttrib3f(fields.colorAttribute, diffuseColor.x, diffuseColor.y, diffuseColor.z);
	}

	if (fields.positionAttribute != -1)
	{
		glEnableVertexAttribArray(fields.positionAttribute);
		glVertexAttribPointer(fields.positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(VertexNfg), 0);
	}

	if (fields.viewUniform != -1)
	{
		glUniformMatrix4fv(fields.viewUniform, 1, GL_FALSE, (float*)(SceneManager::getInstance()->getActiveCamera()->getViewMatrix()).m);
	}

	if (fields.projectionUniform != -1)
	{
		glUniformMatrix4fv(fields.projectionUniform, 1, GL_FALSE, (float*)(SceneManager::getInstance()->getActiveCamera()->getProjMatrix()).m);
	}
	
	// If the light has an associated object set it's model in debug mode
	if (const auto & pointLight = dynamic_cast<PointLight*>(this); fields.modelUniform != -1 && pointLight && pointLight->getAObj() != NormalLight::STATE_NOT_ASSOCIATED)
	{
		const auto& objects = SceneManager::getInstance()->getSceneObjects();
		const auto& [x, y, z] = (*std::find_if(objects.begin(), objects.end(),
			[&pointLight](const auto& object) { return object->getId() == pointLight->getAObj(); }))->getPosition();
		glUniformMatrix4fv(fields.modelUniform, 1, GL_FALSE, (float*)(Matrix().SetTranslation(x, y, z)).m);
	}
	else
	{
		glUniformMatrix4fv(fields.modelUniform, 1, GL_FALSE, (float*)(debugModelMatrix).m);
	}

	// Drawing selected data from buffers
	glDrawElements(GL_TRIANGLES, debugModel->getNoInd(), GL_UNSIGNED_SHORT, 0);

	// Closing data buffers
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void NormalLight::update()
{
	// Does nothing . . .
}

void NormalLight::print(std::ostream& os) const
{
	os << "NormalLight: [Specular Color = " << specularColor << ", Diffuse Color = " << diffuseColor << ", Specular Power = " << specPower << ", Direction = " << direction
		<< ", debugModelMatrix = " << debugModelMatrix << "]";
}

void NormalLight::setDebugShader(const std::shared_ptr<Shader>& debugShader)
{
	NormalLight::debugShader = debugShader;
}

void NormalLight::setDebugModel(const std::shared_ptr<Model>& debugModel)
{
	NormalLight::debugModel = debugModel;
}

const Vector3& NormalLight::getSpecularColor() const
{
	return specularColor;
}

void NormalLight::setSpecularcolor(const Vector3& specularColor)
{
	this->specularColor = specularColor;
}

const Vector3& NormalLight::getDiffuseColor() const
{
	return diffuseColor;
}

void NormalLight::setDiffuseColor(const Vector3& diffuseColor)
{
	this->diffuseColor = diffuseColor;
}

GLfloat NormalLight::getSpecPower() const
{
	return specPower;
}

void NormalLight::setSpecPower(const GLfloat specPower)
{
	this->specPower = specPower;
}

const GLint NormalLight::getId() const
{
	return id;
}

void NormalLight::setId(const GLint id)
{
	this->id = id;
}

const GLint NormalLight::getLightType() const
{
	return lightType;
}

const Vector3& NormalLight::getDirection() const
{
	return direction;
}

void NormalLight::setDirection(const Vector3& direction)
{
	this->debugModelMatrix.SetTranslation(direction.x, direction.y, direction.z);
	this->direction = direction;
}

const Matrix& NormalLight::getDebugModelMatrix() const
{
	return debugModelMatrix;
}

const GLint NormalLight::getAObj() const
{
	return aObj;
}

void NormalLight::setAObj(const GLint aObj)
{
	if (aObj != STATE_NOT_ASSOCIATED)
	{
		const auto& objects = SceneManager::getInstance()->getSceneObjects();
		const auto& exists = std::find_if(objects.cbegin(), objects.cend(),
			[=](const auto& object) { return object->getId() == aObj; }) != objects.cend();

		if (!exists)
		{
			throw std::runtime_error{ "Light " + std::to_string(id) + " doesn't have a valid associated object." };
		}
	}
	this->aObj = aObj;
}

NormalLight::~NormalLight() = default;