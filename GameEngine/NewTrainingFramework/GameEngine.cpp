// GameEngine.cpp : Defines the entry point for the console application.
#include "stdafx.h"
#include "ResourceManager.h"
#include "SceneManager.h"

int init(ESContext* esContext)
{
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	ResourceManager::getInstance()->init();
	SceneManager::getInstance()->init(esContext);
	return 0;
}

void Draw(ESContext* esContext)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	SceneManager::getInstance()->draw();
	eglSwapBuffers(esContext->eglDisplay, esContext->eglSurface);
}

void Update(ESContext* esContext, float deltaTime)
{
	SceneManager::getInstance()->getActiveCamera()->setDeltaTime(deltaTime);
	Globals::TIME += (Globals::TIME > Globals::FRAME_LIMIT) ? -Globals::FRAME_LIMIT : deltaTime;

	if (Globals::TIME <= Globals::FRAME_LIMIT)
	{
		SceneManager::getInstance()->update();
	}
}

void Key(ESContext* esContext, unsigned char key, bool bIsPressed)
{
	SceneManager::getInstance()->pressKey(key, bIsPressed);
}

int _tmain(int argc, _TCHAR* argv[])
{
	// Identifying memory leaks
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	
	ESContext esContext;
	esInitContext(&esContext);
	esCreateWindow(&esContext, NULL, NULL, NULL, ES_WINDOW_RGB | ES_WINDOW_DEPTH);
	
	if (init(&esContext) != 0)
		return 0;

	esRegisterDrawFunc(&esContext, Draw);
	esRegisterUpdateFunc(&esContext, Update);
	esRegisterKeyFunc(&esContext, Key);

	esMainLoop(&esContext);
	
	delete SceneManager::getInstance();
	delete ResourceManager::getInstance();
	printf("Press any key...\n");
	return 0;
}