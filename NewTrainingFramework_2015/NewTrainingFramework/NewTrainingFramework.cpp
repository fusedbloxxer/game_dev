// NewTrainingFramework.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../Utilities/utilities.h" // if you use STL, please include this line AFTER all other include
#include "Vertex.h"
#include "Shaders.h"
#include <conio.h>
#include "Camera.h"
#include "Shaders1.h"

GLuint vboId, vboId1;
Camera* camera;
Shaders myShaders;
Shaders1 lineShaders;

int Init(ESContext* esContext)
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	//triangle data (heap)
	Vertex verticesData[6];
	Vertex lineVerticesData[2];

	// First triangle
	verticesData[0].pos.x = 0.5f;  verticesData[0].pos.y = 0.5f;  verticesData[0].pos.z = 0.0f;
	verticesData[0].color.x = 1.0f;  verticesData[0].color.y = 0.0f;  verticesData[0].color.z = 0.0f;

	verticesData[1].pos.x = -0.5f;  verticesData[1].pos.y = -0.5f;  verticesData[1].pos.z = 0.0f;
	verticesData[1].color.x = 0.0f;  verticesData[1].color.y = 1.0f;  verticesData[1].color.z = 0.0f;

	verticesData[2].pos.x = 0.5f;  verticesData[2].pos.y = -0.5f;  verticesData[2].pos.z = 0.0f;
	verticesData[2].color.x = 0.0f;  verticesData[2].color.y = 0.0f;  verticesData[2].color.z = 1.0f;

	// Second triangle
	verticesData[3].pos.x = 0.5f;  verticesData[3].pos.y = 0.5f;  verticesData[3].pos.z = 0.0f;
	verticesData[3].color.x = 1.0f;  verticesData[3].color.y = 0.0f;  verticesData[3].color.z = 0.0f;

	verticesData[4].pos.x = -0.5f;  verticesData[4].pos.y = -0.5f;  verticesData[4].pos.z = 0.0f;
	verticesData[4].color.x = 0.0f;  verticesData[4].color.y = 1.0f;  verticesData[4].color.z = 0.0f;

	verticesData[5].pos.x = -0.5f;  verticesData[5].pos.y = 0.5f;  verticesData[5].pos.z = 0.0f;
	verticesData[5].color.x = 1.0f;  verticesData[5].color.y = 1.0f;  verticesData[5].color.z = 0.0f;

	// Line
	lineVerticesData[0].pos.x = 0.0f; lineVerticesData[0].pos.y = -1.0f; lineVerticesData[0].pos.z = 0.0f;
	lineVerticesData[1].pos.x = 0.0f; lineVerticesData[1].pos.y = 1.0f; lineVerticesData[1].pos.z = 0.0f;

	//buffer object - for the two triangles
	glGenBuffers(1, &vboId);
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesData), verticesData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//buffer1 object - for the white line
	glGenBuffers(1, &vboId1);
	glBindBuffer(GL_ARRAY_BUFFER, vboId1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(lineVerticesData), lineVerticesData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//creation of shaders and program 
	return myShaders.Init("../Resources/Shaders/TriangleShaderVS.vs", "../Resources/Shaders/TriangleShaderFS.fs") |
		lineShaders.Init("../Resources/Shaders/LineShaderVS.vs", "../Resources/Shaders/LineShaderFS.fs");
}

void Draw(ESContext* esContext)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(myShaders.program);

	glBindBuffer(GL_ARRAY_BUFFER, vboId);

	if (myShaders.positionAttribute != -1)
	{
		glEnableVertexAttribArray(myShaders.positionAttribute);
		glVertexAttribPointer(myShaders.positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	}

	if (myShaders.colorAttribute != -1)
	{
		glEnableVertexAttribArray(myShaders.colorAttribute);
		glVertexAttribPointer(myShaders.colorAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(Vector3));
	}

	if (myShaders.uniformMatrix != -1)
	{
		glUniformMatrix4fv(myShaders.uniformMatrix, 1, GL_FALSE, (float*)(camera->getViewMatrix() * camera->getProjMatrix()).m);
	}

	glDrawArrays(GL_TRIANGLES, 0, 6);

	// For the white line.
	glUseProgram(lineShaders.program);
	
	glBindBuffer(GL_ARRAY_BUFFER, vboId1);

	if (lineShaders.positionAttribute != -1)
	{
		glVertexAttribPointer(lineShaders.positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
		glEnableVertexAttribArray(lineShaders.positionAttribute);
	}

	glDrawArrays(GL_LINES, 0, 2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	eglSwapBuffers(esContext->eglDisplay, esContext->eglSurface);
}

void Update(ESContext* esContext, float deltaTime)
{
	Globals::alpha = (Globals::alpha > 2 * PI) ? 2 * PI - Globals::alpha : Globals::alpha + Globals::pas;
	camera->setDeltaTime(deltaTime);
}

void Key(ESContext* esContext, unsigned char key, bool bIsPressed)
{
	if (bIsPressed) {

		switch (key)
		{
		case 'W':
			camera->moveOz(-1);
			break;
		case 'A':
			camera->moveOx(-1);
			break;
		case 'S':
			camera->moveOz(1);
			break;
		case 'D':
			camera->moveOx(1);
			break;
		case VK_RIGHT:
			camera->rotateOy(-1);
			break;
		case VK_LEFT:
			camera->rotateOy(1);
			break;
		case VK_UP:
			camera->rotateOx(1);
			break;
		case VK_DOWN:
			camera->rotateOx(-1);
			break;
		case 'O':
			camera->rotateOz(1);
			break;
		case 'P':
			camera->rotateOz(-1);
			break;
		case 'X':
			camera->moveOy(1);
			break;
		case 'Z':
			camera->moveOy(-1);
			break;
		default:
			break;
		}
	}
}

void CleanUp()
{
	glDeleteBuffers(1, &vboId);
}

int _tmain(int argc, _TCHAR* argv[])
{
	//identifying memory leaks
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	ESContext esContext;

	esInitContext(&esContext);

	esCreateWindow(&esContext, "Hello Triangle", Globals::screenWidth, Globals::screenHeight, ES_WINDOW_RGB | ES_WINDOW_DEPTH);

	if (Init(&esContext) != 0)
		return 0;

	camera = new Camera(Vector3{ 0.0f, 0.0f, 1.0f });
	glEnable(GL_DEPTH_TEST);

	esRegisterDrawFunc(&esContext, Draw);
	esRegisterUpdateFunc(&esContext, Update);
	esRegisterKeyFunc(&esContext, Key);

	esMainLoop(&esContext);

	//releasing OpenGL resources
	CleanUp();

	delete camera;
	printf("Press any key...\n");
	_getch();
	return 0;
}

