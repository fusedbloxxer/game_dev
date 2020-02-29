#pragma once
#include "ResourceAdapter.h"
#include "Rapid.h"

class RapidResourceAdapter final : public ResourceAdapter, public Rapid
{
public:
	// Constructor
	RapidResourceAdapter(const char* resourceManagerPath);

	// Inherited via ResourceAdapter
	virtual std::unordered_map<GLint, std::shared_ptr<SoundResource>> getSoundResources() const override;
	virtual std::unordered_map<GLint, std::shared_ptr<ModelResource>> getModelResources() const override;
	virtual std::unordered_map<GLint, std::shared_ptr<ShaderResource>> getShaderResources() const override;
	virtual std::unordered_map<GLint, std::shared_ptr<TextureResource>> getTextureResources() const override;
	
	// Virtual destructor
	virtual ~RapidResourceAdapter() = default;

private:
	template<typename Fun>
	void loadHelper(rapidxml::xml_node<>* root, const char* first, const char* second, Fun fun) const;
};