#include "stdafx.h"
#include "RapidSceneAdapter.h"
#include "ResourceManager.h"
#include "TerrainObject.h"
#include "SkyboxObject.h"
#include "FireObject.h"
#include <sstream>
#include <fstream>
#include <string>
#include "SceneObjectFactory.h"

RapidSceneAdapter::RapidSceneAdapter(const char* sceneManagerPath)
	:xmlParser{ std::make_unique<rapidxml::xml_document<>>() }
{
	std::ifstream input{ sceneManagerPath };

	if (!input.is_open()) {
		throw std::runtime_error{ "Could not open file: " + std::string(sceneManagerPath) };
	}

	std::stringstream ss; ss << input.rdbuf();
	input.close(); content = ss.str();
	xmlParser->parse<0>(&content[0]);
	root = xmlParser->first_node("sceneManager");
}

Fog RapidSceneAdapter::getFog() const
{
	auto fog = root->first_node("fog"); if (!fog) { throw std::runtime_error{ "No fog was found !" }; }
	auto radius = fog->first_node("radius"); if (!radius) { throw std::runtime_error{ "No radius was found." }; }
	auto fogClarityRadius = radius->first_node("r"); if (!fogClarityRadius) { throw std::runtime_error{ "No r was found." }; }
	auto fogTransiRadius = radius->first_node("R"); if (!fogTransiRadius) { throw std::runtime_error{ "No R was found." }; }
	auto color = loadVector(fog, "color", "r", "g", "b");

	return { {color.x / 256.0f, color.y / 256.0f, color.z / 256.0f},
		static_cast<GLfloat>(atof(fogClarityRadius->value())),
		static_cast<GLfloat>(atof(fogTransiRadius->value())) };
}

std::shared_ptr<Shader> RapidSceneAdapter::getAxis() const
{
	auto axis = root->first_node("axisShader"); if (!axis) { throw std::runtime_error{ "No AxisShader found !" }; }
	auto id = axis->first_attribute("id"); if (!id) { throw std::runtime_error{ "No id was found for axis shader!" }; }
	auto axisShader = ResourceManager::getInstance()->load<Shader>(atoi(id->value()));
	axisShader->load();

	return axisShader;
}

GLint RapidSceneAdapter::getActiveCameraId() const
{
	auto activeCamera = root->first_node("activeCamera"); if (!activeCamera) { throw std::runtime_error{ "No activeCamera was detected for in sceneManager." }; }
	return atoi(activeCamera->value());
}

Vector3 RapidSceneAdapter::getBackground() const
{
	return loadVector(root, "backgroundColor", "r", "g", "b");
}

std::string RapidSceneAdapter::getGameTitle() const
{
	auto name = root->first_node("gameName"); if (!name) { throw std::runtime_error{ "No gameName was detected for in sceneManager." }; }
	return name->value();
}

std::tuple<GLboolean, GLint, GLint> RapidSceneAdapter::getScreenSize() const
{
	GLint width, height;
	GLboolean fullscreen;

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
			RECT desktop;
			fullscreen = true;
			GetWindowRect(GetDesktopWindow(), &desktop);
			width = desktop.right;
			height = desktop.bottom;
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

	return { fullscreen, width, height };
}

std::unordered_map<GLubyte, Controls::Type> RapidSceneAdapter::getKeys() const
{
	auto controls = root->first_node("controls"); if (!controls) { throw std::runtime_error{ "Controls not found in file." }; }

	std::unordered_map<GLubyte, Controls::Type> keyMap;
	for (auto control = controls->first_node("control"); control; control = control->next_sibling())
	{
		auto key = control->first_node("key"); if (!key) { throw std::runtime_error{ "Key is missing." }; }
		auto action = control->first_node("action"); if (!action) { throw std::runtime_error{ "Action is missing." }; }
		keyMap[Controls::atok(key->value())] = Controls::atoc(action->value());
	}

	return keyMap;
}

