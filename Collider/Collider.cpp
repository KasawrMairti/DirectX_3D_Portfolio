#include "Framework.h"
#include "Collider.h"

bool Collider::hiddenInGame = false;

Collider::Collider()
{
	material = new Material(L"03Collider");

	SetColor(0.0f, 1.0f, 0.0f);
}

Collider::~Collider()
{
	delete material;
	delete mesh;
}

bool Collider::Collision(Collider* other)
{
	Transform::UpdateWorld();
		other->UpdateWorld();

	switch (other->type)
	{
		case Collider::BOX:
			return Collision((ColliderBox*)other);
			break;
		case Collider::SPHERE:
			return Collision((ColliderSphere*)other);
			break;
		case Collider::CAPSHULE:
			return Collision((ColliderCapsule*)other);
			break;
		default:
			break;
	}	

	return false;
}

void Collider::Render()
{
	if (hiddenInGame)
		return;

	Transform::SetWorld();

	mesh->SetMesh(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	material->SetMaterial();

	DC->DrawIndexed(indices.size(), 0, 0);
}
