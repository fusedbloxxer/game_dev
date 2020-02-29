#include "stdafx.h"
#include "RapidResourceAdapter.h"

RapidResourceAdapter::RapidResourceAdapter(const char* resourceManagerPath)
	:Rapid{ resourceManagerPath } {}

std::unordered_map<GLint, std::shared_ptr<SoundResource>> RapidResourceAdapter::getSoundResources() const
{
	std::unordered_map<GLint, std::shared_ptr<SoundResource>> soundResMap;

	loadHelper(root, "sounds", "sound", [&](rapidxml::xml_node<>* sound, const char* path, GLint id) -> void {
		soundResMap[id] = std::make_shared<SoundResource>(id, path + std::string{ sound->value() });
		});

	return soundResMap;
}

std::unordered_map<GLint, std::shared_ptr<ModelResource>> RapidResourceAdapter::getModelResources() const
{
	std::unordered_map<GLint, std::shared_ptr<ModelResource>> modelResMap;

	loadHelper(root, "models", "model", [&](rapidxml::xml_node<>* model, const char* path, GLint id) -> void {
		auto file = model->first_node("file"); if (!file) { throw std::runtime_error{ "No file was specified." }; }
		modelResMap[id] = std::make_shared<ModelResource>(id, path + std::string{ file->value() });
		});

	return modelResMap;
}

std::unordered_map<GLint, std::shared_ptr<ShaderResource>> RapidResourceAdapter::getShaderResources() const
{
	std::unordered_map<GLint, std::shared_ptr<ShaderResource>> shaderResMap;

	loadHelper(root, "shaders", "shader", [&](rapidxml::xml_node<>* shader, const char* path, GLint id) -> void {
		auto vs = shader->first_node("vs"); if (!vs) { throw std::runtime_error{ "Invalid vertex shader." }; }
		auto fs = shader->first_node("fs"); if (!fs) { throw std::runtime_error{ "Invalid fragment shader." }; }

		shaderResMap[id] = std::make_shared<ShaderResource>(id, path + std::string{ vs->value() },
			path + std::string{ fs->value() });
		});

	return shaderResMap;
}

std::unordered_map<GLint, std::shared_ptr<TextureResource>> RapidResourceAdapter::getTextureResources() const
{
	std::unordered_map<GLint, std::shared_ptr<TextureResource>> textureResMap;

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

	return textureResMap;
}

/*
	@param root   - the xml root
	@param first  - the first  <tag/> - specifies the dataType
	@param second - the second <tag/> - specifies the folderPath
	@param fun	  - function pointer/functor/lambda which receives the folder node, path value and id
				  - specify how it should proceed with addinional data
*/
template<typename Fun>
void RapidResourceAdapter::loadHelper(rapidxml::xml_node<>* root, const char* first, const char* second, Fun fun) const
{
	const auto& objects = root->first_node(first); if (!objects) { throw std::runtime_error{ std::string{ "No " } +first + " were detected." }; }

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