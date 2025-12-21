#include "Renderer.h"
#include <SDL.h>
#include "SpriteComponent.h"
#include <algorithm>
#include "Shader.h"
#include "VertexArray.h"
#include "Game.h"
#include "MeshComponent.h"


Renderer::Renderer(Game* game)
	:mGame(game)
{

}

Renderer::~Renderer()
{

}

bool Renderer::Initialize()
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

void Renderer::Draw()
{
	glClearColor(0.86f, 0.86f, 0.86f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	mMeshShader->SetActive();
	mMeshShader->SetMatrixUniform("uViewProj", mView * mProjection);

	for (auto mc : mMeshComps)
	{
		mc->Draw(mMeshShader.get());
	}


	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(
		GL_SRC_ALPHA,
		GL_ONE_MINUS_SRC_ALPHA
	);

	mSpriteShader->SetActive();
	mSpriteVerts->SetActive();

	for (auto sprite : mSprites)
	{
		sprite->Draw(mSpriteShader.get());
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
{ 
	float vertexBuffer[] = {
	-0.5f, 0.5f, 0.f,0.0f, 0.f, 0.f, 0.f, 0.f,
	0.5f, 0.5f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f,
	0.5f, -0.5f, 0.f, 0.f, 0.f, 0.f, 1.f, 1.f,
	-0.5f, -0.5f, 0.f, 0.f, 0.f, 0.f, 0.f, 1.f
	};

	unsigned int indexBuffer[] = {
		0, 1, 2,
		2, 3, 0
	};

	mSpriteVerts = std::make_unique<VertexArray>(vertexBuffer, 4, indexBuffer, 6);
}

bool Renderer::LoadShaders()
{ 
	mSpriteShader = std::make_unique<Shader>();
	if (!mSpriteShader->Load("Shaders/Sprite.vert", "Shaders/Sprite.frag"))
	{
		return false;
	}
	mSpriteShader->SetActive();
	Matrix4 viewProj = Matrix4::CreateSimpleViewProj(1024.f, 768.f);
	mSpriteShader->SetMatrixUniform("uViewProj", viewProj);

	mMeshShader = std::make_unique<Shader>();
	if (!mMeshShader->Load("Shaders/Phong.vert", "Shaders/Phong.frag"))
	{
		return false;
	}
	mMeshShader->SetActive();
	mView = Matrix4::CreateLookAt(Vector3::Zero, Vector3::UnitX, Vector3::UnitZ);
	mProjection = Matrix4::CreatePerspectiveFOV(Math::ToRadians(70.0f),
		mGame->GetScreenSize().x, mGame->GetScreenSize().y, 25.0f, 10000.0f);
	mMeshShader->SetMatrixUniform("uViewProj", mView * mProjection);
	return true;
}

void Renderer::SetLightUniforms(Shader* shader)
{
	Matrix4 invView = mView;
	invView.Invert();
	shader->SetVectorUniform("uCameraPos", invView.GetTranslation());
	shader->SetVectorUniform("uAmbientLight", mAmbientLight);
	shader->SetVectorUniform("uDirLight.mDirection", mDirLight.mDirection);
	shader->SetVectorUniform("uDirLight.mDiffuseColor", mDirLight.mDiffuseColor);
	shader->SetVectorUniform("uDirLight.mSpecColor", mDirLight.mSpecColor);
}