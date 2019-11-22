#include "stdafx.h"
#include "ResourceManager.h"
#include "TerrainObject.h"
#include "SceneManager.h"
#include "SkyboxObject.h"
#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <string>

SceneManager* SceneManager::scManInstance = nullptr;

SceneManager::SceneManager()
	:width{ 960 }, height{ 720 }, fullscreen{ false }, backgroundColor{ Vector3{0.0f, 0.0f, 0.0f} }, activeCameraId{ 0 }, sceneObjects{}
{
	// TODO;
}

std::ostream& operator<<(std::ostream& os, const SceneManager& sceneManager)
{
	os << "Game Name:\n\t" << sceneManager.gameName << '\n'
		<< "\tDefault Screen Size: " << (sceneManager.fullscreen ? "FULLSCREEN" : "FALSE") << ", (" << sceneManager.width << ", " << sceneManager.height << ")\n"
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

Vector3 SceneManager::loadXML(rapidxml::xml_node<>* root, const char* node, const char xP[2], const char yP[2], const char zP[2]) const
{
	GLfloat x = 0.0f, y = 0.0f, z = 0.0f;

	if (auto position = root->first_node(node))
	{
		if (auto xPos = position->first_node(xP))
		{
			x = atof(xPos->value());
		}

		if (auto yPos = position->first_node(yP))
		{
			y = atof(yPos->value());
		}

		if (auto zPos = position->first_node(zP))
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
			keyMap[atok(key->value())] = atoc(action->value());
		}
	}
}

template<>
void SceneManager::loadXML<Camera>(rapidxml::xml_node<>* root)
{
	for (auto camera = root->first_node("camera"); camera; camera = camera->next_sibling())
	{
		std::shared_ptr<Camera> cameraPtr = std::make_shared<Camera>(width, height, atoi(camera->first_attribute("id")->value()));

		if (auto type = camera->first_node("type"))
		{
			cameraPtr->setType(Camera::atot(type->value()));
		}

		cameraPtr->setPosition(loadXML(camera, "position", "x", "y", "z"));
		cameraPtr->setTarget(loadXML(camera, "target", "x", "y", "z"));
		cameraPtr->setUp(loadXML(camera, "up", "x", "y", "z"));

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
			fullscreen = true;
		}
		else
		{
			fullscreen = false;
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

template<>
void SceneManager::loadXML<SceneObject>(rapidxml::xml_node<>* root)
{
	for (auto object = root->first_node("object"); object; object = object->next_sibling())
	{
		SceneObject::Type objectType = SceneObject::atot(object->first_node("type")->value());
		GLint objId = atoi(object->first_attribute("id")->value());
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
		}

		if (auto name = object->first_node("name"))
		{
			sceneObjectPtr->setName(name->value());
		}

		sceneObjectPtr->setDepthTest(object->first_node("depth") != nullptr);
		sceneObjectPtr->setWiredFormat(object->first_node("wired") != nullptr);
		sceneObjectPtr->setPosition(loadXML(object, "position", "x", "y", "z"));
		sceneObjectPtr->setRotation(loadXML(object, "rotation", "x", "y", "z"));
		sceneObjectPtr->setScale(loadXML(object, "scale", "x", "y", "z"));
		sceneObjectPtr->setColor(loadXML(object, "color", "r", "g", "b"));

		if (auto model = object->first_node("model"); model && strcmp(model->value(), "generated") != 0)
		{
			sceneObjectPtr->setModel(ResourceManager::getInstance()->load<Model>(atoi(model->value())));
		}

		if (auto shader = object->first_node("shader"))
		{
			sceneObjectPtr->setShader(ResourceManager::getInstance()->load<Shader>(atoi(shader->value())));
		}

		if (auto textures = object->first_node("textures"))
		{
			for (auto texture = textures->first_node("texture"); texture; texture = texture->next_sibling())
			{
				sceneObjectPtr->getTextures().push_back(ResourceManager::getInstance()->load<Texture>(atoi(texture->first_attribute("id")->value())));
			}
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

void SceneManager::init(const char* sceneManagerPath)
{
	// Initialize ESContext
	esInitContext(&esContext);

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

	// Handle FULLSCREEN CASE !!! + IF DEPTH !!
	// TODO;
	esCreateWindow(&esContext, gameName.c_str(), width, height, ES_WINDOW_RGB | ES_WINDOW_DEPTH);

	// Get vector3 containing background colors
	backgroundColor = loadXML(root, "backgroundColor", "r", "g", "b");

	// Load key-binds to map
	loadXML<Controls>(root->first_node("controls"));

	// Load cameras
	loadXML<Camera>(root->first_node("cameras"));

	// Load SceneObjects
	loadXML<SceneObject>(root->first_node("objects"));

	// Get active camera id
	loadXML<GLint>(root);

	// Set clear background color
	glClearColor(backgroundColor.x, backgroundColor.y, backgroundColor.z, 0.0f);

	// std::cout << *getActiveCamera() << std::endl;
	// std::cout << *this << std::endl;
}

void SceneManager::draw()
{
	/* TODO;

		Draw(), care va face setarile generale pentru desenarea unui frame si apoi va itera prin
		vectorul de obiecte, apeland Draw-ul lor.
	*/
	for (const auto& o : sceneObjects)
	{
		o->draw();
	}
}

void SceneManager::update()
{
	/* TODO;

		Update() - updatarea unor valori care nu tin in mod direct de desenarea scenei. Folosit
		de exemplu pentru preluarea coordonatelor cursorului, ca sa se observe daca s-a facut
		click pe un obiect din scena. In afara de asta, in cazul in care si obiectele au o functie de
		Update va itera prin vectorul de obiecte si va apela Update-ul lor.
	*/
	for (const auto& o : sceneObjects)
	{
		o->update();
	}
}

void SceneManager::freeResources()
{
	// TODO;
	keyMap.clear();
	cameraMap.clear();
	sceneObjects.clear();
}

SceneManager::~SceneManager()
{
	std::cout << "Destructor was called for SceneManager." << std::endl;
	freeResources();
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

std::unordered_map<unsigned char, Controls>& SceneManager::getControls()
{
	return keyMap;
}

void SceneManager::setControls(std::unordered_map<unsigned char, Controls>& controls)
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

ESContext& SceneManager::getESContext()
{
	return esContext;
}

void SceneManager::setESContext(ESContext esContext)
{
	this->esContext = esContext;
}