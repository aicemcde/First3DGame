#pragma once
#include <string>
#include <vector>
#include <memory>
#include "Math.h"

struct DirectionalLight
{
	Vector3 mDirection;
	Vector3 mDiffuseColor;
	Vector3 mSpecColor;
};

class Renderer
{
public:
	Renderer(class Game* game);
	~Renderer();

	bool Initialize();
	void Shutdown();
	void UnloadData();
	void Draw();

	void AddSprite(class SpriteComponent* sprite);
	void RemoveSprite(class SpriteComponent* sprite);
	class Mesh* GetMesh(const std::string& fileName);

	void SetLightUniforms(class Shader* shader);
private:
	bool LoadShaders();
	void CreateSpriteVerts();

	std::vector<class SpriteComponent*> mSprites;
	std::vector<class MeshComponent*> mMeshComps;

	std::unique_ptr<class VertexArray> mSpriteVerts;
	std::unique_ptr<class Shader> mSpriteShader;
	std::unique_ptr<class Shader> mMeshShader;
	
	Matrix4 mView;
	Matrix4 mProjection;
	Vector3 mAmbientLight;
	DirectionalLight mDirLight;


	class Game* mGame;
};