#pragma once
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
	// Object Types
	enum Type {
		NORMAL,
		TERRAIN,
		SKYBOX
	};

protected:
	// Object id
	GLint id;

	// Object type
	Type type;

	// Object name
	std::string name;

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
	Vector3 position, rotation, scale, color;

	// Pointer to its textures
	std::vector<std::shared_ptr<Texture>> textures;

public:

	// Constructors
	SceneObject(GLint id);
	SceneObject(GLint id, Type type);

	// !TODO; Prevent code auto-generation
	SceneObject(const SceneObject& copy) = delete;
	SceneObject(SceneObject&& move) noexcept = delete;
	SceneObject& operator=(const SceneObject& copy) = delete;
	SceneObject& operator=(SceneObject&& move) noexcept = delete;

	// Virtual destructor
	virtual ~SceneObject();

	// Draws the object on screen
	virtual void draw() override;

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

	virtual Matrix& getModelMatrix();

	Vector3& getPosition();
	void setPosition(Vector3& position);

	Vector3& getRotation();
	void setRotation(Vector3& rotation);

	Vector3& getScale();
	void setScale(Vector3& scale);

	Vector3& getColor();
	void setColor(Vector3& color);

	std::vector<std::shared_ptr<Texture>>& getTextures();
	void setTextures(std::vector<std::shared_ptr<Texture>>& textures);
};