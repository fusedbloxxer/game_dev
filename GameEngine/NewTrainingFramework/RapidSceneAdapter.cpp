#include "stdafx.h"
#include "TerrainObjectBuilder.h"
#include "TrajectoryFactory.h"
#include "RapidSceneAdapter.h"
#include "FireObjectBuilder.h"
#include "CircleTrajectory.h"
#include "ResourceManager.h"
#include "LineTrajectory.h"
#include "CameraBuilder.h"
#include "LightFactory.h"
#include "SpotLight.h"
#include <algorithm>
#include "Logger.h"
#include <sstream>
#include <fstream>
#include <numeric>
#include <string>

RapidSceneAdapter::RapidSceneAdapter(const char* sceneManagerPath)
	:Rapid{ sceneManagerPath } {}

Fog RapidSceneAdapter::getFog() const
{
	auto fog = root->first_node("fog"); if (!fog) { throw std::runtime_error{ "No fog was found !" }; }
	auto radius = fog->first_node("radius"); if (!radius) { throw std::runtime_error{ "No radius was found." }; }
	auto fogClarityRadius = radius->first_node("r"); if (!fogClarityRadius) { throw std::runtime_error{ "No r was found." }; }
	auto fogTransiRadius = radius->first_node("R"); if (!fogTransiRadius) { throw std::runtime_error{ "No R was found." }; }
	auto color = loadVector(fog->first_node("color"), "r", "g", "b");

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
	return loadVector(root->first_node("backgroundColor"), "r", "g", "b");
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

std::shared_ptr<Trajectory> RapidSceneAdapter::loadTrajectory(rapidxml::xml_node<>* trajectoryXml) const
{
	if (trajectoryXml)
	{
		auto type = trajectoryXml->first_attribute("type"); if (!type) { throw std::runtime_error{ "Trajectory type was not found." }; }
		std::shared_ptr<Trajectory> trajectory = std::shared_ptr<Trajectory>(TrajectoryFactory::newInstance(Trajectory::strToType(type->value())));

		if (auto itCount = trajectoryXml->first_attribute("iteration-count"))
		{
			trajectory->setItCount(itCount->value());
		}

		if (auto speed = trajectoryXml->first_attribute("speed"))
		{
			trajectory->setSpeed(static_cast<GLfloat>(::atoi(speed->value())));
		}

		switch (trajectory->getType())
		{
		case Trajectory::Type::LINEAR:
		case Trajectory::Type::LINE_LOOP:
		case Trajectory::Type::LINE_STRIP:
		{
			if (auto lineTrajectory = dynamic_cast<LineTrajectory*>(trajectory.get()))
			{
				if (auto rotationSpeed = trajectoryXml->first_attribute("rotationSpeed"))
				{
					lineTrajectory->setRotationSpeed(static_cast<GLfloat>(::atof(rotationSpeed->value())));
				}

				// Add all points.
				std::vector<Vector3> transport;

				if (auto points = trajectoryXml->first_node("points"))
				{
					for (auto point = points->first_node("point"); point; point = point->next_sibling())
					{
						transport.push_back(loadVector(point));
					}

					lineTrajectory->setPoints(transport);
				}
				else
				{
					throw std::runtime_error{ "No points were found for trajectory." };
				}

				// Common for all line trajectories.
				if (trajectory->getType() == Trajectory::Type::LINE_LOOP)
				{
					break;
				}

				// It's a varying trajectory.
				if (auto direction = trajectoryXml->first_attribute("direction"))
				{
					lineTrajectory->setDirection(direction->value());
				}
			}
		}
		break;
		case Trajectory::Type::CIRCLE:
		{
			auto circle = dynamic_cast<CircleTrajectory*>(trajectory.get());
			circle->setCenter(loadVector(trajectoryXml->first_node("center")));

			if (auto radius = trajectoryXml->first_node("radius"))
			{
				circle->setRadius(static_cast<GLfloat>(::atof(radius->value())));
			}
			else
			{
				throw std::runtime_error{ "No radius was found." };
			}

			if (auto rotationPlane = trajectoryXml->first_node("rotationPlane"))
			{
				circle->setRotationPlane(loadVector(rotationPlane));
			}
			else
			{
				throw std::runtime_error{ "No rotationPlane was found." };
			}
		}
		break;
		default:
			throw std::runtime_error{ "Trajectory type is not valid !" };
			break;
		}

		return trajectory;
	}

	return nullptr;
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
		auto kdif = object->first_node("kdif"), kspec = object->first_node("kspec");

		const auto& idValue = atoi(id->value());

		{
			auto exists = std::count_if(sceneObjects.begin(), sceneObjects.end(), [&idValue](const auto& o) { return o->getId() == idValue; }) != 0;

			if (exists)
			{
				throw std::runtime_error{ "Scene object id cannot be duplicate." };
			}
		}

		auto builder = std::unique_ptr<SceneObjectBuilder>(SceneObjectBuilderFactory::newBuilderInstance(SceneObject::atot(type->value()), idValue));

		builder->setShader(ResourceManager::getInstance()->load<Shader>(atoi(shader->value())))
			.setKSpec(kspec ? static_cast<GLfloat>(::atof(kspec->value())) : 1.0f)
			.setKDif(kdif ? static_cast<GLfloat>(::atof(kdif->value())) : 1.0f)
			.setPosition(loadVector(object->first_node("position")))
			.setRotation(loadVector(object->first_node("rotation")))
			.setScale(loadVector(object->first_node("scale")))
			.setColor(loadVector(object->first_node("color"), "r", "g", "b"))
			.setTrajectory(loadTrajectory(object->first_node("trajectory")))
			.setReflection(object->first_node("reflection") != nullptr)
			.setWiredFormat(object->first_node("wired") != nullptr)
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
		auto rgb = loadVector(object->first_node("colorBind"), "r", "g", "b");
		auto cells = object->first_node("cells"); if (!cells) { throw std::runtime_error{ "Could not find cells for special object." }; }
		auto size = cells->first_node("size"); if (!size) { throw std::runtime_error{ "Could not find cell-size for special object." }; }
		auto count = cells->first_node("count"); if (!count) { throw std::runtime_error{ "Could not find cell-count for special object." }; }
		auto colorBind = object->first_node("colorBind"); if (!colorBind) { throw std::runtime_error{ "Could not find colorBind in sceneManager." }; }
		auto blend = colorBind->first_node("blend"); if (!blend) { throw std::runtime_error{ "Could not find colorBind - blend property in sceneManager." }; }

		terrain->setCenter(activeCamera);
		terrain->setSideCells(atoi(count->value()));
		terrain->setCellSize(GLfloat(atof(size->value())));
		terrain->setHeight(loadVector(object->first_node("height"), "r", "g", "b"));
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

std::shared_ptr<AmbientLight> RapidSceneAdapter::getAmbientLight() const
{
	if (!root->first_node("lights"))
	{
		throw std::runtime_error{ "Could not find lights tag in order to fetch ambiental light." };
	}
	else if (auto ambientalLight = root->first_node("lights")->first_node("ambientalLight"))
	{
		auto ratio = ambientalLight->first_node("ratio");
		auto color = loadVector(ambientalLight->first_node("color"), "r", "g", "b");
		return std::make_shared<AmbientLight>(color, static_cast<GLfloat>(ratio ? ::atof(ratio->value()) : 1.0f));
	}
	else
	{
		throw std::runtime_error{ "Could not find ambientalLight in sceneManager." };
	}
}

std::vector<std::shared_ptr<Light>> RapidSceneAdapter::getLights() const
{
	if (auto lightsXml = root->first_node("lights"))
	{
		auto debugShaderId = lightsXml->first_node("debugShader"); if (!debugShaderId) { throw std::runtime_error{ "Lighting debugging shader was not found." }; }
		NormalLight::setDebugShader(ResourceManager::getInstance()->load<Shader>(::atoi(debugShaderId->value())));

		auto debugModelId = lightsXml->first_node("debugModel"); if (!debugModelId) { throw std::runtime_error{ "Lighting debugging model was not found." }; }
		NormalLight::setDebugModel(ResourceManager::getInstance()->load<Model>(::atoi(debugModelId->value())));

		std::vector<std::shared_ptr<Light>> lights;
		for (auto light = lightsXml->first_node("light"); light; light = light->next_sibling())
		{
			// Send everything that is needed
			auto specPower = light->first_node("specPower");
			auto asObjId = light->first_node("associatedObj");
			auto direction = loadVector(light->first_node("direction"), "x", "y", "z");
			auto diffuseColor = loadVector(light->first_node("diffuseColor"), "r", "g", "b");
			auto specularColor = loadVector(light->first_node("specularColor"), "r", "g", "b");
			auto id = light->first_attribute("id"); if (!id) { throw std::runtime_error{ "Light doesn't have an id." }; }
			auto type = light->first_node("type"); if (!type) { throw std::runtime_error{ "Light doesn't have a type." }; }
			if (strcmp(type->value(), "spotlight") == 0 && !asObjId) { throw std::runtime_error("Spotlight doesn't have an associated obj id."); }

			// Check if id doesn't already exist.
			GLint lightId = static_cast<GLint>(::atoi(id->value()));

			auto exists = std::count_if(lights.begin(), lights.end(), [&lightId](const auto& li) { return dynamic_cast<NormalLight*>(li.get())->getId() == lightId; }) != 0;

			if (exists) { throw std::runtime_error{ "Id is already in lights array." }; }

			// Initialize and insert into array.
			lights.push_back(
				LightFactory::newInstance(
					type->value(),
					lightId,
					diffuseColor,
					specularColor,
					specPower ? static_cast<GLfloat>(::atof(specPower->value())) : 0.0f,
					direction,
					asObjId ? static_cast<GLint>(::atoi(asObjId->value())) : 0
				)
			);

			if (auto spotLight = dynamic_cast<SpotLight*>(lights.back().get()))
			{
				auto innerCutoff = light->first_node("innerCutoff"); if (!innerCutoff) { throw std::runtime_error{ "Inner cutoff specifier is missing." }; }
				auto outerCutoff = light->first_node("outerCutoff"); if (!outerCutoff) { throw std::runtime_error{ "Outer cutoff specifier is missing." }; }
				spotLight->setCutoffs(::cos(TO_RAD(static_cast<GLfloat>(::atof(innerCutoff->value())))), ::cos(TO_RAD(static_cast<GLfloat>(::atof(outerCutoff->value())))));
			}
		}

		return lights;
	}
	else
	{
		throw std::runtime_error{ "Could not find ambientalLight in sceneManager." };
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

		const auto& idValue = ::atoi(id->value());

		{
			auto exists = cameraMap.find(idValue) != cameraMap.end();

			if (exists)
			{
				throw std::runtime_error{ "Camera id cannot be duplicate." };
			}
		}

		auto cameraObject = CameraBuilder(idValue, static_cast<GLfloat>(width), static_cast<GLfloat>(height))
			.setPosition(loadVector(camera->first_node("position")))
			.setTarget(loadVector(camera->first_node("target")))
			.setMoveSpeed(GLfloat(atof(translation->value())))
			.setRotateSpeed(GLfloat(atof(rotation->value())))
			.setUp(loadVector(camera->first_node("up")))
			.setNear(GLfloat(atof(nearP->value())))
			.setFar(GLfloat(atof(farP->value())))
			.setType(Camera::atot(type->value()))
			.setFov(GLfloat(atof(fov->value())))
			.build();

		cameraMap[cameraObject->getCameraId()] = std::shared_ptr<Camera>(cameraObject);
	}
	return cameraMap;
}

Vector3 RapidSceneAdapter::loadVector(rapidxml::xml_node<>* root, const char xP[2], const char yP[2], const char zP[2]) const
{
	GLfloat x = 0.0f, y = 0.0f, z = 0.0f;

	if (root)
	{
		auto xPos = root->first_node(xP); if (!xPos) { throw std::runtime_error{ "X value is missing from file." }; }
		auto yPos = root->first_node(yP); if (!yPos) { throw std::runtime_error{ "Y value is missing from file." }; }
		auto zPos = root->first_node(zP); if (!zPos) { throw std::runtime_error{ "Z value is missing from file." }; }
		x = GLfloat(atof(xPos->value()));
		y = GLfloat(atof(yPos->value()));
		z = GLfloat(atof(zPos->value()));
	}

	return { x, y, z };
}