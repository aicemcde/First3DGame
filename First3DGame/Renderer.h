#pragma once
#include <string>
#include <vector>
#include <memory>

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
private:
	bool LoadShaders();
	void CreateSpriteVerts();

	std::vector<class SpriteComponent*> mSprites;

	std::unique_ptr<class VertexArray> mSpriteVerts;
	std::unique_ptr<class Shader> mSpriteShader;
	std::unique_ptr<class Shader> mMeshShader;
	
	Matrix4 mView;
	Matrix4 mProjection;

	class Game* mGame;
};