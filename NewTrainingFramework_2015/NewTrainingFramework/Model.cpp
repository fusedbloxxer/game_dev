#include "stdafx.h"
#include "Parser.h"
#include "Model.h"

void Model::load()
{
	auto pair = Parser::parseFile(mr->file.c_str());
	glGenBuffers(3, &iboId);
	noInd = pair.second.size();

	// Load vertices into buffer
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex_NFG) * pair.first.size(), pair.first.data(), GL_STATIC_DRAW);

	// Load indexes into buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * noInd, pair.second.data(), GL_STATIC_DRAW);

	// Create wireframe indexes
	auto wireframe = getWired(pair.second);
	noIndWired = wireframe.size();

	// Load wirefram indexes into buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, wiredboId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * noIndWired, wireframe.data(), GL_STATIC_DRAW);

	std::cout << "Model with id: " << mr->id << " was loaded successfully." << std::endl;
	holdsResources = true;
}

Model::Model(std::shared_ptr<ModelResource> mr)
	:mr{ mr }
{
	// TODO;

}

Model& Model::init(std::shared_ptr<ModelResource> mr)
{
	freeResources();
	this->mr = mr;
	return *this;
}

Model::~Model()
{
	std::cout << "Model destructor with id " << mr->id << " was called." << std::endl;
	freeResources();
}

GLuint Model::getIboId() const
{
	return iboId;
}

void Model::setIboId(GLuint id)
{
	iboId = id;
}

GLuint Model::getWiredboId() const
{
	return wiredboId;
}

void Model::setWiredboId(GLuint id)
{
	wiredboId = id;
}

GLuint Model::getVboId() const
{
	return vboId;
}

void Model::setVboId(GLuint id)
{
	vboId = id;
}

GLuint Model::getNoInd() const
{
	return noInd;
}

void Model::setNoInd(GLuint id)
{
	noInd = id;
}

GLuint Model::getNoIndWired() const
{
	return noIndWired;
}

void Model::setNoIndWired(GLuint id)
{
	noIndWired = id;
}

std::shared_ptr<ModelResource> Model::getModelResource()
{
	return mr;
}

void Model::setModelResource(std::shared_ptr<ModelResource> mr)
{
	this->mr = mr;
}

std::vector<GLushort> Model::getWired(const std::vector<GLushort>& indexes)
{
	std::vector<GLushort> wired;

	for (int i = 0; i < indexes.size(); i += 3)
	{
		wired.push_back(indexes[i]);
		wired.push_back(indexes[i + 1]);

		wired.push_back(indexes[i + 1]);
		wired.push_back(indexes[i + 2]);

		wired.push_back(indexes[i + 2]);
		wired.push_back(indexes[i]);
	}

	return wired;
}

void Model::freeResources()
{
	if (holdsResources) {
		glDeleteBuffers(3, &iboId);
		holdsResources = false;
	}
}
