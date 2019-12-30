#pragma once
#include "stdafx.h"
#include "SceneObjectBuilder.h"
#include "SceneAdapter.h"
#include "Rapid.h"

class RapidSceneAdapter final : public SceneAdapter, public Rapid
{
public:
	// Constructor
	RapidSceneAdapter(const char* sceneManagerPath);

	// Inherited via SceneAdapter
	virtual Fog getFog() const override;

	virtual Vector3 getBackground() const override;

	virtual GLint getActiveCameraId() const override;

	virtual std::string getGameTitle() const override;

	virtual std::shared_ptr<Shader> getAxis() const override;

	virtual std::tuple<GLboolean, GLint, GLint> getScreenSize() const override;

	virtual std::unordered_map<GLubyte, Controls::Type> getKeys() const override;

	virtual std::vector<std::shared_ptr<SceneObject>> getSceneObjects(const Vector3& camera) const override;

	virtual std::unordered_map<GLint, std::shared_ptr<Camera>> getCameras(GLint width, GLint height) const override;

	// Virtual destructor
	virtual ~RapidSceneAdapter() = default;

private:
	Vector3 loadFollowingCamera(rapidxml::xml_node<>* object) const;
	std::shared_ptr<Trajectory> loadTrajectory(rapidxml::xml_node<>* trajectory) const;
	std::vector<std::shared_ptr<Texture>> loadTextures(rapidxml::xml_node<>* object) const;
	Vector3 loadVector(rapidxml::xml_node<>* root, const char xP[2] = "x", const char yP[2] = "y", const char zP[2] = "z") const;
	void setSpecificProperties(std::unique_ptr<SceneObjectBuilder>& builder, rapidxml::xml_node<>* object, const Vector3& activeCamera) const;
};