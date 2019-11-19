#include "stdafx.h"
#include "Shader.h"
#include <iostream>

Shader::Shader(std::shared_ptr<ShaderResource> sr)
	:programId{}, sr{ sr }, holdsResources{ false }
{
	// 

}

Shader& Shader::init(std::shared_ptr<ShaderResource> sr)
{
	freeResources();
	this->sr = sr;
	return *this;
}

Shader::~Shader()
{
	std::cout << "Shader destructor with id " << sr->id << " was called." << std::endl;
	freeResources();
}

void Shader::load()
{
	GLuint vertexShader = esLoadShader(GL_VERTEX_SHADER, const_cast<char*>(sr->vsShader.c_str()));

	if (vertexShader == 0) {
		std::cerr << "Failed to compile vertex shader for id: " << sr->id << ", vs: " << sr->vsShader << std::endl;
		abort();
	}

	GLuint fragmentShader = esLoadShader(GL_FRAGMENT_SHADER, const_cast<char*>(sr->fsShader.c_str()));

	if (fragmentShader == 0) {
		std::cerr << "Failed to compile fragment shader for id: " << sr->id << ", fs: " << sr->fsShader << std::endl;
		glDeleteShader(vertexShader);
		abort();
	}

	programId = esLoadProgram(vertexShader, fragmentShader);

	if (programId == 0) {
		std::cerr << "Failed to link program for id: " << sr->id << ", (vs, fs): (" << sr->vsShader << ", " << sr->fsShader << ")\n";
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		abort();
	}

	// Attributes
	binormAttribute = glGetAttribLocation(programId, "a_binormL");
	positionAttribute = glGetAttribLocation(programId, "a_posL");
	colorAttribute = glGetAttribLocation(programId, "a_colorL");
	normAttribute = glGetAttribLocation(programId, "a_normL");
	tgtAttribute = glGetAttribLocation(programId, "a_tgtL");
	uvAttribute = glGetAttribLocation(programId, "a_uvL");

	// Uniforms
	isTextureUniform = glGetUniformLocation(programId, "u_isTexture");
	textureUniform = glGetUniformLocation(programId, "u_texture");
	unifMatrix = glGetUniformLocation(programId, "u_matrix");

	std::cout << "Program was linked successfully for id: " << sr->id << std::endl;
	holdsResources = true;
}

void Shader::freeResources()
{
	if (holdsResources) {
		// Current
		GLint maxShaders;
		glGetProgramiv(programId, GL_ATTACHED_SHADERS, &maxShaders);

		// Fetch current shaders
		GLsizei curentShaders; GLuint* shaders = new GLuint[maxShaders];
		glGetAttachedShaders(programId, maxShaders, &curentShaders, shaders);

		// Delete current shaders
		for (GLsizei i{ 0 }; i < curentShaders; ++i) {
			glDeleteShader(shaders[i]);
		}

		// Free array of indexes.
		delete[] shaders;

		// Delete current program
		glDeleteProgram(programId);

		holdsResources = false;
	}
}

GLuint Shader::getProgramId() const
{
	return programId;
}

void Shader::setProgramId(GLuint programId)
{
	this->programId = programId;
}

std::shared_ptr<ShaderResource> Shader::getShaderResource()
{
	return sr;
}

void Shader::setShaderResource(std::shared_ptr<ShaderResource> shaderResource)
{
	sr = shaderResource;
}