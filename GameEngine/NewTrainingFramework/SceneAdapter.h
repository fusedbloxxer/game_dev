#pragma once
#include "..\Utilities\utilities.h"
#include "AmbientLight.h"
#include "SceneObject.h"
#include <unordered_map>
#include "PointLight.h"
#include "Controls.h"
#include "Camera.h"
#include <vector>
#include <memory>
#include <tuple>
#include "Fog.h"

class SceneAdapter
{
public:
	// Methods to implement
	virtual Fog getFog() const = 0;
	
	virtual Vector3 getBackground() const = 0;

	virtual GLint getActiveCameraId() const = 0;
	
	virtual std::string getGameTitle() const = 0;

	virtual std::shared_ptr<Shader> getAxis() const = 0;

	virtual std::shared_ptr<AmbientLight> getAmbientLight() const = 0;
	
	virtual std::tuple<GLboolean, GLint, GLint> getScreenSize() const = 0;
	
	virtual std::unordered_map<GLubyte, Controls::Type> getKeys() const = 0;

	virtual std::vector<std::shared_ptr<PointLight>> getNormalLights() const = 0;

	virtual std::vector<std::shared_ptr<SceneObject>> getSceneObjects(const Vector3& camera) const = 0;

	virtual std::unordered_map<GLint, std::shared_ptr<Camera>> getCameras(GLint width, GLint height) const = 0;

	// Virtual destructor
	virtual ~SceneAdapter() = default;
};