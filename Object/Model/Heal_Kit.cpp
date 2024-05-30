#include "Framework.h"
#include "Heal_Kit.h"


Heal_Kit::Heal_Kit()
{
	SetDiffuse(L"Texture/heal.png");

	collider = new ColliderBox();
	collider->SetParent(this);

	scale *= 0.25f;
	translation.y = 0.75f;
	rotation.z = 0.705f;
}

Heal_Kit::~Heal_Kit()
{
	delete collider;
}

void Heal_Kit::Update()
{
	if (!isActive) return;

	TextureCube::Update();
	collider->Update();

	rotation.y += Time::Delta();
}

void Heal_Kit::Render()
{
	if (!isActive) return;

	TextureCube::Render();
	collider->Render();
}
