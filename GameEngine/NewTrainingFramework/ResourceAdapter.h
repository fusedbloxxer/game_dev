#pragma once
#include "..\Utilities\utilities.h"
#include <unordered_map>
#include "TextureResource.h"
#include "ShaderResource.h"
#include "ModelResource.h"

class ResourceAdapter
{
public:

	virtual std::unordered_map<GLint, std::shared_ptr<ModelResource>> getModelResources() const = 0;
	
	virtual std::unordered_map<GLint, std::shared_ptr<ShaderResource>> getShaderResources() const = 0;
	
	virtual std::unordered_map<GLint, std::shared_ptr<TextureResource>> getTextureResources() const = 0;

	virtual ~ResourceAdapter() = default;
};