std::vector<std::shared_ptr<SceneObject>> RapidSceneAdapter::getSceneObjects(const Vector3& activeCamera) const
{
	auto objects = root->first_node("objects"); if (!objects) { throw std::runtime_error{ "Could not find objects in file." }; }
	std::vector<std::shared_ptr<SceneObject>> sceneObjects;

	for (auto object = objects->first_node("object"); object; object = object->next_sibling())
	{
		auto shader = object->first_node("shader"); if (!shader) { throw std::runtime_error{ "Object doesn't have a shader in sceneManager." }; }
		auto model = object->first_node("model"); if (!model) { throw std::runtime_error{ "Object doesn't have a model in sceneManager." }; }
		auto name = object->first_node("name"); if (!name) { throw std::runtime_error{ "Object doesn't have a name in sceneManager." }; }
		auto type = object->first_node("type"); if (!type) { throw std::runtime_error{ "Object doesn't have a type in sceneManager." }; }
		auto id = object->first_attribute("id"); if (!id) { throw std::runtime_error{ "Object doesn't have an id in sceneManager." }; }

		SceneObject::Type objectType = SceneObject::atot(type->value());
		GLint objId = atoi(id->value());

		std::shared_ptr<SceneObject> sceneObjectPtr =
			std::shared_ptr<SceneObject>(SceneObjectFactory::newInstance(objectType, objId));

		// Relatia dintre scenemanager si trajectory decuplata printr-o abstractizare
		// Read trajectory -- use factory

		sceneObjectPtr->setShader(ResourceManager::getInstance()->load<Shader>(atoi(shader->value())));
		sceneObjectPtr->setReflection(object->first_node("reflection") != nullptr);
		sceneObjectPtr->setWiredFormat(object->first_node("wired") != nullptr);
		sceneObjectPtr->setPosition(loadVector(object, "position", "x", "y", "z"));
		sceneObjectPtr->setRotation(loadVector(object, "rotation", "x", "y", "z"));
		sceneObjectPtr->setScale(loadVector(object, "scale", "x", "y", "z"));
		sceneObjectPtr->setColor(loadVector(object, "color", "r", "g", "b"));
		sceneObjectPtr->setName(name->value());

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

		if (strcmp(model->value(), "generated") != 0)
		{
			sceneObjectPtr->setModel(ResourceManager::getInstance()->load<Model>(atoi(model->value())));

			if (auto fire = dynamic_cast<FireObject*>(sceneObjectPtr.get()))
			{
				auto dispMax = object->first_node("dispMax"); if (!dispMax) { throw std::runtime_error{ "No displacement max value was found." }; }
				fire->setDispMax((GLfloat)atof(dispMax->value()));
			}
		}
		else if (auto terrain = dynamic_cast<TerrainObject*>(sceneObjectPtr.get()))
		{
			auto rgb = loadVector(object, "colorBind", "r", "g", "b");
			auto cells = object->first_node("cells"); if (!cells) { throw std::runtime_error{ "Could not find cells for special object." }; }
			auto size = cells->first_node("size"); if (!size) { throw std::runtime_error{ "Could not find cell-size for special object." }; }
			auto count = cells->first_node("count"); if (!count) { throw std::runtime_error{ "Could not find cell-count for special object." }; }
			auto colorBind = object->first_node("colorBind"); if (!colorBind) { throw std::runtime_error{ "Could not find colorBind in sceneManager." }; }
			auto blend = colorBind->first_node("blend"); if (!blend) { throw std::runtime_error{ "Could not find colorBind - blend property in sceneManager." }; }

			terrain->setSideCells(atoi(count->value()));
			terrain->setCellSize(GLfloat(atof(size->value())));
			terrain->setHeight(loadVector(object, "height", "r", "g", "b"));
			terrain->setColorBind((GLuint)rgb.x, (GLuint)rgb.y, (GLuint)rgb.z, atoi(blend->value()));

			// Auto-generates model
			terrain->setCenter(activeCamera);
			terrain->generateModel();
		}
		else
		{
			throw std::runtime_error{ "Object doesn't have a model in sceneManager." };
		}

		sceneObjects.push_back(sceneObjectPtr);
	}

	return sceneObjects;
}

std::unordered_map<GLint, std::shared_ptr<Camera>> RapidSceneAdapter::getCameras(GLint width, GLint height) const
{
	auto cameras = root->first_node("cameras"); if (!cameras) { throw std::runtime_error{ "No cameras were found." }; }
	std::unordered_map<GLint, std::shared_ptr<Camera>> cameraMap;

	for (auto camera = cameras->first_node("camera"); camera; camera = camera->next_sibling())
	{
		std::shared_ptr<Camera> cameraPtr;

		auto fov = camera->first_node("fov"); if (!fov) { throw std::runtime_error{ "No camera fov was detected for a camera in sceneManager." }; }
		auto id = camera->first_attribute("id"); if (!id) { throw std::runtime_error{ "No camera id was detected for a camera in sceneManager." }; }
		auto farP = camera->first_node("far"); if (!farP) { throw std::runtime_error{ "No camera far was detected for a camera in sceneManager." }; }
		auto type = camera->first_node("type"); if (!type) { throw std::runtime_error{ "No camera type was detected for a camera in sceneManager." }; }
		auto nearP = camera->first_node("near"); if (!nearP) { throw std::runtime_error{ "No camera near was detected for a camera in sceneManager." }; }
		auto rotation = camera->first_node("rotationSpeed"); if (!rotation) { throw std::runtime_error{ "No camera rotationSpeed in sceneManager." }; }
		auto translation = camera->first_node("translationSpeed"); if (!translation) { throw std::runtime_error{ "No camera translationSpeed in sceneManager." }; }

		cameraPtr = std::make_shared<Camera>(width, height, atoi(camera->first_attribute("id")->value()));
		cameraPtr->setPosition(loadVector(camera, "position", "x", "y", "z"));
		cameraPtr->setMoveSpeed(GLfloat(atof(translation->value())));
		cameraPtr->setRotateSpeed(GLfloat(atof(rotation->value())));
		cameraPtr->setTarget(loadVector(camera, "target", "x", "y", "z"));
		cameraPtr->setUp(loadVector(camera, "up", "x", "y", "z"));
		cameraPtr->setNear(GLfloat(atof(nearP->value())));
		cameraPtr->setFar(GLfloat(atof(farP->value())));
		cameraPtr->setType(Camera::atot(type->value()));
		cameraPtr->setFov(GLfloat(atof(fov->value())));
		cameraMap[cameraPtr->getCameraId()] = cameraPtr;
	}
	return cameraMap;
}

Vector3 RapidSceneAdapter::loadVector(rapidxml::xml_node<>* root, const char* node, const char xP[2], const char yP[2], const char zP[2]) const
{
	GLfloat x = 0.0f, y = 0.0f, z = 0.0f;

	if (auto position = root->first_node(node))
	{
		auto xPos = position->first_node(xP); if (!xPos) { throw std::runtime_error{ "X value is missing from file." }; }
		auto yPos = position->first_node(yP); if (!yPos) { throw std::runtime_error{ "Y value is missing from file." }; }
		auto zPos = position->first_node(zP); if (!zPos) { throw std::runtime_error{ "Z value is missing from file." }; }
		x = GLfloat(atof(xPos->value()));
		y = GLfloat(atof(yPos->value()));
		z = GLfloat(atof(zPos->value()));
	}

	return { x, y, z };
}