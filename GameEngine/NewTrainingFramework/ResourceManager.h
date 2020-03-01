#pragma once
#include "ResourceAdapter.h"
#include <unordered_map>
#include "Releaser.h"
#include "Texture.h"
#include "Shader.h"
#include "Model.h"
#include "Sound.h"
#include <vector>
#include <memory>

class ResourceManager final : public Releaser
{
	// Private Constructor.
	ResourceManager();

	// Single instance.
	static ResourceManager* resManInstance;

	// C++ - OpenGL binding maps.
	std::unordered_map<GLint, std::shared_ptr<Sound>> soundMap;
	std::unordered_map<GLint, std::shared_ptr<Model>> modelMap;
	std::unordered_map<GLint, std::shared_ptr<Shader>> shaderMap;
	std::unordered_map<GLint, std::shared_ptr<Texture>> textureMap;

	// Configuration maps.
	std::unordered_map<GLint, std::shared_ptr<SoundResource>> soundResMap;
	std::unordered_map<GLint, std::shared_ptr<ModelResource>> modelResMap;
	std::unordered_map<GLint, std::shared_ptr<ShaderResource>> shaderResMap;
	std::unordered_map<GLint, std::shared_ptr<TextureResource>> textureResMap;

public:
	// Load configuration file.
	void init(ResourceAdapter* adapter);

	// Free resources.
	void freeResources();

	// Load 
	template<typename Type>
	std::shared_ptr<Type> load(GLint id);

	// Load model with collision box color being specified
	std::shared_ptr<Model> load(GLint id, const Vector3& collisionBoxColor);

	// Load sound with specific properties
	std::shared_ptr<Sound> load(GLint id, const FMOD_MODE loopMode, const bool isOverlapping);

	// Get Singleton Instance
	static ResourceManager* getInstance();

	// Destructor
	virtual ~ResourceManager();

private:
	// Load helper
	template<typename Loader, typename Resource>
	auto loadHelper(GLint id, std::unordered_map<GLint, std::shared_ptr<Loader>>& loadMap, std::unordered_map<GLint, std::shared_ptr<Resource>>& resMap)->std::shared_ptr<Loader>;
};