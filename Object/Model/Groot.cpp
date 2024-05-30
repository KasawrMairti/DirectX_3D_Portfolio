#include "Framework.h"
#include "Groot.h"


Groot::Groot()
	:ModelAnimator("Groot")
{
	scale *= 0.04f;
	ReadClip("groot_idle");
	ReadClip("groot_walk");
	ReadClip("groot_attack");
	CreateTexture();
	reader->GetMaterial()[0]->Load(L"Groot.mat");

	weapon = new Model("Dwarven_Axe");
	weapon->GetReader()->GetMaterial()[0]->Load(L"Dwarven_Axe.mat");
	weapon->rotation.y = XMConvertToRadians(274);
	weapon->rotation.z = XMConvertToRadians(-79);
	weapon->translation.x = -50.63f;
	weapon->translation.y = 68.64f;
	weapon->translation.z = -20.58f;

	rightHand = new Transform();
	weapon->SetParent(rightHand);

	////////////

	clips[ATTACK]->SetEndEvent(0.7f, bind(&Groot::SetClip, this, IDLE));

	hpBar = new ProgressBar(L"UI/hp_bar.png", L"UI/hp_bar_BG.png");
	hpBar->SetLabel("HP Bar");
	hpBar->scale.x *= 0.5f;
}

Groot::~Groot()
{
	delete weapon;
	delete rightHand;

	delete hpBar;
}

void Groot::Update()
{
	ModelAnimator::Update();
	weapon->Update();
	hpBar->Update();
	hpBar->translation = CAMERA->worldToSreenPoint(this->globalPosition + V_UP * 10.0f);

	Move();

	UpdateRightHand();
}

void Groot::Render()
{
	ModelAnimator::Render();
	weapon->Render();
	
}

void Groot::Debug()
{
	ModelAnimator::Debug();

	ImGui::SliderFloat("Speed", &speed, 0.0f, 10.0f);
	ImGui::SliderFloat("TakeTime", &takeTime, 0.0f, 1.0f);

	weapon->Debug();
	
	Transform::Debug();
	label = "Groot";

	static float value = 1.0f;
	hpBar->SetValue(value);
	ImGui::SliderFloat("HP", &value, 0.0f, 1.0f);
}

void Groot::PostRender()
{
	Debug();

	hpBar->Render();
}

void Groot::UpdateRightHand()
{
	UINT nodeIndex = reader->GetNodeIndex("mixamorig:RightHand");
	Matrix nodeTransform = GetTransformByNode(nodeIndex);
	rightHand->GetWorld() = nodeTransform * world;
}

void Groot::SetClip(AnimState state)
{
	if (curState != state)
	{
		PlayClip(state, speed, takeTime);
		curState = state;
	}
}

void Groot::Move()
{
	if (KEY_PRESS('W'))
	{
		translation -= Forward() * moveSpeed * Time::Delta();
		SetClip(RUN);
	}

	if (KEY_PRESS('S'))
	{
		translation -= Backward() * moveSpeed * Time::Delta();
		SetClip(RUN);
	}

	if (KEY_UP('W') || KEY_UP('S')) SetClip(IDLE);

	if (KEY_PRESS('A'))
	{
		rotation.y -= rotSpeed * Time::Delta();
	}

	if (KEY_PRESS('D'))
	{
		rotation.y += rotSpeed * Time::Delta();
	}

	if (KEY_DOWN(VK_LBUTTON))
	{
		SetClip(ATTACK);
	}
}
