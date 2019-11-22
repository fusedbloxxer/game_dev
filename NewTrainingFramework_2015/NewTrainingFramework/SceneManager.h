#pragma once
#include "..\\Utilities\\rapidxml-1.13\\rapidxml.hpp"
#include "..\\Utilities\\utilities.h"
#include "SceneObject.h"
#include <unordered_map>
#include "Controls.h"
#include "Camera.h"
#include <string>
#include <memory>
#include <vector>

class SceneManager
{
	// Singleton Instance
	static SceneManager* scManInstance;

	// Private Constructor
	SceneManager();

	// Loading XML Configurations
	template<typename Config>
	void loadXML(rapidxml::xml_node<>* root);
	Vector3 loadXML(rapidxml::xml_node<>* root, const char* node, const char xP[2], const char yP[2], const char zP[2]) const;

	// Game Name
	std::string gameName;

	// Dimensions
	GLint width, height;
	GLboolean fullscreen;

	// Window Context
	ESContext esContext;

	// Active camera
	GLint activeCameraId;

	// Background color
	Vector3 backgroundColor;

	// Key mapping
	std::unordered_map<unsigned char, Controls> keyMap;

	// SceneObject mapping
	std::vector<std::shared_ptr<SceneObject>> sceneObjects;

	// Camera mapping
	std::unordered_map<GLint, std::shared_ptr<Camera>> cameraMap;

	friend std::ostream& operator<<(std::ostream& os, const SceneManager& sceneManager);

public:
	// Get Singleton Instance
	static SceneManager* getInstance();

	// Initializer
	void init(const char* sceneManagerPath = "..\\Resources\\XMLFiles\\sceneManager.xml");

	// Draw Scene Objects
	void draw();

	// Update Scene Objects
	void update();

	// Free resources
	void freeResources();

	// Destructor
	~SceneManager();

	// Getters and setters
	std::string getGameName() const;
	void setGameName(std::string gameName);

	GLint getWidth() const;
	void setWidth(GLint width);

	GLint getHeight() const;
	void setHeight(GLint height);

	bool isFullscreen() const;
	void setFullscreen(GLboolean fullscreen);

	GLint getActiveCameraId() const;
	void setActiveCameraId(GLint cameraId);

	Vector3& getBackgroundColor();
	void setBackgroundColor(Vector3& backColor);

	std::shared_ptr<Camera> getActiveCamera();

	std::unordered_map<unsigned char, Controls>& getControls();
	void setControls(std::unordered_map<unsigned char, Controls>& controls);

	std::unordered_map<GLint, std::shared_ptr<Camera>>& getCameras();
	void setCameras(std::unordered_map<GLint, std::shared_ptr<Camera>>& cameras);

	std::vector<std::shared_ptr<SceneObject>>& getSceneObjects();
	void setSceneObjects(std::vector<std::shared_ptr<SceneObject>>& sceneObjects);

	ESContext& getESContext();
	void setESContext(ESContext esContext);
};