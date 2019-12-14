#pragma once
#include "stdafx.h"
#include "..\Utilities\rapidxml-1.13\rapidxml.hpp"
#include "SceneAdapter.h"

class RapidSceneAdapter final : public SceneAdapter
{
	std::string content;
	rapidxml::xml_node<>* root;
	std::unique_ptr<rapidxml::xml_document<>> xmlParser;

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
	Vector3 loadVector(rapidxml::xml_node<>* root, const char* node, const char xP[2], const char yP[2], const char zP[2]) const;
};

