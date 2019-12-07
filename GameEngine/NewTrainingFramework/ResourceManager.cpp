#include "stdafx.h"
#include "ResourceManager.h"
#include "Logger.h"
#include <fstream>
#include <sstream>

ResourceManager* ResourceManager::resManInstance = nullptr;

ResourceManager::ResourceManager() {}

ResourceManager* ResourceManager::getInstance()
{
	if (!resManInstance) {
		resManInstance = new ResourceManager();
	}
	return resManInstance;
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

template<typename Fun>
void ResourceManager::loadHelper(rapidxml::xml_node<>* root, const char* first, const char* second, Fun fun)
{
	auto objects = root->first_node(first); if (!objects) { throw std::runtime_error{ std::string{ "No " } +first + " were detected." }; }

	for (auto folder = objects->first_node("folder"); folder; folder = folder->next_sibling())
	{
		auto path = folder->first_attribute("path"); if (!path) { throw std::runtime_error{ "No path was specified for folder." }; }

		for (auto object = folder->first_node(second); object; object = object->next_sibling())
		{
			auto idA = object->first_attribute("id"); if (!idA) { throw std::runtime_error{ "No id was specified." }; }
			fun(object, path->value(), std::atoi(idA->value()));
		}
	}
}

void ResourceManager::loadXML(rapidxml::xml_node<>* root) {
	if (!root) { throw std::runtime_error{ "No resource manager was detected." }; }

	loadHelper(root, "models", "model", [&](rapidxml::xml_node<>* model, const char* path, GLint id) -> void {
		auto file = model->first_node("file"); if (!file) { throw std::runtime_error{ "No file was specified." }; }
		modelResMap[id] = std::make_shared<ModelResource>(id, path + std::string{ file->value() });
		});

	loadHelper(root, "shaders", "shader", [&](rapidxml::xml_node<>* shader, const char* path, GLint id) -> void {
		auto vs = shader->first_node("vs"); if (!vs) { throw std::runtime_error{ "Invalid vertex shader." }; }
		auto fs = shader->first_node("fs"); if (!fs) { throw std::runtime_error{ "Invalid fragment shader." }; }

		shaderResMap[id] = std::make_shared<ShaderResource>(id, path + std::string{ vs->value() },
			path + std::string{ fs->value() });
		});

	loadHelper(root, "textures", "texture", [&](rapidxml::xml_node<>* texture, const char* path, GLint id) -> void {
		auto file = texture->first_node("file"); if (!file) { throw std::runtime_error{ "Invalid file." }; }
		auto type = texture->first_attribute("type"); if (!type) { throw std::runtime_error{ "Invalid type." }; }
		auto min = texture->first_node("min_filter"); if (!min) { throw std::runtime_error{ "Invalid min_filter." }; }
		auto mag = texture->first_node("mag_filter"); if (!mag) { throw std::runtime_error{ "Invalid mag_filter." }; }
		auto wrap_s = texture->first_node("wrap_s"); if (!wrap_s) { throw std::runtime_error{ "Invalid wrap_s." }; }
		auto wrap_t = texture->first_node("wrap_t"); if (!wrap_t) { throw std::runtime_error{ "Invalid wrap_t." }; }

		textureResMap[id] = std::make_shared<TextureResource>(id, type->value(), path + std::string{ file->value() },
			min->value(), mag->value(), wrap_s->value(), wrap_t->value());
		});
}

void ResourceManager::init(const char* resourceManagerPath)
{
	std::ifstream input{ resourceManagerPath };

	if (!input.is_open()) {
		throw std::runtime_error{ "Failed to open file: " + std::string{resourceManagerPath} };
	}

	std::stringstream ss; ss << input.rdbuf();
	input.close();

	std::string content{ ss.str() };
	auto resDoc = std::make_unique<rapidxml::xml_document<>>();
	resDoc->parse<0>(&content[0]);

	loadXML(resDoc->first_node("resourceManager"));
}