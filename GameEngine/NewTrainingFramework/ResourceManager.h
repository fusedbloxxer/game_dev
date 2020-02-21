#pragma once
#include "ResourceAdapter.h"
#include <unordered_map>
#include "Texture.h"
#include "Shader.h"
#include "Model.h"
#include <vector>
#include <memory>

class ResourceManager final
{
	// Private Constructor.
	ResourceManager();

	// Single instance.
	static ResourceManager* resManInstance;

	// C++ - OpenGL binding maps.
	std::unordered_map<GLint, std::shared_ptr<Model>> modelMap;
	std::unordered_map<GLint, std::shared_ptr<Shader>> shaderMap;
	std::unordered_map<GLint, std::shared_ptr<Texture>> textureMap;

	// Configuration maps.
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

	// Get Singleton Instance
	static ResourceManager* getInstance();

	// Destructor
	~ResourceManager();
};