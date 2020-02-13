#pragma once
#include <unordered_map>
#include "SceneAdapter.h"
#include "AmbientLight.h"
#include "SceneObject.h"
#include "Controls.h"
#include "Releaser.h"
#include "Camera.h"
#include "Light.h"
#include <memory>
#include <vector>
#include "Fog.h"

class SceneManager final : public Drawable, public Releaser
{
	// Singleton Instance
	static SceneManager* scManInstance;

	// Private Constructor
	SceneManager();

	// Fog
	Fog fog;

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

	// Other lights
	std::vector<std::shared_ptr<Light>> lights;

	// AmbientalLight
	std::shared_ptr<AmbientLight> ambientalLight;

	// Map containing pressed keys
	std::unordered_map<Controls::Type, GLboolean> pressed;

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
	void init(ESContext* esContext, SceneAdapter* adapter);

	// Setting up the window parameters
	void setUpWindow(ESContext* esContext, SceneAdapter* adapter);

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

	Fog getFog() const;
	void setFog(const Fog& fog);

	bool isFullscreen() const;
	void setFullscreen(GLboolean fullscreen);

	GLint getActiveCameraId() const;
	void setActiveCameraId(GLint cameraId);

	Vector3& getBackgroundColor();
	void setBackgroundColor(Vector3& backColor);

	std::shared_ptr<Camera> getActiveCamera();

	std::unordered_map<Controls::Type, GLboolean>& getPressedButtons();
	void setPressedButtons(std::unordered_map<Controls::Type, GLboolean>& pressed);

	std::unordered_map<unsigned char, Controls::Type>& getControls();
	void setControls(std::unordered_map<unsigned char, Controls::Type>& controls);

	std::unordered_map<GLint, std::shared_ptr<Camera>>& getCameras();
	void setCameras(std::unordered_map<GLint, std::shared_ptr<Camera>>& cameras);

	std::vector<std::shared_ptr<SceneObject>>& getSceneObjects();
	void setSceneObjects(std::vector<std::shared_ptr<SceneObject>>& sceneObjects);

	std::shared_ptr<AmbientLight> getAmbientalLight();
	void setAmbientalLight(const std::shared_ptr<AmbientLight>& ambientalLight);

	const std::vector<std::shared_ptr<Light>>& getLights();
	void setLights(const std::vector<std::shared_ptr<Light>>& lights);
};