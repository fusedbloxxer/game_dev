#include "ResourceManager.h"
#include <filesystem>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <numeric>
#include <cstdlib>
#include <string>

ResourceManager::ResourceManager()
{
	// TODO;
}

void ResourceManager::freeResources()
{
	// TODO;
	modelResMap.clear();
	shaderResMap.clear();
	textureResMap.clear();
}

ResourceManager::~ResourceManager()
{
	// TODO;
}

template<typename Type>
std::shared_ptr<Type> ResourceManager::load(GLint id) {
	std::cout << " ------ ! No specialization was made for this. ! ------ ";
	abort();
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
		std::cout << "Invalid index was detected: " << id << std::endl;
		abort();
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
		std::cout << "Invalid index was detected: " << id << std::endl;
		abort();
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
		std::cout << "Invalid index was detected: " << id << std::endl;
		abort();
	}
}

ResourceManager* ResourceManager::resManInstance = nullptr;

void ResourceManager::loadXML(rapidxml::xml_node<>* root) {
	if (root != nullptr) {
		for (auto type = root->first_node(); type; type = type->next_sibling()) {

			// For each folder
			for (auto folder = type->first_node(); folder; folder = folder->next_sibling()) {

				// For each element
				for (auto element = folder->first_node(); element; element = element->next_sibling()) {
					GLint id = std::atoi(element->first_attribute()->value());

					if (strcmp(type->name(), "models") == 0)
					{
						modelResMap[id] = std::make_shared<ModelResource>(id, folder->first_attribute("path")->value() + std::string{ element->first_node()->value() });
					}
					else if (strcmp(type->name(), "shaders") == 0)
					{
						shaderResMap[id] = std::make_shared<ShaderResource>(id, folder->first_attribute("path")->value() + std::string{ element->first_node("vs")->value() },
							folder->first_attribute("path")->value() + std::string{ element->first_node("fs")->value() });
					}
					else if (strcmp(type->name(), "textures") == 0)
					{
						textureResMap[id] = std::make_shared<TextureResource>(id, element->first_attribute("type")->value(),
							folder->first_attribute("path")->value() + std::string{ element->first_node("file")->value() },
							element->first_node("min_filter")->value(), element->first_node("mag_filter")->value(),
							element->first_node("wrap_s")->value(), element->first_node("wrap_s")->value());
					}
				}
			}
		}
	}
}

void ResourceManager::Init(const char* resourceManagerPath)
{
	std::ifstream input{ resourceManagerPath };

	// Current path
	// std::cout << std::filesystem::current_path() << '\n';

	if (!input.is_open()) {
		printf("Failed to open file: %s\n", resourceManagerPath);
		abort();
	}

	std::stringstream ss; ss << input.rdbuf();
	input.close();

	std::string content{ ss.str() };
	auto resDoc = std::make_unique<rapidxml::xml_document<>>();
	resDoc->parse<0>(&content[0]);

	loadXML(resDoc->first_node());

	std::for_each(modelResMap.begin(), modelResMap.end(),
		[](const auto& x) { std::cout << x.first << " = " << x.second->file << std::endl; });

	std::for_each(shaderResMap.begin(), shaderResMap.end(),
		[](const auto& x) { std::cout << x.first << " = [" << x.second->vsShader << ", " << x.second->fsShader << "]" << std::endl; });

	std::for_each(textureResMap.begin(), textureResMap.end(),
		[](const auto& x) { std::cout << x.first << " = [" << x.second->type << ", " << x.second->file << ", "
		<< x.second->minFilter << ", " << x.second->magFilter << ", " << x.second->wrapS
		<< ", " << x.second->wrapT << "]\n"; });
}

ResourceManager* ResourceManager::getInstance()
{
	if (!resManInstance) {
		resManInstance = new ResourceManager();
	}
	return resManInstance;
}