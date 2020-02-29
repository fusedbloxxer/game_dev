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
	soundResMap = adapter->getSoundResources();

	modelResMap = adapter->getModelResources();

	shaderResMap = adapter->getShaderResources();

	textureResMap = adapter->getTextureResources();

	delete adapter;
}

void ResourceManager::freeResources()
{
	soundMap.clear();
	modelMap.clear();
	shaderMap.clear();
	textureMap.clear();
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
std::shared_ptr<Sound> ResourceManager::load<Sound>(GLint id) {
	return loadHelper<Sound, SoundResource>(id, soundMap, soundResMap);
}

template<>
std::shared_ptr<Model> ResourceManager::load<Model>(GLint id) {
	return loadHelper<Model, ModelResource>(id, modelMap, modelResMap);
}

template<>
std::shared_ptr<Shader> ResourceManager::load<Shader>(GLint id) {
	return loadHelper<Shader, ShaderResource>(id, shaderMap, shaderResMap);
}

template<>
std::shared_ptr<Texture> ResourceManager::load<Texture>(GLint id) {
	return loadHelper<Texture, TextureResource>(id, textureMap, textureResMap);
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

template<typename Loader, typename Resource>
auto ResourceManager::loadHelper(GLint id, std::unordered_map<GLint, std::shared_ptr<Loader>>& loadMap, std::unordered_map<GLint, std::shared_ptr<Resource>>& resMap) -> std::shared_ptr<Loader>
{
	if (loadMap.find(id) != loadMap.end()) {
		return loadMap[id];
	}
	else if (resMap.find(id) != resMap.end()) {
		std::shared_ptr<Loader> loader = std::make_shared<Loader>(resMap[id]);
		loader->load();
		loadMap[id] = loader;
		return loader;
	}
	else {
		throw std::runtime_error{ "Loader invalid index was detected." };
	}
}