#include "stdafx.h"
#include "SceneManager.h"
#include "NormalLight.h"
#include "VertexAxis.h"

NormalLight::NormalLight(const GLuint lightType, const GLint id, const Vector3& diffuseColor, const Vector3& specularColor, const GLfloat specPower, const Vector3& direction)
	:lightType{ lightType }, id{ id }, diffuseColor{ diffuseColor }, specularColor{ specularColor }, specPower{ specPower }, direction{ direction }, debugModelMatrix{ Matrix().SetTranslation(direction.x, direction.y, direction.z) }
{
	if (lightType > 3 || lightType < 1)
	{
		throw std::runtime_error("Invalid light type.");
	}
}

void NormalLight::draw()
{
	if (SceneManager::getInstance()->debug())
	{
		// Binding program
		glUseProgram(debugShader->getProgramId());

		// Binding data and index buffers.
		glBindBuffer(GL_ARRAY_BUFFER, debugModel->getVboId());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, debugModel->getIboId());

		// Send data to the shaders.
		Fields fields = debugShader->getFields();

		if (fields.positionAttribute != -1)
		{
			glEnableVertexAttribArray(fields.positionAttribute);
			glVertexAttribPointer(fields.positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(VertexNfg), 0);
		}

		if (fields.modelUniform != -1)
		{
			glUniformMatrix4fv(fields.modelUniform, 1, GL_FALSE, (float*)(debugModelMatrix).m);
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

		if (fields.normAttribute != -1)
		{
			glEnableVertexAttribArray(fields.normAttribute);
			glVertexAttribPointer(fields.normAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(VertexNfg), (void*)(2 * sizeof(Vector3)));
		}

		if (fields.colorAttribute != -1)
		{
			glDisableVertexAttribArray(fields.colorAttribute);
			glVertexAttrib3f(fields.colorAttribute, diffuseColor.x, diffuseColor.y, diffuseColor.z);
		}

		if (fields.ambientalRatioUniform != -1)
		{
			glUniform1f(fields.ambientalRatioUniform, 1.0f);
		}

		if (fields.kdifUniform != -1)
		{
			glUniform1f(fields.kdifUniform, 0.0f);
		}

		if (fields.kspecUniform != -1)
		{
			glUniform1f(fields.kspecUniform, 0.0f);
		}

		// Drawing selected data from buffers
		glDrawElements(GL_TRIANGLES, debugModel->getNoInd(), GL_UNSIGNED_SHORT, 0);

		// Closing data buffers
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
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

NormalLight::~NormalLight() {}