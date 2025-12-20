#include "Renderer.h"
#include <SDL.h>
#include "SpriteComponent.h"
#include <algorithm>
#include "Shader.h"
#include "VertexArray.h"


Renderer::Renderer(Game* game)
	:mGame(game)
{

}

Renderer::~Renderer()
{

}

bool Renderer::Initialize(float screenWidth, float screenHeight)
{
	if (!LoadShaders())
	{
		SDL_Log("Failed to load shader");
		return false;
	} 

	CreateSpriteVerts();

	return true;
}

void Renderer::Shutdown()
{
	UnloadData();
}

void Renderer::UnloadData()
{

}

void Renderer::Draw(class Shader* shader)
{
	for (auto sprite : mSprites)
	{
		sprite->Draw(shader);
	}
}

void Renderer::AddSprite(SpriteComponent* sc)
{
	int myOrder = sc->GetUpdateOrder();
	auto iter = std::ranges::lower_bound(mSprites, myOrder, {}, &SpriteComponent::GetUpdateOrder);
	mSprites.insert(iter, sc);
}

void Renderer::RemoveSprite(SpriteComponent* sc)
{
	auto iter = std::ranges::find(mSprites, sc);
	if (iter != mSprites.end())
	{
		mSprites.erase(iter);
	}
}

void Renderer::CreateSpriteVerts()
{ }

bool Renderer::LoadShaders()
{ 
	mSpriteShader = std::make_unique<Shader>();
	if (!mSpriteShader->Load("Shader/Sprite.vert", "Shader/Sprite.frag"))
	{
		return false;
	}
	mSpriteShader->SetActive();
	Matrix4 viewProj = Matrix4::CreateSimpleViewProj(1024.f, 768.f);
	mSpriteShader->SetMatrixUniform("uViewProj", viewProj);
	return true;
}