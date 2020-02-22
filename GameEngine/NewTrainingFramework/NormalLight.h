#pragma once
#include "Drawable.h"
#include "Shader.h"
#include "Model.h"
#include "Light.h"
#include <memory>
#include <string>

class NormalLight : public Light, public Drawable
{
public:
	inline static constexpr GLint STATE_NOT_ASSOCIATED = -1;

protected:
	inline static std::shared_ptr<Shader> debugShader;
	inline static std::shared_ptr<Model> debugModel;

	inline static constexpr GLuint DIRECTIONAL_LIGHT = 1;
	inline static constexpr GLuint POINT_LIGHT = 2;
	inline static constexpr GLuint SPOT_LIGHT = 3;

	Vector3 specularColor, diffuseColor;
	Matrix debugModelMatrix;
	Vector3 direction;
	GLfloat specPower;
	GLuint lightType;
	GLint id, aObj;

	NormalLight(const GLuint lightType, const GLint id, const GLint aObj, const Vector3& diffuseColor = { 0.0f, 0.0f, 0.0f }, const Vector3& specularColor = { 0.0f, 0.0f, 0.0f }, const GLfloat specPower = 0, const Vector3 & direction = { 0.0f, 0.0f, 0.0f });
	NormalLight(const GLuint lightType, const GLint id, const Vector3& diffuseColor = { 0.0f, 0.0f, 0.0f }, const Vector3& specularColor = { 0.0f, 0.0f, 0.0f }, const GLfloat specPower = 0, const Vector3& direction = { 0.0f, 0.0f, 0.0f });

public:
	// Inherited via Drawable
	virtual void draw() override;
	virtual void update() override;

	// Inherited via Light
	virtual void print(std::ostream& os) const override;

	// Getters and setters
	static void setDebugShader(const std::shared_ptr<Shader>& debugShader);
	static void setDebugModel(const std::shared_ptr<Model>& debugModel);

	const Vector3& getSpecularColor() const;
	void setSpecularcolor(const Vector3& specularColor);

	const Vector3& getDiffuseColor() const;
	void setDiffuseColor(const Vector3& diffuseColor);

	GLfloat getSpecPower() const;
	void setSpecPower(const GLfloat specPower);

	const GLint getId() const;
	void setId(const GLint id);

	const GLint getLightType() const;

	const Vector3& getDirection() const;
	void setDirection(const Vector3& direction);

	const Matrix& getDebugModelMatrix() const;

	const GLint getAObj() const;
	void setAObj(const GLint aObj);

	virtual ~NormalLight() = 0;
};

