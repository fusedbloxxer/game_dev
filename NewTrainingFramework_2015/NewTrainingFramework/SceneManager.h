#pragma once
#include "..\\Utilities\\rapidxml-1.13\\rapidxml.hpp"
#include "..\\Utilities\\utilities.h"
#include <unordered_map>
#include "Controls.h"
#include "Camera.h"
#include <string>
#include <memory>

class SceneManager
{
	// Singleton Instance
	static SceneManager* scManInstance;

	// Private Constructor
	SceneManager();

	// Loading XML Configuration File
	// void loadXML()

	// Game Name
	std::string gameName;

	// Dimensions
	GLint width, height;

	// Background color
	Vector3 backgroundColor;

	// Key mapping
	std::unordered_map<unsigned char, Controls> keyMap;

	// Camera mapping
	std::unordered_map<GLint, std::shared_ptr<Camera>> cameraMap;

	// Active camera
	GLint activeCameraId;

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
};

