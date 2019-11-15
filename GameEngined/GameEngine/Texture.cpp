#include "Texture.h"

Texture::Texture(std::shared_ptr<TextureResource> tr)
	:textureId{}, tr{ nullptr }
{
	// TODO;
}

Texture::~Texture()
{
	// TODO;
}

void Texture::load()
{
	// TODO;
}

GLint Texture::getTextureId() const
{
	return textureId;
}

void Texture::setTextureId(GLint id)
{
	textureId = id;
}

std::shared_ptr<TextureResource> Texture::getTextureResource()
{
	return tr;
}

void Texture::setTextureResource(std::shared_ptr<TextureResource> textureResource)
{
	tr = textureResource;
}