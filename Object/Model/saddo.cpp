#include "Framework.h"
#include "saddo.h"


Saddo::Saddo()
	:ModelAnimator("tiger")
{
	scale *= 5.0f;
	ReadClip("saddo_idle");
	ReadClip("saddo_run");
	CreateTexture();

	//reader->GetMaterial()[0]->Load(L"Saddo.mat");

	weapon = new Sphere();
	weapon->scale *= 0.5f;

	rightHand = new Transform();
	weapon->SetParent(rightHand);
}

Saddo::~Saddo()
{
	delete weapon;

	delete rightHand;
}

void Saddo::Update()
{
	ModelAnimator::Update();
	weapon->Update();

	if (KEY_DOWN('1'))
		PlayClip(0, speed, takeTime);
	if (KEY_DOWN('2'))
		PlayClip(1, speed, takeTime);

	UpdateRightHand();
}

void Saddo::Render()
{
	ModelAnimator::Render();
	weapon->Render();
}

void Saddo::Debug()
{
	ModelAnimator::Debug();

	ImGui::SliderFloat("Speed", &speed, 0.0f, 10.0f);
	ImGui::SliderFloat("TakeTime", &takeTime, 0.0f, 1.0f);

	weapon->Debug();
}

void Saddo::UpdateRightHand()
{
	UINT boneIndex = reader->GetBoneIndex("hand_r");
	Matrix boneTransform = GetTransformByBone(boneIndex);

	UINT nodeIndex = reader->GetNodeIndex("hand_r");
	Matrix nodeTransform = GetTransformByNode(nodeIndex);
	rightHand->GetWorld() = nodeTransform * world;
}
