#pragma once
#include <memory>
#include <unordered_map>
#include <string>

class Texture;

class ResourceManager
{
public:
	ResourceManager();
	~ResourceManager();
	Texture* GetTexture(const std::string& fileName);
	void Unload();
private:
	std::unordered_map<std::string, std::unique_ptr<Texture>> mTextures;
};