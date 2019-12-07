#pragma once
#include "..\\Utilities\\rapidxml-1.13\\rapidxml.hpp"
#include <unordered_map>
#include "SceneObject.h"
#include "Controls.h"
#include "Releaser.h"
#include "Logger.h"
#include "Camera.h"
#include <string>
#include <memory>
#include <vector>

class SceneManager : public Drawable, public Releaser
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
	ESContext* esContext;

	// Active camera
	GLint activeCameraId;

	// Background color
	Vector3 backgroundColor;

	// Map containing pressed keys
	std::unordered_map<GLubyte, GLboolean> pressed;

	// Key mapping
	std::unordered_map<GLubyte, Controls::Type> keyMap;

	// SceneObject mapping
	std::vector<std::shared_ptr<SceneObject>> sceneObjects;

	// Camera mapping
	std::unordered_map<GLint, std::shared_ptr<Camera>> cameraMap;

	friend std::ostream& operator<<(std::ostream& os, const SceneManager& sceneManager);

public:
	// Get Singleton Instance
	static SceneManager* getInstance();

	// Initializer
	void init(ESContext* esContext, const char* sceneManagerPath = "..\\Resources\\XMLFiles\\sceneManager.xml");

	// Draw Scene Objects
	virtual void draw() override;

	// Update Scene Objects
	virtual void update() override;

	// Free resources
	virtual void freeResources() override;

	// Destructor
	virtual ~SceneManager();

	// Other methods
	void pressKey(GLubyte key, GLboolean isPressed);

	// Getters and setters
	std::string getGameName() const;
	void setGameName(std::string gameName);

	GLint getWidth() const;
	void setWidth(GLint width);

	GLint getHeight() const;
	void setHeight(GLint height);

	ESContext* getESContext();
	void setESContext(ESContext* esContext);

	bool debug();

	bool isFullscreen() const;
	void setFullscreen(GLboolean fullscreen);

	GLint getActiveCameraId() const;
	void setActiveCameraId(GLint cameraId);

	Vector3& getBackgroundColor();
	void setBackgroundColor(Vector3& backColor);

	std::shared_ptr<Camera> getActiveCamera();

	std::unordered_map<GLubyte, GLboolean>& getPressedButtons();
	void setPressedButtons(std::unordered_map<GLubyte, GLboolean>& pressed);

	std::unordered_map<unsigned char, Controls::Type>& getControls();
	void setControls(std::unordered_map<unsigned char, Controls::Type>& controls);

	std::unordered_map<GLint, std::shared_ptr<Camera>>& getCameras();
	void setCameras(std::unordered_map<GLint, std::shared_ptr<Camera>>& cameras);

	std::vector<std::shared_ptr<SceneObject>>& getSceneObjects();
	void setSceneObjects(std::vector<std::shared_ptr<SceneObject>>& sceneObjects);
};