#pragma once
#include "AxisModel.h"
#include "Drawable.h"
#include "Texture.h"
#include "Shader.h"
#include "Model.h"
#include <string>
#include <vector>
#include <memory>

class SceneObject : public Drawable
{
public:
	// Axis Shader
	inline static std::shared_ptr<Shader> axisShader;

	// Object Types
	enum Type {
		NORMAL,
		TERRAIN,
		SKYBOX,
		FIRE
	};

protected:
	// Object id
	GLint id;

	// Object type
	Type type;

	// Object name
	std::string name;

	// Reflects Skybox
	GLboolean reflection;

	// Modified lazily if 'modified' flag is true
	Matrix modelMatrix;

	// Flag stating if pos/rot/sca was modified
	GLboolean modified;

	// Pointer to its model
	std::shared_ptr<Model> model;

	// Pointer to its shader
	std::shared_ptr<Shader> shader;

	// Wiring flag
	GLboolean wiredFormat;

	// Vector3 object properties
	Vector3 position, rotation, scale, color, followingCamera, offset;

	// Pointer to its textures
	std::vector<std::shared_ptr<Texture>> textures;

	void drawAxis();

public:
	// Constructors
	SceneObject(GLint id);
	SceneObject(GLint id, Type type);

	// Virtual destructor
	virtual ~SceneObject();

	// Draws the object on screen
	virtual void draw() override;

	// Common data to all scene objects
	virtual void sendCommonData();

	// Update objects
	virtual void update() override;

	// Convert const char * to SceneObject::Type
	static Type atot(const char* type);

	// Getters and setters
	GLint getId() const;
	void setId(GLint id);

	Type getType() const;

	std::string getName() const;
	void setName(std::string name);

	std::shared_ptr<Model> getModel();
	void setModel(std::shared_ptr<Model> model);

	std::shared_ptr<Shader> getShader();
	void setShader(std::shared_ptr<Shader> shader);

	GLboolean getWiredFormat() const;
	void setWiredFormat(GLboolean wiredFormat);

	GLboolean getReflection() const;
	void setReflection(GLboolean reflection);

	virtual Matrix& getModelMatrix();

	Vector3& getFollowingCamera();
	void setFollowingCamera(const Vector3& followingCamera);

	Vector3& getPosition();
	void setPosition(const Vector3& position);

	Vector3& getRotation();
	void setRotation(const Vector3& rotation);

	Vector3& getScale();
	void setScale(const Vector3& scale);

	Vector3& getColor();
	void setColor(const Vector3& color);

	std::vector<std::shared_ptr<Texture>>& getTextures();
	void setTextures(const std::vector<std::shared_ptr<Texture>>& textures);
};