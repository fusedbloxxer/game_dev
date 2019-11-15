#include <iostream>
#include "ResourceManager.h"

int main()
{
	ResourceManager::getInstance()->Init();
	auto ptr = ResourceManager::getInstance()->load<Texture>(4);
}
