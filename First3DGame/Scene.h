#pragma once
#include <memory>
#include <vector>
#include <SDL.h>

class Asteroid;

class Scene
{
public:
	Scene();

	void Update(float deltaTime);
	void Draw(class Shader* shader);
	void Unload();
	void ProcessInput(const uint8_t* keyState);

	void AddActor(std::unique_ptr<class Actor> actor);
	void RemoveActor(class Actor* actor);

	void AddSpriteComponent(class SpriteComponent* sc);
	void RemoveSpriteComponent(class SpriteComponent* sc);

	void InputActor(const uint8_t* keyState);

private:
	std::vector<std::unique_ptr<class Actor>> mActors;
	std::vector < std::unique_ptr<class Actor>> mPendingActors;

	bool mUpdatingActors;
	std::vector<class SpriteComponent*> mSpriteComps;
};