#include "stdafx.h"
#include "TerrainObjectBuilder.h"
#include "RapidSceneAdapter.h"
#include "FireObjectBuilder.h"
#include "ResourceManager.h"
#include "CameraBuilder.h"
#include <sstream>
#include <fstream>
#include <string>

RapidSceneAdapter::RapidSceneAdapter(const char* sceneManagerPath)
	:Rapid{ sceneManagerPath } {}

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

#include "SceneObjectBuilderFactory.h"

Vector3 RapidSceneAdapter::loadFollowingCamera(rapidxml::xml_node<>* object) const
{
	Vector3 foll;

	if (auto follow = object->first_node("followingCamera"))
	{
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
	}

	return foll;
}

std::vector<std::shared_ptr<Texture>> RapidSceneAdapter::loadTextures(rapidxml::xml_node<>* object) const
{
	std::vector<std::shared_ptr<Texture>> texturesPtrs;

	if (auto textures = object->first_node("textures"))
	{
		for (auto texture = textures->first_node("texture"); texture; texture = texture->next_sibling())
		{
			if (auto id = texture->first_attribute("id"))
			{
				texturesPtrs.push_back(ResourceManager::getInstance()->load<Texture>(atoi(id->value())));
			}
			else
			{
				throw std::runtime_error{ "Object texture doesn't have an id in sceneManager." };
			}
		}
	}

	return texturesPtrs;
}

std::vector<std::shared_ptr<SceneObject>> RapidSceneAdapter::getSceneObjects(const Vector3& activeCamera) const
{
	auto objects = root->first_node("objects"); if (!objects) { throw std::runtime_error{ "Could not find objects in file." }; }
	std::vector<std::shared_ptr<SceneObject>> sceneObjects;

	for (auto object = objects->first_node("object"); object; object = object->next_sibling())
	{
		auto shader = object->first_node("shader"); if (!shader) { throw std::runtime_error{ "Object doesn't have a shader in sceneManager." }; }
		auto name = object->first_node("name"); if (!name) { throw std::runtime_error{ "Object doesn't have a name in sceneManager." }; }
		auto type = object->first_node("type"); if (!type) { throw std::runtime_error{ "Object doesn't have a type in sceneManager." }; }
		auto id = object->first_attribute("id"); if (!id) { throw std::runtime_error{ "Object doesn't have an id in sceneManager." }; }

		auto builder = std::unique_ptr<SceneObjectBuilder>(SceneObjectBuilderFactory::newBuilderInstance(SceneObject::atot(type->value()), atoi(id->value())));

		builder->setShader(ResourceManager::getInstance()->load<Shader>(atoi(shader->value())))
			.setReflection(object->first_node("reflection") != nullptr)
			.setPosition(loadVector(object, "position", "x", "y", "z"))
			.setRotation(loadVector(object, "rotation", "x", "y", "z"))
			.setWiredFormat(object->first_node("wired") != nullptr)
			.setScale(loadVector(object, "scale", "x", "y", "z"))
			.setColor(loadVector(object, "color", "r", "g", "b"))
			.setFollowingCamera(loadFollowingCamera(object))
			.setTextures(loadTextures(object))
			.setName(name->value());

		RapidSceneAdapter::setSpecificProperties(builder, object, activeCamera);
		sceneObjects.push_back(std::shared_ptr<SceneObject>(builder->build()));
	}

	return sceneObjects;
}

void RapidSceneAdapter::setSpecificProperties(std::unique_ptr<SceneObjectBuilder>& builder, rapidxml::xml_node<>* object, const Vector3& activeCamera) const
{
	if (auto terrain = dynamic_cast<TerrainObjectBuilder*>(builder.get()))
	{
		auto rgb = loadVector(object, "colorBind", "r", "g", "b");
		auto cells = object->first_node("cells"); if (!cells) { throw std::runtime_error{ "Could not find cells for special object." }; }
		auto size = cells->first_node("size"); if (!size) { throw std::runtime_error{ "Could not find cell-size for special object." }; }
		auto count = cells->first_node("count"); if (!count) { throw std::runtime_error{ "Could not find cell-count for special object." }; }
		auto colorBind = object->first_node("colorBind"); if (!colorBind) { throw std::runtime_error{ "Could not find colorBind in sceneManager." }; }
		auto blend = colorBind->first_node("blend"); if (!blend) { throw std::runtime_error{ "Could not find colorBind - blend property in sceneManager." }; }

		terrain->setCenter(activeCamera);
		terrain->setSideCells(atoi(count->value()));
		terrain->setCellSize(GLfloat(atof(size->value())));
		terrain->setHeight(loadVector(object, "height", "r", "g", "b"));
		terrain->setColorBind((GLuint)rgb.x, (GLuint)rgb.y, (GLuint)rgb.z, atoi(blend->value()));
	}
	else
	{
		auto model = object->first_node("model"); if (!model) { throw std::runtime_error{ "Object doesn't have a model in sceneManager." }; }
		builder->setModel(ResourceManager::getInstance()->load<Model>(atoi(model->value())));

		if (auto fire = dynamic_cast<FireObjectBuilder*>(builder.get()))
		{
			auto dispMax = object->first_node("dispMax"); if (!dispMax) { throw std::runtime_error{ "No displacement max value was found." }; }
			fire->setDispMax((GLfloat)atof(dispMax->value()));
		}
	}
}

std::unordered_map<GLint, std::shared_ptr<Camera>> RapidSceneAdapter::getCameras(GLint width, GLint height) const
{
	auto cameras = root->first_node("cameras"); if (!cameras) { throw std::runtime_error{ "No cameras were found." }; }
	std::unordered_map<GLint, std::shared_ptr<Camera>> cameraMap;

	for (auto camera = cameras->first_node("camera"); camera; camera = camera->next_sibling())
	{
		auto fov = camera->first_node("fov"); if (!fov) { throw std::runtime_error{ "No camera fov was detected for a camera in sceneManager." }; }
		auto id = camera->first_attribute("id"); if (!id) { throw std::runtime_error{ "No camera id was detected for a camera in sceneManager." }; }
		auto farP = camera->first_node("far"); if (!farP) { throw std::runtime_error{ "No camera far was detected for a camera in sceneManager." }; }
		auto type = camera->first_node("type"); if (!type) { throw std::runtime_error{ "No camera type was detected for a camera in sceneManager." }; }
		auto nearP = camera->first_node("near"); if (!nearP) { throw std::runtime_error{ "No camera near was detected for a camera in sceneManager." }; }
		auto rotation = camera->first_node("rotationSpeed"); if (!rotation) { throw std::runtime_error{ "No camera rotationSpeed in sceneManager." }; }
		auto translation = camera->first_node("translationSpeed"); if (!translation) { throw std::runtime_error{ "No camera translationSpeed in sceneManager." }; }

		auto cameraObject = CameraBuilder(atoi(camera->first_attribute("id")->value()), (GLint) width, (GLint) height)
			.setPosition(loadVector(camera, "position", "x", "y", "z"))
			.setTarget(loadVector(camera, "target", "x", "y", "z"))
			.setMoveSpeed(GLfloat(atof(translation->value())))
			.setRotateSpeed(GLfloat(atof(rotation->value())))
			.setUp(loadVector(camera, "up", "x", "y", "z"))
			.setNear(GLfloat(atof(nearP->value())))
			.setFar(GLfloat(atof(farP->value())))
			.setType(Camera::atot(type->value()))
			.setFov(GLfloat(atof(fov->value())))
			.build();

		cameraMap[cameraObject->getCameraId()] = std::shared_ptr<Camera>(cameraObject);
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