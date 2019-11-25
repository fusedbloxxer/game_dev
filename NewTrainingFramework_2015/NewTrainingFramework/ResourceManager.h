#pragma once
#include "..\Utilities\rapidxml-1.13\rapidxml.hpp"
#include <unordered_map>
#include "Texture.h"
#include "Shader.h"
#include "Model.h"
#include <vector>
#include <string>
#include <memory>

class ResourceManager
{
	// Private Constructor.
	ResourceManager();

	// Single instance.
	static ResourceManager* resManInstance;

	// Load configuration file.
	void loadXML(rapidxml::xml_node<>* root);

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
	void init(const char* resourceManagerPath = "..\\Resources\\XMLFiles\\resourceManager.xml");

	// Free resources.
	void freeResources();

	// Load 
	template<typename Type>
	std::shared_ptr<Type> load(GLint id);

	// Add generated model
	// std::shared_ptr<Model> load(std::vector<)

	// Get Singleton Instance
	static ResourceManager* getInstance();

	// Destructor
	~ResourceManager();
};