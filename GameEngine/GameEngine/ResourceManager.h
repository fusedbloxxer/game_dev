#pragma once

struct ModelResource {

};

struct ShaderResource {

};

struct TextureResource {

};

class ResourceManager
{
private:
	ResourceManager();
	static ResourceManager* resManInstance;
public:
	void Init();
	void freeResources();
	static ResourceManager* getInstance();
	~ResourceManager();
};

