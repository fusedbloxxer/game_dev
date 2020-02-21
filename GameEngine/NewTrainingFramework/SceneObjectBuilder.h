#pragma once
#include "IBuilder.h"
#include "SceneObject.h"

class SceneObjectBuilder : public IBuilder<SceneObject>
{
	// Object name
	std::string name;

	// Light elements
	GLfloat kdif, kspec;

	// Reflects Skybox
	GLboolean reflection;

	// Wiring flag
	GLboolean wiredFormat;

	// Pointer to its model
	std::shared_ptr<Model> model;

	// Pointer to its shader
	std::shared_ptr<Shader> shader;

	// Pointer to its normal map
	std::shared_ptr<Texture> normalMap;

	// Pointer to its trajectory
	std::shared_ptr<Trajectory> trajectory;

	// Pointer to its textures
	std::vector<std::shared_ptr<Texture>> textures;

	// Vector3 object properties
	Vector3 position, rotation, scale, color, followingCamera, offset, collisionBoxColor;

protected:
	// Object id
	GLint id;

	// Object type
	SceneObject::Type type;

	void sendCommonProperties(SceneObject* base) const;

public:
	// Constructor
	SceneObjectBuilder(SceneObject::Type type, GLint id);

	// Inherited via IBuilder
	virtual SceneObject* build() const override;

	// Setters
	SceneObjectBuilder& setKDif(GLfloat kdif);
	SceneObjectBuilder& setKSpec(GLfloat kspec);
	SceneObjectBuilder& setScale(const Vector3& scale);
	SceneObjectBuilder& setColor(const Vector3& color);
	SceneObjectBuilder& setName(const std::string& name);
	SceneObjectBuilder& setRotation(const Vector3& rotation);
	SceneObjectBuilder& setPosition(const Vector3& position);
	SceneObjectBuilder& setReflection(GLboolean reflection);
	SceneObjectBuilder& setWiredFormat(GLboolean wiredFormat);
	SceneObjectBuilder& setModel(std::shared_ptr<Model> model);
	SceneObjectBuilder& setShader(std::shared_ptr<Shader> shader);
	SceneObjectBuilder& setFollowingCamera(const Vector3& followingCamera);
	SceneObjectBuilder& setCollisionBoxColor(const Vector3& collisionBoxColor);
	SceneObjectBuilder& setNormalMap(const std::shared_ptr<Texture>& normalMap);
	SceneObjectBuilder& setTrajectory(const std::shared_ptr<Trajectory> trajectory);
	SceneObjectBuilder& setTextures(const std::vector<std::shared_ptr<Texture>>& textures);

	// Getters, if necessary!
	const Vector3& getCollisionBoxColor() const;

	// Virtual destructor
	virtual ~SceneObjectBuilder() = default;
};