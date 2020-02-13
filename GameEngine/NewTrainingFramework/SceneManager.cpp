#include "stdafx.h"
#include "SceneManager.h"

SceneManager* SceneManager::scManInstance = nullptr;

SceneManager::SceneManager() : width{}, height{}, fullscreen{ false }, backgroundColor{ Vector3{0.0f, 0.0f, 0.0f} },
activeCameraId{ 0 }, sceneObjects{}, esContext{ nullptr } {}

SceneManager* SceneManager::getInstance()
{
	if (scManInstance == nullptr) {
		scManInstance = new SceneManager();
	}
	return scManInstance;
}

void SceneManager::init(ESContext* esContext, SceneAdapter* adapter)
{
	this->esContext = esContext;

	// Get game title
	gameName = adapter->getGameTitle();

	setUpWindow(esContext, adapter);

	// Get vector3 containing background colors
	backgroundColor = adapter->getBackground();

	// Load key-binds to map
	keyMap = adapter->getKeys();

	// Load cameras
	cameraMap = adapter->getCameras(width, height);

	// Get active camera id
	activeCameraId = adapter->getActiveCameraId();

	// Load SceneObjects
	sceneObjects = adapter->getSceneObjects(getActiveCamera()->getPosition());

	// Load the Axis Shader
	SceneObject::axisShader = adapter->getAxis();

	// Load fog
	fog = adapter->getFog();

	// Load ambiental light
	ambientalLight = adapter->getAmbientLight();

	// Load other lights
	lights = adapter->getLights();

	// Set clear background color
	glClearColor(backgroundColor.x, backgroundColor.y, backgroundColor.z, 0.0f);

	// The adapter did its job :)
	delete adapter;
}

void SceneManager::setUpWindow(ESContext* esContext, SceneAdapter* adapter)
{
	// Set window title
	SetWindowTextA(esContext->hWnd, gameName.c_str());

	// Get screen size
	std::tie(fullscreen, width, height) = adapter->getScreenSize();

	// Setting window to fullscreen / (width, height)
	esContext->width = width;
	esContext->height = height;
	glViewport(0, 0, width, height);
	SetWindowPos(esContext->hWnd, 0, 0, 0, width, height, SWP_SHOWWINDOW);
	if (fullscreen) { ShowWindow(esContext->hWnd, SW_MAXIMIZE); }
}

void SceneManager::draw()
{
	for (const auto& o : sceneObjects)
	{
		o->draw();
	}
}

void SceneManager::update()
{
	auto camera = getActiveCamera();

	for (const auto& [key, isPressed] : pressed)
	{
		if (isPressed)
		{
			camera->execute(key);
		}
	}

	for (const auto& o : sceneObjects)
	{
		o->update();
	}
}

void SceneManager::freeResources()
{
	pressed.clear();
	keyMap.clear();
	cameraMap.clear();
	sceneObjects.clear();
}

SceneManager::~SceneManager()
{
	freeResources();
}

void SceneManager::pressKey(GLubyte key, GLboolean isPressed)
{
	if (keyMap.find(key) != keyMap.end())
	{
		if (keyMap[key] == Controls::Type::MODE_DEBUG)
		{
			if (isPressed)
			{
				pressed[keyMap[key]] = !pressed[keyMap[key]];
			}
		}
		else
		{
			pressed[keyMap[key]] = isPressed;
		}
	}
}

std::string SceneManager::getGameName() const
{
	return gameName;
}

void SceneManager::setGameName(std::string gameName)
{
	this->gameName = gameName;
}

GLint SceneManager::getWidth() const
{
	return width;
}

void SceneManager::setWidth(GLint width)
{
	this->width = width;
}

GLint SceneManager::getHeight() const
{
	return height;
}

void SceneManager::setHeight(GLint height)
{
	this->height = height;
}

bool SceneManager::isFullscreen() const
{
	return fullscreen;
}

