#include "stdafx.h"
#include "Texture.h"
#include "Logger.h"

Texture::Texture(std::shared_ptr<TextureResource> tr)
	:textureId{}, tr{ tr } {}

Texture& Texture::init(std::shared_ptr<TextureResource> tr)
{
	freeResources();
	this->tr = tr;
	return *this;
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

	if (tr->type == GL_TEXTURE_2D)
	{
		// Get 2D image
		glTexImage2D(tr->type, 0, (bpp == 24) ? GL_RGB : GL_RGBA, width, height, 0, (bpp == 24) ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, chr);
	}
	else if (tr->type == GL_TEXTURE_CUBE_MAP)
	{
		// Varianta 1:
		GLint bytes = bpp / 8;
		GLint widthFace = width / 4;
		GLint heightFace = height / 3;
		GLint size = widthFace * heightFace * bytes + 1;
		GLchar* sbuff = new GLchar[size];

		// 6, 4, 1, 9, 5, 7
		fetchData(GL_TEXTURE_CUBE_MAP_POSITIVE_X, sbuff, size, chr + (2 * widthFace + width * heightFace) * bytes, bytes, heightFace, width, widthFace);
		fetchData(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, sbuff, size, chr + width * heightFace * bytes, bytes, heightFace, width, widthFace);
		fetchData(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, sbuff, size, chr + widthFace * bytes, bytes, heightFace, width, widthFace);
		fetchData(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, sbuff, size, chr + (2 * width * heightFace + widthFace) * bytes, bytes, heightFace, width, widthFace);
		fetchData(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, sbuff, size, chr + (width * heightFace + widthFace) * bytes, bytes, heightFace, width, widthFace);
		fetchData(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, sbuff, size, chr + (width * heightFace + 3 * widthFace) * bytes, bytes, heightFace, width, widthFace);

		delete[] sbuff;
	}

	// Free TGA memory
	delete[] chr;
	holdsResources = true;
}

void Texture::fetchData(GLenum target, GLchar* const sbuff, GLint sz, const GLchar* const buff, const GLint bytes, const GLint hf, const GLint W, const GLint wf)
{
	memset(sbuff, 0, sz * sizeof(GLchar));
	for (GLint line = 0; line < hf; ++line)
	{
		strncat_s(sbuff, sz * sizeof(GLchar), buff + line * W * bytes, wf * bytes);
	}

	glTexImage2D(target, 0, (bytes == 3) ? GL_RGB : GL_RGBA, wf, hf, 0, (bytes == 3) ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, sbuff);
}

void Texture::freeResources()
{
	if (holdsResources) {
		glDeleteTextures(1, &textureId);
		holdsResources = false;
	}
}

Texture::~Texture()
{
	freeResources();
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