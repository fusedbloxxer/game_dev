#include "stdafx.h"
#include "Parser.h"
#include "Model.h"

void Model::load()
{
	auto pair = Parser::parseFile(mr->file.c_str());
	load<Vertex_NFG>(pair.first, pair.second);
}

Model::Model(std::shared_ptr<ModelResource> mr)
	:mr{ mr }, iboId{}, noInd{}, noIndWired{}, vboId{}, wiredboId{} 
{
	glGenBuffers(3, &iboId);
}

Model& Model::init(std::shared_ptr<ModelResource> mr)
{
	freeResources();
	this->mr = mr;
	return *this;
}

Model::~Model()
{
	freeResources();
	glDeleteBuffers(3, &iboId);
}

std::vector<GLushort> Model::getWired(const std::vector<GLushort>& indexes)
{
	std::vector<GLushort> wired;

	for (size_t i = 0; i < indexes.size(); i += 3)
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
		// TODO; glDeleteBuffers(3, &iboId);
		holdsResources = false;
	}
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