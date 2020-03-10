// GameEngine.cpp : Defines the entry point for the console application.
#include "stdafx.h"
#include "RapidResourceAdapter.h"
#include "RapidSceneAdapter.h"
#include "RapidEventAdapter.h"
#include "ResourceManager.h"
#include "EventManager.h"
#include "SceneManager.h"
#include "Logger.h"
#include <future>
#include "GameEngine.h"
#include <array>

GLuint texture, frameBuffer, depthRenderBuffer, postProcessingBufferId;

void setUpPostProcessing(GLuint& texture, GLuint& frameBuffer, GLuint& depthRenderBuffer)
{
	GLint texWidth = SceneManager::getInstance()->getWidth();
	GLint texHeight = SceneManager::getInstance()->getHeight();

	checkTextureSize(texWidth, texHeight);

	// generate the framebuffer, renderbuffer, and texture object names
	glGenFramebuffers(1, &frameBuffer);
	glGenRenderbuffers(1, &depthRenderBuffer);
	glGenTextures(1, &texture);

	// 1. Model data
	std::array quad = {
		Vector2{  1.0f,  1.0f },
		Vector2{ -1.0f,  1.0f },
		Vector2{  1.0f, -1.0f },
		Vector2{ -1.0f,  1.0f },
		Vector2{  1.0f, -1.0f },
		Vector2{ -1.0f, -1.0f }
	};

	// 2. Generate databuffer to hold the vertices for the quad
	glGenBuffers(1, &postProcessingBufferId);

	// 3. Send data to buffer
	glBindBuffer(GL_ARRAY_BUFFER, postProcessingBufferId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// bind texture and load the texture mip-level 0
	// texels are RGB565
	// no texels need to be specified as we are going to draw into
	// the texture

	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight,
		0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	// bind renderbuffer and create a 16-bit depth buffer
	// width and height of renderbuffer = width and height of
	// the texture

	glBindRenderbuffer(GL_RENDERBUFFER, depthRenderBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16,
		texWidth, texHeight);
	// bind the framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	// specify texture as color attachment
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
		GL_TEXTURE_2D, texture, 0);
	// specify depth_renderbufer as depth attachment
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
		GL_RENDERBUFFER, depthRenderBuffer);

	// check for framebuffer complete
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		throw std::runtime_error{ "GL_FRAMEBUFFER IS NOT COMPLETE!" };
	}
}

int init(ESContext* esContext, GLuint& texture, GLuint& frameBuffer, GLuint& depthRenderBuffer)
{
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	ResourceManager::getInstance()->init(new RapidResourceAdapter("..\\Resources\\XMLFiles\\resourceManager.xml"));
	SceneManager::getInstance()->init(esContext, new RapidSceneAdapter("..\\Resources\\XMLFiles\\sceneManager.xml"));
	EventManager::getInstance()->init(new RapidEventAdapter("..\\Resources\\XMLFiles\\eventManager.xml"));
	EventManager::getInstance()->event(Event::Type::EVENT_PASSIVE)->trigger(Trigger::Type::MAIN_AREA_BACKGROUND);

	setUpPostProcessing(texture, frameBuffer, depthRenderBuffer);

	return 0;
}

void Draw(ESContext* esContext)
{
	// render to texture using FBO
	// clear color and depth buffer
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

	// render to texture using FBO
	// clear color and depth buffer
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// drawing commands to the framebuffer object
	SceneManager::getInstance()->draw();

	// render to window system provided framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Use texture to draw to window system provided framebuffer
	// We draw a quad that is the size of the viewport.
	//
	// The vertex shader outputs the vertex position and texture
	// coordinates passed as inputs.
	//
	// The fragment shader uses the texture coordinate to sample
	// the texture and uses this as the per-fragment color value.

	// load uniforms for vertex and fragment shade used to render to FBO.
	// drawing commands to the framebuffer object
	const auto& shader = ResourceManager::getInstance()->load<Shader>(9);

	// 4. Bind program
	glUseProgram(shader->getProgramId());

	// 5. Select fields to send data to
	if (shader->getFields().textureUniform[0] != -1)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glUniform1i(shader->getFields().textureUniform[0], 0);
	}

	// 6. Bind buffers and select data from buffers
	glBindBuffer(GL_ARRAY_BUFFER, postProcessingBufferId);

	if (const auto posAttrib = shader->getFields().positionAttribute; posAttrib != -1)
	{
		glEnableVertexAttribArray(posAttrib);
		glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
	}

	// Draw on screen
	glDrawArrays(GL_TRIANGLES, 0, 6);

	// Close buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Swap buffers and repeat operation
	eglSwapBuffers(esContext->eglDisplay, esContext->eglSurface);
}

void checkTextureSize(const GLint& texWidth, const GLint& texHeight)
{
	GLint maxRenderbufferSize;
	glGetIntegerv(GL_MAX_RENDERBUFFER_SIZE, &maxRenderbufferSize);

	// check if GL_MAX_RENDERBUFFER_SIZE is >= texWidth and texHeight
	if ((maxRenderbufferSize <= texWidth) ||
		(maxRenderbufferSize <= texHeight))
	{
		throw std::runtime_error{ "Invalid size." };
	}
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

void cleanUp(GLuint& texture, GLuint& frameBuffer, GLuint& depthRenderBuffer);

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

		if (init(&esContext, texture, frameBuffer, depthRenderBuffer) != 0)
			return 0;

		esRegisterDrawFunc(&esContext, Draw);
		esRegisterUpdateFunc(&esContext, Update);
		esRegisterKeyFunc(&esContext, Key);

		esMainLoop(&esContext);
	}
	catch (std::runtime_error & ex)
	{
		Logger::e("An error took place:");
		Logger::e(ex.what());
	}
	catch (std::exception & ex)
	{
		Logger::e("An exception took place:");
		Logger::e(ex.what());
	}
	catch (...)
	{
		Logger::wtf("An exception took place:");
	}

	cleanUp(texture, frameBuffer, depthRenderBuffer);
	printf("Press any key...\n");

	return 0;
}

void cleanUp(GLuint& texture, GLuint& frameBuffer, GLuint& depthRenderBuffer)
{
	delete EventManager::getInstance();
	delete SceneManager::getInstance();
	delete ResourceManager::getInstance();

	// cleanup
	glDeleteRenderbuffers(1, &depthRenderBuffer);
	glDeleteBuffers(1, &postProcessingBufferId);
	glDeleteFramebuffers(1, &frameBuffer);
	glDeleteTextures(1, &texture);
}
