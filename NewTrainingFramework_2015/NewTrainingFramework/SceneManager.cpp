#include "stdafx.h"
#include "ResourceManager.h"
#include "TerrainObject.h"
#include "SceneManager.h"
#include "SkyboxObject.h"
#include <algorithm>
#include <iostream>
#include "Logger.h"
#include <fstream>
#include <sstream>
#include <string>

SceneManager* SceneManager::scManInstance = nullptr;

SceneManager::SceneManager()
	:width{}, height{}, fullscreen{ false }, backgroundColor{ Vector3{0.0f, 0.0f, 0.0f} },
	activeCameraId{ 0 }, sceneObjects{}, esContext{ nullptr } {}

Vector3 SceneManager::loadXML(rapidxml::xml_node<>* root, const char* node, const char xP[2], const char yP[2], const char zP[2]) const
{
	GLfloat x = 0.0f, y = 0.0f, z = 0.0f;

	if (auto position = root->first_node(node))
	{
		if (auto xPos = position->first_node(xP))
		{
			x = GLfloat(atof(xPos->value()));
		}
		else
		{
			throw std::runtime_error{ "X value is missing from resourceManager" };
		}

		if (auto yPos = position->first_node(yP))
		{
			y = GLfloat(atof(yPos->value()));
		}
		else
		{
			throw std::runtime_error{ "Y value is missing from resourceManager" };
		}

		if (auto zPos = position->first_node(zP))
		{
			z = GLfloat(atof(zPos->value()));
		}
		else
		{
			throw std::runtime_error{ "Z value is missing from resourceManager" };
		}
	}

	return { x, y, z }; // Default: (0, 0, 0)
}

template<>
void SceneManager::loadXML<Controls::Type>(rapidxml::xml_node<>* root)
{
	for (auto control = root->first_node("control"); control; control = control->next_sibling())
	{
		if (auto key = control->first_node("key"), action = control->first_node("action"); key && action)
		{
			keyMap[Controls::atok(key->value())] = Controls::atoc(action->value());
		}
		else
		{
			throw std::runtime_error{ "Key-Action pair is missing from controls in sceneManager." };
		}
	}
}

