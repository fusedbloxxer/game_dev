#include "stdafx.h"
#include "VertexNfg.h"
#include <string>

VertexNfg::VertexNfg() {}

VertexNfg::VertexNfg(const VertexNfg& copy)
{
	pos = copy.pos;
	binorm = copy.binorm;
	norm = copy.norm;
	tgt = copy.tgt;
	color = copy.color;
	uv = copy.uv;
	uv2 = copy.uv2;
}

GLfloat VertexNfg::extr_num(const GLchar* const buff, GLint& i)
{
	GLint end = i;
	while (buff[end] != '\0' && ((buff[end] >= '0' && buff[end] <= '9') || buff[end] == '-' || buff[end] == '.'))
	{
		++end;
	}

	GLchar* temp = new GLchar[end - i + 1]; temp[end - i] = '\0';
	strncpy_s(temp, end - i + 1, buff + i, end - i);

	GLfloat result = GLfloat(atof(temp));
	delete[] temp;

	i = end + 1;
	return result;
}

void VertexNfg::find_num(const GLchar* const buff, GLint& i)
{
	while (buff[i] != '\0' && !(buff[i] >= '0' && buff[i] <= '9') && !(buff[i] == '-') && !(buff[i] == '.'))
		++i;
}

void VertexNfg::read(const GLchar* const buff, GLint& i, Vector3& vec3)
{
	find_num(buff, i);
	vec3.x = extr_num(buff, i);
	find_num(buff, i);
	vec3.y = extr_num(buff, i);
	find_num(buff, i);
	vec3.z = extr_num(buff, i);
}

void VertexNfg::read(const GLchar* const buff, GLint& i, Vector2& vec2)
{
	find_num(buff, i);
	vec2.x = extr_num(buff, i);
	find_num(buff, i);
	vec2.y = extr_num(buff, i);
}

std::ostream& operator<<(std::ostream& os, const VertexNfg& obj)
{
	os << "pos:" << obj.pos << "; norm:" << obj.norm << "; binorm:" << obj.binorm
		<< "; tgt:" << obj.tgt << "; uv:" << obj.uv;
	return os;
}

std::istream& operator>>(std::istream& is, VertexNfg& obj)
{
	// Read whole line into buffer
	std::string buffer; std::getline(is, buffer); 
	buffer = buffer.substr(buffer.find('[') + 1);
	char* buff;

	if (buffer.size() > 0)
	{
		buff = new char[buffer.size() + 1];
		strcpy_s(buff, buffer.size() + 1, buffer.c_str());
		buffer.clear();

		GLint i = 0;
		VertexNfg::read(buff, i, obj.pos);
		VertexNfg::read(buff, i, obj.norm);
		VertexNfg::read(buff, i, obj.binorm);
		VertexNfg::read(buff, i, obj.tgt);
		VertexNfg::read(buff, i, obj.uv);
		delete[] buff;
	}

	return is;
}