#include "stdafx.h"
#include "Texture.h"
#include <iostream>

Texture::Texture(std::shared_ptr<TextureResource> tr)
	:textureId{}, tr{ tr }
{
	// TODO;
}

Texture::~Texture()
{
	std::cout << "Texture destructor with id " << tr->id << " was called." << std::endl;
	glDeleteTextures(1, &textureId);
}

void Texture::load()
{
	GLint width, height, bpp;
	const char* chr = LoadTGA(tr->file.c_str(), &width, &height, &bpp);

	// Reserve a buffer name called textureId
	glGenTextures(1, &textureId);

	// Reserve buffer and bind it to that id;
	glBindTexture(tr->type, textureId);

	// Set parameters
	glTexParameteri(tr->type, GL_TEXTURE_MIN_FILTER, tr->minFilter);
	glTexParameteri(tr->type, GL_TEXTURE_MAG_FILTER, tr->magFilter);
	glTexParameteri(tr->type, GL_TEXTURE_WRAP_S, tr->wrapS);
	glTexParameteri(tr->type, GL_TEXTURE_WRAP_T, tr->wrapT);

	// Get 2D image
	glTexImage2D(tr->type, 0, (bpp == 24) ? GL_RGB : GL_RGBA, width, height, 0, (bpp == 24) ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, chr);

	std::cout << "Textures were loaded for id: " << tr->id << std::endl;
}

GLuint Texture::getTextureId() const
{
	return textureId;
}

void Texture::setTextureId(GLuint id)
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