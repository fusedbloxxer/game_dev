// GameEngine.cpp : Defines the entry point for the console application.
#include "stdafx.h"
#include "RapidResourceAdapter.h"
#include "RapidSceneAdapter.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "Logger.h"

int init(ESContext* esContext)
{
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	ResourceManager::getInstance()->init(new RapidResourceAdapter("..\\Resources\\XMLFiles\\resourceManager.xml"));
	SceneManager::getInstance()->init(esContext, new RapidSceneAdapter("..\\Resources\\XMLFiles\\sceneManager.xml"));

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
	if (Globals::TIME <= Globals::FRAME_LIMIT)
	{
		SceneManager::getInstance()->getActiveCamera()->setDeltaTime(deltaTime);
		SceneManager::getInstance()->update();
		Globals::TIME += deltaTime;
	}
	else
	{
		Globals::TIME = -Globals::FRAME_LIMIT;
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

	Logger::setMode(15);

	try
	{
		ESContext esContext;
		esInitContext(&esContext);
		esCreateWindow(&esContext, NULL, NULL, NULL, ES_WINDOW_RGB | ES_WINDOW_DEPTH);

		if (init(&esContext) != 0)
			return 0;

		esRegisterDrawFunc(&esContext, Draw);
		esRegisterUpdateFunc(&esContext, Update);
		esRegisterKeyFunc(&esContext, Key);

		esMainLoop(&esContext);
	}
	catch (std::runtime_error& ex)
	{
		Logger::e("An error took place:");
		Logger::e(ex.what());
	}
	catch (std::exception& ex)
	{
		Logger::e("An exception took place:");
		Logger::e(ex.what());
	}
	catch (...)
	{
		Logger::wtf("An exception took place:");
	}

	delete SceneManager::getInstance();
	delete ResourceManager::getInstance();

	printf("Press any key...\n");
	return 0;
}