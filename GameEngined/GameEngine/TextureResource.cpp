#include "TextureResource.h"

TextureResource::TextureResource(int id, std::string type, std::string file, std::string min, std::string mag, std::string ws, std::string wt)
	: id{ id }, type{ type }, file{ file }, minFilter{ min }, magFilter{ mag }, wrapS{ ws }, wrapT{ wt }
{
}