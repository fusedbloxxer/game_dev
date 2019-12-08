#include "stdafx.h"
#include "Model.h"
#include <fstream>
#include <string>

Model::Model(std::shared_ptr<ModelResource> mr)
	:mr{ mr }, iboId{}, noInd{}, noIndWired{}, vboId{}, wiredboId{}
{
	// Create buffer
	glGenBuffers(3, &iboId); 
}

void Model::load()
{
	auto pair = Model::parseFile(mr->file.c_str());
	load<VertexNfg>(pair.first, pair.second);
}

Model& Model::init(std::shared_ptr<ModelResource> mr)
{
	freeResources();
	this->mr = mr;
	return *this;
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
		axisModel.freeResources();
		holdsResources = false;
	}
}

template<typename VertexType>
inline void Model::loadAxisModel(const std::vector<VertexType>& vertices)
{
	if (vertices.size() > 0)
	{
		GLfloat x = vertices[0].pos.x, y = vertices[0].pos.y, z = vertices[0].pos.z;
		for (decltype(vertices.size()) i = 1; i < vertices.size(); ++i)
		{
			if (x < vertices[i].pos.x)
			{
				x = vertices[i].pos.x;
			}

			if (y < vertices[i].pos.y)
			{
				y = vertices[i].pos.y;
			}

			if (z < vertices[i].pos.z)
			{
				z = vertices[i].pos.z;
			}
		}

		axisModel.init({ x, y, z });
		axisModel.load();
	}
}

std::pair<std::vector<VertexNfg>, std::vector<GLushort>> Model::parseFile(const char* const filePath)
{
	std::ifstream input{ filePath };

	if (!input.is_open())
	{
		throw std::exception{ "Could not open file." };
	}

	auto pair = std::make_pair<std::vector<VertexNfg>, std::vector<GLushort>>(std::vector<VertexNfg>(), std::vector<GLushort>());

	std::string str; int lines; input >> str >> lines;
	GLchar ch; input.get(ch); // Read new line

	VertexNfg v;
	for (GLint i = 0; i < lines; ++i)
	{
		input >> v;
		pair.first.push_back(v);
	}

	input >> str >> lines; input.get(ch); // Read new line
	for (GLint i = 0, j = 0; i < lines / 3; ++i, j = 0)
	{
		std::getline(input, str);
		str = str.substr(str.find('.') + 1);
		GLchar* buff = new GLchar[str.size() + 1];
		strcpy_s(buff, str.size() + 1, str.c_str());
		str.clear();

		VertexNfg::find_num(buff, j);
		pair.second.push_back(static_cast<GLushort>(VertexNfg::extr_num(buff, j)));
		VertexNfg::find_num(buff, j);
		pair.second.push_back(static_cast<GLushort>(VertexNfg::extr_num(buff, j)));
		VertexNfg::find_num(buff, j);
		pair.second.push_back(static_cast<GLushort>(VertexNfg::extr_num(buff, j)));

		delete[] buff;
	}

	input.close();
	return pair;
}

Model::~Model()
{
	freeResources();
	glDeleteBuffers(3, &iboId);
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

AxisModel& Model::getAxisModel()
{
	return axisModel;
}

std::shared_ptr<ModelResource> Model::getModelResource()
{
	return mr;
}

void Model::setModelResource(std::shared_ptr<ModelResource> mr)
{
	this->mr = mr;
}