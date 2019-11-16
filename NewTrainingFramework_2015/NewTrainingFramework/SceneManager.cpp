#include "stdafx.h"
#include "SceneManager.h"
#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <string>

SceneManager* SceneManager::scManInstance = nullptr;

SceneManager::SceneManager()
	:width{ 960 }, height{ 720 }, isFullscreen{ false }, backgroundColor{ Vector3{0.0f, 0.0f, 0.0f} }
{
	// TODO;
}

std::ostream& operator<<(std::ostream& os, const SceneManager& sceneManager)
{
	os << "Game Name:\n\t" << sceneManager.gameName << '\n'
		<< "\tDefault Screen Size: " << (sceneManager.isFullscreen ? "FULLSCREEN" : "FALSE") << ", (" << sceneManager.width << ", " << sceneManager.height << ")\n"
		<< "\tBackground Color: (" << sceneManager.backgroundColor.x << ", " << sceneManager.backgroundColor.y << ", " << sceneManager.backgroundColor.z << ")\n"
		<< "\tActive Camera: " << sceneManager.activeCameraId << std::endl
		<< "\tControls:" << std::endl;

	std::for_each(sceneManager.keyMap.begin(), sceneManager.keyMap.end(),
		[](const auto& e) { std::cout << "\t\tMapId = " << e.first << ", " << e.second << std::endl; });

	os << "\tCameras:" << std::endl;

	std::for_each(sceneManager.cameraMap.begin(), sceneManager.cameraMap.end(),
		[](const auto& e) { std::cout << "\t\tMapId = " << e.first << ", " << *e.second << std::endl; });

	return os;
}

Vector3 SceneManager::loadXML(rapidxml::xml_node<>* root, const char* node, const char coord[6]) const
{
	GLfloat x = 0.0f, y = 0.0f, z = 0.0f;

	if (auto position = root->first_node(node))
	{
		if (auto xPos = position->first_node(&coord[0]))
		{
			x = atof(xPos->value());
		}

		if (auto yPos = position->first_node(&coord[2]))
		{
			y = atof(yPos->value());
		}

		if (auto zPos = position->first_node(&coord[4]))
		{
			z = atof(zPos->value());
		}
	}

	return { x, y, z };
}


template<typename Config>
void SceneManager::loadXML(rapidxml::xml_node<>* root)
{
	std::cerr << " ------ ! No specialization was made for this. ! ------ ";
	abort();
}

template<>
void SceneManager::loadXML<Controls>(rapidxml::xml_node<>* root)
{
	for (auto control = root->first_node("control"); control; control = control->next_sibling())
	{
		if (auto key = control->first_node("key"), action = control->first_node("action"); key && action)
		{
			keyMap[key->value()[0]] = atoc(action->value());
		}
	}
}

template<>
void SceneManager::loadXML<Camera>(rapidxml::xml_node<>* root)
{
	for (auto camera = root->first_node("camera"); camera; camera = camera->next_sibling())
	{
		std::shared_ptr<Camera> cameraPtr = std::make_shared<Camera>(atoi(camera->first_attribute("id")->value()));

		if (auto type = camera->first_node("type"))
		{
			cameraPtr->setType(Camera::atot(type->value()));
		}

		cameraPtr->setPosition(loadXML(camera, "position", "x\0y\0z"));
		cameraPtr->setTarget(loadXML(camera, "target", "x\0y\0z"));
		cameraPtr->setUp(loadXML(camera, "up", "x\0y\0z"));

		if (auto translationSpeed = camera->first_node("translationSpeed"))
		{
			cameraPtr->setMoveSpeed(atof(translationSpeed->value()));
		}

		if (auto rotationSpeed = camera->first_node("rotationSpeed"))
		{
			cameraPtr->setRotateSpeed(atof(rotationSpeed->value()));
		}

		if (auto fov = camera->first_node("fov"))
		{
			cameraPtr->setFov(atof(fov->value()));
		}

		if (auto nearP = camera->first_node("near"))
		{
			cameraPtr->setNear(atof(nearP->value()));
		}

		if (auto farP = camera->first_node("far"))
		{
			cameraPtr->setFar(atof(farP->value()));
		}

		cameraMap[cameraPtr->getCameraId()] = cameraPtr;
	}
}

template<>
void SceneManager::loadXML<std::string>(rapidxml::xml_node<>* root)
{
	if (auto name = root->first_node("gameName"))
	{
		gameName = name->value();
	}
}

template<>
void SceneManager::loadXML<GLint>(rapidxml::xml_node<>* root)
{
	if (auto activeCamera = root->first_node("activeCamera"))
	{
		activeCameraId = atoi(activeCamera->value());
	}
}

template<>
void SceneManager::loadXML<GLboolean>(rapidxml::xml_node<>* root)
{
	if (auto screenSize = root->first_node("defaultScreenSize")) {
		if (screenSize->first_node("fullscreen")) {
			isFullscreen = true;
		}
		else
		{
			isFullscreen = false;
			if (auto widthPtr = screenSize->first_node("width"))
			{
				width = atoi(widthPtr->value());
			}
			if (auto heightPtr = screenSize->first_node("height"))
			{
				height = atoi(heightPtr->value());
			}
		}
	}
}

SceneManager* SceneManager::getInstance()
{
	if (scManInstance == nullptr) {
		scManInstance = new SceneManager();
	}
	return scManInstance;
}

void SceneManager::init(const char* sceneManagerPath)
{
	std::ifstream input{ sceneManagerPath };
	if (!input.is_open()) {
		std::cout << "Could't open file: " << sceneManagerPath << std::endl;
		abort();
	}

	std::stringstream ss; ss << input.rdbuf();
	input.close(); std::string content{ ss.str() };

	auto xmlPtr = std::make_unique<rapidxml::xml_document<>>();
	xmlPtr->parse<0>(&content[0]);
	auto root = xmlPtr->first_node();

	// Get game title
	loadXML<std::string>(root);

	// Get fullscreen/screen size
	loadXML<GLboolean>(root);

	// Get vector3 containing background colors
	backgroundColor = loadXML(root, "backgroundColor", "r\0g\0b");

	// Load key-binds to map
	loadXML<Controls>(root->first_node("controls"));

	// Load cameras
	loadXML<Camera>(root->first_node("cameras"));

	// Get active camera id
	loadXML<GLint>(root);

	std::cout << *this << std::endl;

	xmlPtr->clear();
}

void SceneManager::draw()
{
	// TODO
}

void SceneManager::update()
{
	// TODO;
}

void SceneManager::freeResources()
{
	// TODO;
}

SceneManager::~SceneManager()
{
	std::cout << "Destructor was called for SceneManager." << std::endl;
	freeResources();
}