#include "stdafx.h"
#include "ResourceManager.h"
#include "Logger.h"

ResourceManager* ResourceManager::resManInstance = nullptr;

ResourceManager::ResourceManager() {}

ResourceManager* ResourceManager::getInstance()
{
	if (!resManInstance) {
		resManInstance = new ResourceManager();
	}
	return resManInstance;
}

void ResourceManager::init(ResourceAdapter* adapter)
{
	// Get resources from adapter
	modelResMap = adapter->getModelResources();
	Logger::d("Fetched model data.");

	shaderResMap = adapter->getShaderResources();
	Logger::d("Fetched shader data.");

	textureResMap = adapter->getTextureResources();
	Logger::d("Fetched texture data.");

	// The adapter did its job :)
	delete adapter;
}

void ResourceManager::freeResources()
{
	modelResMap.clear();
	shaderResMap.clear();
	textureResMap.clear();
}

ResourceManager::~ResourceManager()
{
	freeResources();
}

template<typename Type>
std::shared_ptr<Type> ResourceManager::load(GLint id) {
	throw std::runtime_error{ " ------ ! No specialization was made for this. ! ------ " };
}

template<>
std::shared_ptr<Model> ResourceManager::load<Model>(GLint id) {
	if (modelMap.find(id) != modelMap.end()) {
		return modelMap[id];
	}
	else if (modelResMap.find(id) != modelResMap.end()) {
		std::shared_ptr<Model> model = std::make_shared<Model>(modelResMap[id]);
		model->load();
		modelMap[id] = model;
		return model;
	}
	else {
		throw std::runtime_error{ "Model invalid index was detected: " };
	}
}

std::shared_ptr<Model> ResourceManager::load(GLint id, const Vector3& collisionBoxColor) {
	if (modelMap.find(id) != modelMap.end()) {
		return modelMap[id];
	}
	else if (modelResMap.find(id) != modelResMap.end()) {
		std::shared_ptr<Model> model = std::make_shared<Model>(modelResMap[id], collisionBoxColor);
		model->load();
		modelMap[id] = model;
		return model;
	}
	else {
		throw std::runtime_error{ "Model invalid index was detected: " };
	}
}

template<>
std::shared_ptr<Shader> ResourceManager::load<Shader>(GLint id) {
	if (shaderMap.find(id) != shaderMap.end()) {
		return shaderMap[id];
	}
	else if (shaderResMap.find(id) != shaderResMap.end()) {
		std::shared_ptr<Shader> shader = std::make_shared<Shader>(shaderResMap[id]);
		shader->load();
		shaderMap[id] = shader;
		return shader;
	}
	else {
		throw std::runtime_error{ "Shader invalid index was detected: " };
	}
}

template<>
std::shared_ptr<Texture> ResourceManager::load<Texture>(GLint id) {
	if (textureMap.find(id) != textureMap.end()) {
		return textureMap[id];
	}
	else if (textureResMap.find(id) != textureResMap.end()) {
		std::shared_ptr<Texture> texture = std::make_shared<Texture>(textureResMap[id]);
		texture->load();
		textureMap[id] = texture;
		return texture;
	}
	else {
		throw std::runtime_error{ "Texture invalid index was detected: " };
	}
}