#include "stdafx.h"
#include "SceneManager.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

SceneManager* SceneManager::scManInstance = nullptr;

SceneManager::SceneManager()
{
	// TODO;
}

SceneManager* SceneManager::getInstance()
{
	if (scManInstance == nullptr) {
		scManInstance = new SceneManager();
	}
	return scManInstance;
}

void SceneManager::init(const char* sceneManagerPath)
{
	std::ifstream input{ sceneManagerPath };
	if (!input.is_open()) {
		std::cout << "Could't open file: " << sceneManagerPath << std::endl;
		abort();
	}
	
	std::stringstream ss; ss << input.rdbuf();
	input.close(); std::string content{ ss.str() };

	auto xmlPtr = std::make_unique<rapidxml::xml_document<>>();
	xmlPtr->parse<0>(&content[0]);
	

	
	xmlPtr->clear();
}

void SceneManager::draw()
{
	// TODO
}

void SceneManager::update()
{
	// TODO;
}

void SceneManager::freeResources()
{
	// TODO;
}

SceneManager::~SceneManager()
{
	std::cout << "Destructor was called for SceneManager." << std::endl;
	freeResources();
}
