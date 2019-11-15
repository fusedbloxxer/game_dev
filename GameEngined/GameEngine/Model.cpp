#include "Model.h"

void Model::load()
{
	// TODO;
}

Model::Model(std::shared_ptr<ModelResource> mr)
	:iboId{}, wiredboId{}, vboId{}, noInd{}, noIndWired{}, mr{ nullptr }
{
	// TODO;
}

Model::~Model()
{
	// TODO;
}

GLint Model::getIboId() const
{
	return iboId;
}

void Model::setIboId(GLint id)
{
	iboId = id;
}

GLint Model::getWiredboId() const
{
	return wiredboId;
}

void Model::setWiredboId(GLint id)
{
	wiredboId = id;
}

GLint Model::getVboId() const
{
	return vboId;
}

void Model::setVboId(GLint id)
{
	vboId = id;
}

GLint Model::getNoInd() const
{
	return noInd;
}

void Model::setNoInd(GLint id)
{
	noInd = id;
}

GLint Model::getNoIndWired() const
{
	return noIndWired;
}

void Model::setNoIndWired(GLint id)
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
