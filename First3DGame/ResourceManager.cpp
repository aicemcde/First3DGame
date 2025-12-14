#include "ResourceManager.h"
#include "Texture.h"

ResourceManager::ResourceManager()
{

}

ResourceManager::~ResourceManager()
{

}

Texture* ResourceManager::GetTexture(const std::string& fileName)
{
	Texture* tex = nullptr;
	auto iter = mTextures.find(fileName);
	if (iter != mTextures.end())
	{
		tex = iter->second.get();
	}
	else
	{
		std::unique_ptr<Texture> uniTex = std::make_unique<Texture>();
		if (uniTex->Load(fileName))
		{
			tex = uniTex.get();
			mTextures.emplace(fileName, std::move(uniTex));
		}
	}
	return tex;
}

void ResourceManager::Unload()
{
	mTextures.clear();
}