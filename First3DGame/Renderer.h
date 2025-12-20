#pragma once
#include <string>
#include <vector>
#include <memory>

class Renderer
{
	Renderer(class Game* game);
	~Renderer();

	bool Initialize(float screenWidth, float screenHeight);
	void Shutdown();
	void UnloadData();
	void Draw(class Shader* shader);

	void AddSprite(class SpriteComponent* sprite);
	void RemoveSprite(class SpriteComponent* sprite);
	class Mesh* GetMesh(const std::string& fileName);
private:
	bool LoadShaders();
	void CreateSpriteVerts();

	std::vector<class SpriteComponent*> mSprites;

	std::unique_ptr<class VertexArray> mSpriteVerts;
	std::unique_ptr<class Shader> mSpriteShader;

	class Game* mGame;
};