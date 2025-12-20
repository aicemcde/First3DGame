#include "MeshComponent.h"
#include "Shader.h"
#include "Actor.h"
#include "Texture.h"
#include "Mesh.h"
#include "VertexArray.h"
#include <glew.h>

MeshComponent::MeshComponent(Actor* owner)
	:Component(owner)
{

}

MeshComponent::~MeshComponent()
{

}

void MeshComponent::Draw(Shader* shader)
{
	if (mMesh)
	{
		shader->SetMatrixUniform("uWorldTransform",
			mOwner->GetWorldTransform());

		Texture* t = mMesh->GetTexture(mTextureIndex);
		if (t) { t->SetActive(); }
		VertexArray* va = mMesh->GetVertexArray();
		va->SetActive();
		glDrawElements(GL_TRIANGLES, va->GetNumIndices(),
			GL_UNSIGNED_INT, nullptr);
	}
}