template<>
void SceneManager::loadXML<Camera>(rapidxml::xml_node<>* root)
{
	for (auto camera = root->first_node("camera"); camera; camera = camera->next_sibling())
	{
		std::shared_ptr<Camera> cameraPtr;

		if (auto id = camera->first_attribute("id"))
		{
			cameraPtr = std::make_shared<Camera>(width, height, atoi(camera->first_attribute("id")->value()));
		}
		else
		{
			throw std::runtime_error{ "No camera id was detected for a camera in sceneManager." };
		}

		if (auto type = camera->first_node("type"))
		{
			cameraPtr->setType(Camera::atot(type->value()));
		}
		else
		{
			throw std::runtime_error{ "No camera type was detected for a camera in sceneManager." };
		}

		cameraPtr->setPosition(loadXML(camera, "position", "x", "y", "z"));
		cameraPtr->setTarget(loadXML(camera, "target", "x", "y", "z"));
		cameraPtr->setUp(loadXML(camera, "up", "x", "y", "z"));

		if (auto translationSpeed = camera->first_node("translationSpeed"))
		{
			cameraPtr->setMoveSpeed(GLfloat(atof(translationSpeed->value())));
		}
		else
		{
			throw std::runtime_error{ "No camera translationSpeed was detected for a camera in sceneManager." };
		}

		if (auto rotationSpeed = camera->first_node("rotationSpeed"))
		{
			cameraPtr->setRotateSpeed(GLfloat(atof(rotationSpeed->value())));
		}
		else
		{
			throw std::runtime_error{ "No camera rotationSpeed was detected for a camera in sceneManager." };
		}

		if (auto fov = camera->first_node("fov"))
		{
			cameraPtr->setFov(GLfloat(atof(fov->value())));
		}
		else
		{
			throw std::runtime_error{ "No camera fov was detected for a camera in sceneManager." };
		}

		if (auto nearP = camera->first_node("near"))
		{
			cameraPtr->setNear(GLfloat(atof(nearP->value())));
		}
		else
		{
			throw std::runtime_error{ "No camera near was detected for a camera in sceneManager." };
		}

		if (auto farP = camera->first_node("far"))
		{
			cameraPtr->setFar(GLfloat(atof(farP->value())));
		}
		else
		{
			throw std::runtime_error{ "No camera far was detected for a camera in sceneManager." };
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
	else
	{
		throw std::runtime_error{ "No gameName was detected for in sceneManager." };
	}
}

template<>
void SceneManager::loadXML<GLint>(rapidxml::xml_node<>* root)
{
	if (auto activeCamera = root->first_node("activeCamera"))
	{
		GLint cameraId = atoi(activeCamera->value());
		if (cameraMap.find(cameraId) != cameraMap.end())
		{
			activeCameraId = cameraId;
		}
		else
		{
			throw std::runtime_error{ "ActiveCameraId in sceneManager doesn't exist in SceneManager class." };
		}
	}
	else
	{
		throw std::runtime_error{ "No activeCamera was detected for in sceneManager." };
	}
}

void getScreenSize(GLint& width, GLint& height);

template<>
void SceneManager::loadXML<GLboolean>(rapidxml::xml_node<>* root)
{
	if (auto screenSize = root->first_node("defaultScreenSize")) {
		auto fullPtr = screenSize->first_node("fullscreen");
		auto widthPtr = screenSize->first_node("width");
		auto heightPtr = screenSize->first_node("height");

		if (fullPtr && (widthPtr || heightPtr))
		{
			throw std::runtime_error{ "Cannot specify fullscreen and width/height at the same time!" };
		}
		else if (fullPtr)
		{
			fullscreen = true;
			getScreenSize(width, height);
		}
		else if (widthPtr && heightPtr)
		{
			fullscreen = false;
			width = atoi(widthPtr->value());
			height = atoi(heightPtr->value());
		}
		else
		{
			throw std::runtime_error{ "Could not fetch fullscreen/(width + height) properties." };
		}
	}
	else
	{
		throw std::runtime_error{ "DefaultScreenSize doesn't exist in sceneManager." };
	}
}

template<>
void SceneManager::loadXML<SceneObject>(rapidxml::xml_node<>* root)
{
	for (auto object = root->first_node("object"); object; object = object->next_sibling())
	{
		SceneObject::Type objectType;
		if (auto type = object->first_node("type"))
		{
			objectType = SceneObject::atot(type->value());
		}
		else
		{
			throw std::runtime_error{ "Object doesn't have a type in sceneManager." };
		}

		GLint objId;
		if (auto id = object->first_attribute("id"))
		{
			// TODO; handle case where multiple objects have the same id in file
			objId = atoi(id->value());
		}
		else
		{
			throw std::runtime_error{ "Object doesn't have an id in sceneManager." };
		}

		std::shared_ptr<SceneObject> sceneObjectPtr;

		switch (objectType)
		{
		case SceneObject::Type::NORMAL:
			sceneObjectPtr = std::make_unique<SceneObject>(objId);
			break;
		case SceneObject::Type::TERRAIN:
			sceneObjectPtr = std::make_unique<TerrainObject>(objId);
			break;
		case SceneObject::Type::SKYBOX:
			sceneObjectPtr = std::make_unique<SkyboxObject>(objId);
			break;
		default:
			throw std::runtime_error{ "SceneObject::Type::..... not listed here." };
		}

		if (auto name = object->first_node("name"))
		{
			sceneObjectPtr->setName(name->value());
		}
		else
		{
			throw std::runtime_error{ "Object doesn't have a name in sceneManager." };
		}

		sceneObjectPtr->setWiredFormat(object->first_node("wired") != nullptr);
		sceneObjectPtr->setPosition(loadXML(object, "position", "x", "y", "z"));
		sceneObjectPtr->setRotation(loadXML(object, "rotation", "x", "y", "z"));
		sceneObjectPtr->setScale(loadXML(object, "scale", "x", "y", "z"));
		sceneObjectPtr->setColor(loadXML(object, "color", "r", "g", "b"));

		if (auto follow = object->first_node("followingCamera"))
		{
			Vector3 foll;

			if (follow->first_node("ox"))
			{
				foll.x = 1;
			}

			if (follow->first_node("oy"))
			{
				foll.y = 1;
			}

			if (follow->first_node("oz"))
			{
				foll.z = 1;
			}

			sceneObjectPtr->setFollowingCamera(foll);
		}

		if (auto textures = object->first_node("textures"))
		{
			for (auto texture = textures->first_node("texture"); texture; texture = texture->next_sibling())
			{
				if (auto id = texture->first_attribute("id"))
				{
					sceneObjectPtr->getTextures().push_back(ResourceManager::getInstance()->load<Texture>(atoi(id->value())));
				}
				else
				{
					throw std::runtime_error{ "Object texture doesn't have an id in sceneManager." };
				}
			}
		}

		if (auto shader = object->first_node("shader"))
		{
			sceneObjectPtr->setShader(ResourceManager::getInstance()->load<Shader>(atoi(shader->value())));
		}
		else
		{
			throw std::runtime_error{ "Object doesn't have a shader in sceneManager." };
		}

		if (auto model = object->first_node("model"))
		{
			if (strcmp(model->value(), "generated") != 0)
			{
				sceneObjectPtr->setModel(ResourceManager::getInstance()->load<Model>(atoi(model->value())));
			}
			else if (auto terrain = dynamic_cast<TerrainObject*>(sceneObjectPtr.get()))
			{
				if (auto cells = object->first_node("cells"))
				{
					if (auto count = cells->first_node("count"))
					{
						terrain->setSideCells(atoi(count->value()));
					}
					else
					{
						throw std::runtime_error{ "Could not find cell-count for special object." };
					}

					if (auto size = cells->first_node("size"))
					{
						terrain->setCellSize(GLfloat(atof(size->value())));
					}
					else
					{
						throw std::runtime_error{ "Could not find cell-size for special object." };
					}
				}
				else
				{
					throw std::runtime_error{ "Could not find cells for special object." };
				}

				terrain->setHeight(loadXML(object, "height", "r", "g", "b"));

				if (auto colorBind = object->first_node("colorBind"))
				{
					if (auto blend = colorBind->first_node("blend"))
					{
						auto rgb = loadXML(object, "colorBind", "r", "g", "b");
						terrain->setColorBind(rgb.x, rgb.y, rgb.z, atoi(blend->value()));
					}
					else
					{
						throw std::runtime_error{ "Could not find colorBind - blend property in sceneManager." };
					}
				}
				else
				{
					throw std::runtime_error{ "Could not find colorBind in sceneManager." };
				}

				// Auto-generates model
				if (cameraMap.find(activeCameraId) != cameraMap.end())
				{
					terrain->setCenter(cameraMap[activeCameraId]->getPosition());
					terrain->generateModel();
				}
			}
		}
		else
		{
			throw std::runtime_error{ "Object doesn't have a model in sceneManager." };
		}

		sceneObjects.push_back(sceneObjectPtr);
	}
}

SceneManager* SceneManager::getInstance()
{
	if (scManInstance == nullptr) {
		scManInstance = new SceneManager();
	}
	return scManInstance;
}

void SceneManager::init(ESContext* esContext, const char* sceneManagerPath)
{
	this->esContext = esContext;

	std::ifstream input{ sceneManagerPath };
	if (!input.is_open()) {
		throw std::runtime_error{ "Could not open file: " + std::string(sceneManagerPath) };
	}

	std::stringstream ss; ss << input.rdbuf();
	input.close(); std::string content{ ss.str() };

	auto xmlPtr = std::make_unique<rapidxml::xml_document<>>();
	xmlPtr->parse<0>(&content[0]);
	auto root = xmlPtr->first_node();

	// Get game title
	loadXML<std::string>(root);

	// Set window title
	SetWindowTextA(esContext->hWnd, gameName.c_str());

	// Get fullscreen/screen size
	loadXML<GLboolean>(root);

	// Setting window to fullscreen / (width, height)
	esContext->width = width;
	esContext->height = height;
	glViewport(0, 0, width, height);
	SetWindowPos(esContext->hWnd, 0, 0, 0, width, height, SWP_SHOWWINDOW);
	if (fullscreen) { ShowWindow(esContext->hWnd, SW_MAXIMIZE); }

	// Get vector3 containing background colors
	backgroundColor = loadXML(root, "backgroundColor", "r", "g", "b");

	// Load key-binds to map
	loadXML<Controls::Type>(root->first_node("controls"));

	// Load cameras
	loadXML<Camera>(root->first_node("cameras"));

	// Get active camera id
	loadXML<GLint>(root);

	// Load SceneObjects
	loadXML<SceneObject>(root->first_node("objects"));

	// Load Axis Shader
	if (auto axis = root->first_node("axisShader"))
	{
		if (auto id = axis->first_attribute("id"))
		{
			SceneObject::axisShader = ResourceManager::getInstance()->load<Shader>(atoi(id->value()));
			SceneObject::axisShader->load();
		}
		else
		{
			throw std::runtime_error{ "No id was found for axis shader!" };
		}
	}
	else
	{
		throw std::runtime_error{ "No AxisShader found !" };
	}

	// Set clear background color
	glClearColor(backgroundColor.x, backgroundColor.y, backgroundColor.z, 0.0f);
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
	// in model x, y ,z
	pressed.clear();
	keyMap.clear();
	cameraMap.clear();
	sceneObjects.clear();
}

SceneManager::~SceneManager()
{
	Logger::d("Destructor was called for SceneManager.");
	freeResources();
}

void SceneManager::pressKey(GLubyte key, GLboolean isPressed)
{
	if (keyMap.find(key) != keyMap.end())
	{
		if (keyMap[key] == Controls::MODE_DEBUG)
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

void getScreenSize(GLint& width, GLint& height)
{
	RECT desktop;
	GetWindowRect(GetDesktopWindow(), &desktop);
	width = desktop.right;
	height = desktop.bottom;
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

std::unordered_map<GLubyte, GLboolean>& SceneManager::getPressedButtons()
{
	return pressed;
}

void SceneManager::setPressedButtons(std::unordered_map<GLubyte, GLboolean>& pressed)
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

template<typename Config>
void SceneManager::loadXML(rapidxml::xml_node<>* root)
{
	throw std::runtime_error{ " ------ ! No specialization was made for this. ! ------ " };
}

std::ostream& operator<<(std::ostream& os, const SceneManager& sceneManager)
{
	os << "Game Name:\n\t" << sceneManager.gameName << '\n'
		<< "\tDefault Screen Size: " << (sceneManager.fullscreen ? "FULLSCREEN" : "FALSE") << ", (" << sceneManager.width << ", " << sceneManager.height << ")\n"
		<< "\tBackground Color: (" << sceneManager.backgroundColor.x << ", " << sceneManager.backgroundColor.y << ", " << sceneManager.backgroundColor.z << ")\n"
		<< "\tActive Camera: " << sceneManager.activeCameraId << std::endl
		<< "\tControls:" << std::endl;

	std::for_each(sceneManager.keyMap.begin(), sceneManager.keyMap.end(),
		[&](const auto& e) { os << "\t\tMapId = " << e.first << ", " << e.second << std::endl; });

	os << "\tCameras:" << std::endl;

	std::for_each(sceneManager.cameraMap.begin(), sceneManager.cameraMap.end(),
		[&](const auto& e) { os << "\t\tMapId = " << e.first << ", " << *e.second << std::endl; });

	return os;
}