void SceneManager::setFullscreen(GLboolean fullscreen)
{
	this->fullscreen = fullscreen;
}

GLint SceneManager::getActiveCameraId() const
{
	return activeCameraId;
}

void SceneManager::setActiveCameraId(GLint cameraId)
{
	if (cameraMap.find(cameraId) != cameraMap.end()) {
		this->activeCameraId = cameraId;
	}
}

Vector3& SceneManager::getBackgroundColor()
{
	return backgroundColor;
}

void SceneManager::setBackgroundColor(Vector3& backColor)
{
	this->backgroundColor = backColor;
}

std::shared_ptr<Camera> SceneManager::getActiveCamera()
{
	if (cameraMap.find(activeCameraId) != cameraMap.end())
	{
		return cameraMap[activeCameraId];
	}
	return nullptr;
}

std::shared_ptr<AmbientLight> SceneManager::getAmbientalLight()
{
	return ambientalLight;
}

void SceneManager::setAmbientalLight(const std::shared_ptr<AmbientLight>& ambientalLight)
{
	this->ambientalLight = ambientalLight;
}

const std::vector<std::shared_ptr<Light>>& SceneManager::getLights()
{
	return lights;
}

void SceneManager::setLights(const std::vector<std::shared_ptr<Light>>& lights)
{
	this->lights = lights;
}

std::unordered_map<Controls::Type, GLboolean>& SceneManager::getPressedButtons()
{
	return pressed;
}

void SceneManager::setPressedButtons(std::unordered_map<Controls::Type, GLboolean>& pressed)
{
	this->pressed = pressed;
}

std::unordered_map<unsigned char, Controls::Type>& SceneManager::getControls()
{
	return keyMap;
}

void SceneManager::setControls(std::unordered_map<unsigned char, Controls::Type>& controls)
{
	this->keyMap = controls;
}

std::unordered_map<GLint, std::shared_ptr<Camera>>& SceneManager::getCameras()
{
	return cameraMap;
}

void SceneManager::setCameras(std::unordered_map<GLint, std::shared_ptr<Camera>>& cameras)
{
	this->cameraMap = cameras;
}

std::vector<std::shared_ptr<SceneObject>>& SceneManager::getSceneObjects()
{
	return sceneObjects;
}

void SceneManager::setSceneObjects(std::vector<std::shared_ptr<SceneObject>>& sceneObjects)
{
	this->sceneObjects = sceneObjects;
}

ESContext* SceneManager::getESContext()
{
	return esContext;
}

void SceneManager::setESContext(ESContext* esContext)
{
	this->esContext = esContext;
}

bool SceneManager::debug()
{
	return pressed.find(Controls::Type::MODE_DEBUG) != pressed.end() && pressed[Controls::Type::MODE_DEBUG];
}

Fog SceneManager::getFog() const
{
	return fog;
}

void SceneManager::setFog(const Fog& fog)
{
	this->fog = fog;
}

std::ostream& operator<<(std::ostream& os, const SceneManager& sceneManager)
{
	os << "Game Name:\n\t" << sceneManager.gameName << '\n'
		<< "\tDefault Screen Size: " << (sceneManager.fullscreen ? "FULLSCREEN" : "FALSE") << ", (" << sceneManager.width << ", " << sceneManager.height << ")\n"
		<< "\tBackground Color: (" << sceneManager.backgroundColor.x << ", " << sceneManager.backgroundColor.y << ", " << sceneManager.backgroundColor.z << ")\n"
		<< "\tActive Camera: " << sceneManager.activeCameraId << std::endl
		<< "\tControls:" << std::endl;

	for (const auto& e : sceneManager.keyMap)
	{
		os << "\t\tMapId = " << e.first << ", " << Controls::ctoa(e.second) << std::endl;
	}

	os << "\tCameras:" << std::endl;

	for (const auto& e : sceneManager.cameraMap)
	{
		os << "\t\tMapId = " << e.first << ", " << *e.second << std::endl;
	}

	return os;
}