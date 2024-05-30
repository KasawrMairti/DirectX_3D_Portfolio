#include "Framework.h"
#include "Warrior.h"


Warrior::Warrior()
	:ModelAnimator("Warrior")
{
	GAMEMANAGER->SetWarrior(this);
	scale.x = 0.012f;
	scale.y = 0.012f;
	scale.z = 0.012f;

	ReadClip("Warrior_Idle");
	ReadClip("Warrior_Walk");
	ReadClip("Warrior_norAttack");
	ReadClip("Warrior_powAttack");
	ReadClip("Warrior_Damaged");
	ReadClip("Warrior_Die");
	ReadClip("Warrior_Dying");
	CreateTexture();
	reader->GetMaterial()[0]->Load(L"Warrior.mat");

	clips[NORATTACK]->SetEndEvent(0.9f, bind(&Warrior::AttackNorEndEvent, this));
	clips[POWATTACK]->SetEndEvent(0.3f, bind(&Warrior::AttackPowDamageEvent, this));
	clips[POWATTACK]->SetEndEvent(0.35f, bind(&Warrior::AttackPowEndEvent, this));
	clips[POWATTACK]->SetEndEvent(0.35f, bind(&Warrior::AttackPowEndEvent, this));
	clips[DIE]->SetEndEvent(0.5f, bind(&Warrior::SetClip, this, DYING, 0.0f));

	colliderBody = new ColliderBox();
	colliderBody->SetParent(this);
	colliderBody->scale.x = 120.0f;
	colliderBody->scale.y = 160.0f;
	colliderBody->scale.z = 120.0f;
	colliderBody->translation += Vector3(0.0f, 120.0f, 0.0f);

	collidersensornear = new ColliderBox();
	collidersensornear->SetParent(this);
	collidersensornear->scale.x = 250.0f;
	collidersensornear->scale.y = 160.0f;
	collidersensornear->scale.z = 250.0f;
	collidersensornear->translation += Vector3(0.0f, 80.0f, 0.0f);

	collidersensorfar = new ColliderBox();
	collidersensorfar->SetParent(this);
	collidersensorfar->scale.x = 400.0f;
	collidersensorfar->scale.y = 160.0f;
	collidersensorfar->scale.z = 400.0f;
	collidersensorfar->translation += Vector3(0.0f, 80.0f, 0.0f);

	colliderFoot = new ColliderSphere();
	colliderFoot->SetParent(this);
	colliderFoot->scale.x = 30.0f;
	colliderFoot->scale.y = 30.0f;
	colliderFoot->scale.z = 30.0f;
	colliderFoot->translation += Vector3(0.0f, 30.0f, 0.0f);

	LHand = new Transform();
	colliderLHand = new ColliderCapsule();
	colliderLHand->SetParent(LHand);
	colliderLHand->scale *= 15.0f;

	RHand = new Transform();
	colliderRHand = new ColliderCapsule();
	colliderRHand->SetParent(RHand);
	colliderRHand->scale *= 15.0f;

	SetClip(IDLE);

	hpBar = new HPBar_boss(L"UI/hp_bar_boss.png");
	hpBar->SetLabel("HP Bar Boss");
	hpBar->scale *= 1.5f;
	hpBar->translation.x = WIN_WIDTH * 0.5f;
	hpBar->translation.y = WIN_HEIGHT - 100.0f;
}

Warrior::~Warrior()
{
	delete colliderBody;
	delete collidersensornear;
	delete collidersensorfar;
	delete colliderFoot;
	delete colliderLHand;
	delete colliderRHand;
	delete LHand;
	delete RHand;
	delete hpBar;
}

void Warrior::Update()
{
	if (!isActive) return;

	ModelAnimator::Update();
	colliderBody->Update();
	collidersensornear->Update();
	collidersensorfar->Update();
	colliderFoot->Update();
	colliderLHand->Update();
	colliderRHand->Update();
	LHand->Update();
	RHand->Update();
	hpBar->Update();
	UpdateHand();
	damagedCoolTime -= Time::Delta();

	gravity();

	if (Hp_cur <= 0) return;

	if (!isAttack) move();

	Attack();
}

void Warrior::Render()
{
	if (!isActive) return;

	ModelAnimator::Render();
	colliderBody->Render();
	collidersensornear->Render();
	collidersensorfar->Render();
	colliderFoot->Render();
	colliderLHand->Render();
	colliderRHand->Render();
}

void Warrior::PostRender()
{
	hpBar->Render();
}

void Warrior::Debug()
{
}

void Warrior::SetClip(AnimState state, float speed)
{
	if (curState != state)
	{
		PlayClip(state, speed);
		curState = state;
	}
}

void Warrior::IsDamaged(float damage, float damageCooltime, bool isdown)
{
	if (damagedCoolTime > 0.0f || Hp_cur <= 0.0f) return;

	SoundManager::GetInstance()->Play("Damaged", 0.5f);
	ParticleManager::GetInstance()->Add(L"_TextData/Hit.fx", 0.15f, GetCollider()->GetGlobalPosition());
	Hp_cur -= damage;
	if (Hp_cur < 0.0f) Hp_cur = 0.0f;

	float value = 0.0f;
	value = Hp_cur / Hp_max;

	damagedCoolTime = damageCooltime;

	SetValue(value);
	if (isdown || Hp_cur <= 0.0f)
	{
		SetClip(DIE);
	}
	else
	{
		if (!isAttack) SetClip(DAMAGED);
	}
}

