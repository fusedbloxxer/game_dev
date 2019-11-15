#include "ResourceManager.h"

ResourceManager* ResourceManager::resManInstance = nullptr;

ResourceManager::ResourceManager()
{
}

ResourceManager* ResourceManager::getInstance()
{
	if (!resManInstance) {
		resManInstance = new ResourceManager();
	}
	return resManInstance;
}

ResourceManager::~ResourceManager()
{

}
