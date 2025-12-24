#pragma once
#include "Component.h"

class MeshComponent : public Component
{
public:
	MeshComponent(class Actor* owner);
	~MeshComponent();

	virtual void Draw(class Shader* shader);
	virtual void SetMesh(class Mesh* mesh) { mMesh = mesh; };
	void SetTextureIndex(size_t index) { mTextureIndex = index; };

	virtual void SetShader(class Shader* shader) { mShader = shader; }
	class Shader* GetShader() const noexcept { return mShader; }
protected:
	class Mesh* mMesh;
	size_t mTextureIndex;
	class Shader* mShader;
};