void Warrior::move()
{
	float distance = Distance(GAMEMANAGER->GetPlayer()->translation, translation);

	Vector3 dir = GAMEMANAGER->GetPlayer()->translation - translation;
	dir.y = 0.0f;
	dir = dir.GetNormalized();

	if (dir.x < -0.75f) dir.x = -1.0f;
	else if (dir.x >= -0.75f && dir.x < -0.3f) dir.x = -0.75f;
	else if (dir.x <= 0.3f && dir.x > -0.3f) dir.x = 0.0f;
	else if (dir.x <= 0.75f && dir.x > 0.3f) dir.x = 0.75f;
	else if (dir.x > 0.75f) dir.x = 1.0f;

	if (dir.z < -0.75f) dir.z = -1.0f;
	else if (dir.z >= -0.75f && dir.z < -0.3f) dir.z = -0.75f;
	else if (dir.z <= 0.3f && dir.z > -0.3f) dir.z = 0.0f;
	else if (dir.z <= 0.75f && dir.z > 0.3f) dir.z = 0.75f;
	else if (dir.z > 0.75f) dir.z = 1.0f;

	lastmove = dir * speed * Time::Delta();
	translation += lastmove;
	lastrotate = dir * Time::Delta();

	Vector3 rot = translation - GAMEMANAGER->GetPlayer()->translation;
	rot.y = 0.0f;
	rot = rot.GetNormalized();

	if (rot.x < -0.75f) rot.x = -1.0f;
	else if (rot.x >= -0.75f && rot.x < -0.3f) rot.x = -0.75f;
	else if (rot.x <= 0.3f && rot.x > -0.3f) rot.x = 0.0f;
	else if (rot.x <= 0.75f && rot.x > 0.3f) rot.x = 0.75f;
	else if (rot.x > 0.75f) rot.x = 1.0f;

	if (rot.z < -0.75f) rot.z = -1.0f;
	else if (rot.z >= -0.75f && rot.z < -0.3f) rot.z = -0.75f;
	else if (rot.z <= 0.3f && rot.z > -0.3f) rot.z = 0.0f;
	else if (rot.z <= 0.75f && rot.z > 0.3f) rot.z = 0.75f;
	else if (rot.z > 0.75f) rot.z = 1.0f;
	rotation.y = atan2(rot.x, rot.z);

	SetClip(WALK);

	if (GAMEMANAGER->GetPlayer()->GetCollider()->Collision(this->colliderBody))
	{
		translation -= lastmove;
	}
}

void Warrior::Attack()
{
	if (hand_leftEnable > 0) hand_leftEnable -= Time::Delta();
	if (hand_rightEnable > 0) hand_rightEnable -= Time::Delta();

	if (colliderLHand->Collision(GAMEMANAGER->GetPlayer()->GetCollider()) && hand_leftEnable > 0)
		GAMEMANAGER->GetPlayer()->IsDamaged(damagedPow, hand_leftEnable + 0.001f, translation, true);

	if (colliderRHand->Collision(GAMEMANAGER->GetPlayer()->GetCollider()) && hand_rightEnable > 0)
		GAMEMANAGER->GetPlayer()->IsDamaged(damagedPow, hand_rightEnable + 0.001f, translation, true);

	if (attackCoolTime > 0.0f) 
	{
		attackCoolTime -= Time::Delta();
		return;
	}

	if (collidersensornear->Collision(GAMEMANAGER->GetPlayer()->GetCollider()))
	{
		SetClip(POWATTACK, 1.75f);
		attackCoolTime = 5.0f;
		isAttack = true;
		damagedPow = 30.0f;
	}
	else if (collidersensorfar->Collision(GAMEMANAGER->GetPlayer()->GetCollider()))
	{
		SetClip(NORATTACK, 1.5f);
		attackCoolTime = 4.0f;
		hand_leftEnable = 1.25f;
		isAttack = true;
		damagedPow = 15.0f;
	}
}

void Warrior::AttackNorEndEvent()
{
	isAttack = false;
}

void Warrior::AttackPowDamageEvent()
{
	hand_leftEnable = 0.25f;
	hand_rightEnable = 0.25f;
}

void Warrior::AttackPowEndEvent()
{
	isAttack = false;

	SoundManager::GetInstance()->Play("Bomb", 0.5f);
	ParticleManager::GetInstance()->Add(L"_TextData/Boom.fx", 0.45f, colliderLHand->GetGlobalPosition(), 3.0f);
}

void Warrior::gravity()
{
	gravityspeed += 9.6f * Time::Delta();

	for (Collider* colliderBox : GAMEMANAGER->GetBackground()->GetColliders())
	{
		if (colliderBox->Collision(colliderBody) && colliderBox->WallTypes() == Collider::WallType::WALL)
		{
			Vector3 dir = colliderBox->Forward();

			float speed = 10.0f;
			translation.x += dir.x * speed * Time::Delta();
			translation.z += dir.z * speed * Time::Delta();
		}

		if (colliderFoot->Collision(colliderBox) && colliderBox->WallTypes() == Collider::WallType::FLOOR)
		{
			float isgravity = gravityPower * gravityspeed * Time::Delta();
			if (isgravity > 0)
			{
				gravityspeed = 0.0f;
			}
			translation.y += 2.0f * Time::Delta();

		}
	}

	translation.y -= gravityPower * gravityspeed * Time::Delta();
}

void Warrior::UpdateHand()
{
	UINT nodeIndexR = reader->GetNodeIndex("mixamorig:LeftHand");
	Matrix nodeTransformR = GetTransformByNode(nodeIndexR);
	LHand->GetWorld() = nodeTransformR * world;

	UINT nodeIndexL = reader->GetNodeIndex("mixamorig:RightHand");
	Matrix nodeTransformL = GetTransformByNode(nodeIndexL);
	RHand->GetWorld() = nodeTransformL * world;
}
