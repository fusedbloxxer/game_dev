#include "stdafx.h"
#include "VertexAxis.h"
#include <algorithm>
#include "Model.h"
#include <fstream>
#include <string>

Model::Model(std::shared_ptr<ModelResource> mr)
	:mr{ mr }, iboId{}, noInd{}, noIndWired{}, vboId{}, wiredboId{}
{
	// Create buffer
	glGenBuffers(6, &iboId);
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
void Model::loadNormals(const std::vector<VertexType>& vertices)
{
	// Red color
	const Vector3 normalColor = { 1.0f, 0.0f, 0.0f };

	// Container
	std::vector<VertexAxis> normals;

	// Populate vector
	std::for_each(vertices.cbegin(), vertices.cend(),
		[&normals, &normalColor](const auto& vertex)
		{
			// From the vertex
			normals.push_back({ vertex.pos, normalColor });

			// To the normal
			normals.push_back({ const_cast<Vector3&>(vertex.pos) + const_cast<Vector3&>(vertex.norm), normalColor });
		});

	// Send the data to the buffer
	glBindBuffer(GL_ARRAY_BUFFER, normalVboId);
	glBufferData(GL_ARRAY_BUFFER, (noNormalInd = normals.size()) * sizeof(VertexAxis), normals.data(), GL_STATIC_DRAW);

	// Close the buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);
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

template<typename VertexType>
void Model::loadCollisionBox(const std::vector<VertexType>& vertices)
{
	if (vertices.size() > 0)
	{
		GLfloat m[2][3] = {
			vertices[0].pos.x, vertices[0].pos.y, vertices[0].pos.z,
			vertices[0].pos.x, vertices[0].pos.y, vertices[0].pos.z
		};

		for (decltype(vertices.size()) i = 1; i < vertices.size(); ++i)
		{
			// Maximum values
			if (m[0][0] < vertices[i].pos.x)
			{
				m[0][0] = vertices[i].pos.x;
			}

			if (m[0][1] < vertices[i].pos.y)
			{
				m[0][1] = vertices[i].pos.y;
			}

			if (m[0][2] < vertices[i].pos.z)
			{
				m[0][2] = vertices[i].pos.z;
			}

			// Minimum values
			if (m[1][0] > vertices[i].pos.x)
			{
				m[1][0] = vertices[i].pos.x;
			}

			if (m[1][1] > vertices[i].pos.y)
			{
				m[1][1] = vertices[i].pos.y;
			}

			if (m[1][2] > vertices[i].pos.z)
			{
				m[1][2] = vertices[i].pos.z;
			}
		}

		const Vector3& boxColor = { 1.0f, 1.0f, 0.0f };

		VertexAxis aabb[8] = {
			{ { m[0][0], m[0][1], m[0][2] }, boxColor },
			{ { m[0][0], m[0][1], m[1][2] }, boxColor },
			{ { m[0][0], m[1][1], m[1][2] }, boxColor },
			{ { m[0][0], m[1][1], m[0][2] }, boxColor },
			{ { m[1][0], m[0][1], m[0][2] }, boxColor },
			{ { m[1][0], m[0][1], m[1][2] }, boxColor },
			{ { m[1][0], m[1][1], m[1][2] }, boxColor },
			{ { m[1][0], m[1][1], m[0][2] }, boxColor }
		};

		GLushort order[24] = {
			0, 1,
			0, 3,
			0, 4,
			2, 1,
			2, 3,
			2, 6,
			5, 1,
			5, 4,
			5, 6,
			7, 3,
			7, 4,
			7, 6
		};

		noCollisionInd = 8;
		noCollisionIndices = 24;

		if (holdsResources)
		{
			// Send box data
			glBindBuffer(GL_ARRAY_BUFFER, collisionVboId);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(aabb), aabb);

			// Send indices
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, collisionIboId);
			glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(order), order);

			// Close buffers
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}
		else
		{
			// Store the vertices
			for (const auto& v : aabb)
			{
				this->AABBs.push_back({ v.pos, v.color });
			}

			// Send box data
			glBindBuffer(GL_ARRAY_BUFFER, collisionVboId);
			glBufferData(GL_ARRAY_BUFFER, sizeof(aabb), aabb, GL_STATIC_DRAW);

			// Send indices
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, collisionIboId);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(order), order, GL_STATIC_DRAW);

			// Close buffers
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}
	}
}

void Model::updateCollisionBox(const Matrix& worldMatrix)
{
	Vector4 auxiliary;
	auto modifiedAABBs = AABBs;

	for (auto& v : modifiedAABBs)
	{
		auxiliary.x = v.pos.x;
		auxiliary.y = v.pos.y;
		auxiliary.z = v.pos.z;
		auxiliary.w = 1.0f;

		auxiliary = auxiliary * worldMatrix;
		
		v.pos.x = auxiliary.x;
		v.pos.y = auxiliary.y;
		v.pos.z = auxiliary.z;
	}

	loadCollisionBox<VertexAxis>(modifiedAABBs);
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

GLuint Model::getNoNormalInd() const
{
	return noNormalInd;
}

void Model::setNoNormalInd(GLuint count)
{
	this->noNormalInd = count;
}

GLuint Model::getNormalVboId() const
{
	return normalVboId;
}

void Model::setNormalVboId(GLuint id)
{
	this->normalVboId = id;
}

GLuint Model::getCollisionVboId() const
{
	return collisionVboId;
}

void Model::setCollisionVboId(GLuint id)
{
	this->collisionVboId = id;
}

GLuint Model::getNoCollisionInd() const
{
	return this->noCollisionInd;
}

void Model::setNoCollisionInd(GLuint count)
{
	this->noCollisionInd = count;
}

GLuint Model::getCollisionIboId() const
{
	return this->collisionIboId;
}

void Model::setCollisionIboId(GLuint id)
{
	this->collisionIboId = id;
}

GLuint Model::getNoCollisionIndices() const
{
	return noCollisionIndices;
}

void Model::setNoCollisionIndices(GLuint count)
{
	this->noCollisionIndices = count;
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
