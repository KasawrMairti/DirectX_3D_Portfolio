#include "Framework.h"
#include "Enemy.h"


Enemy::Enemy()
	:ModelAnimator("Enemy")
{
	GAMEMANAGER->SetEnemy(this);
	scale.x = 0.008f;
	scale.y = 0.008f;
	scale.z = 0.008f;

	ReadClip("Enemy_Idle");
	ReadClip("Enemy_walk");
	ReadClip("Enemy_run");
	ReadClip("Enemy_norAttack1");
	ReadClip("Enemy_norAttack2");
	ReadClip("Enemy_norAttack3");
	ReadClip("Enemy_powAttack1");
	ReadClip("Enemy_powAttack2");
	ReadClip("Enemy_jumpup");
	ReadClip("Enemy_jumping");
	ReadClip("Enemy_jumpdown");
	ReadClip("Enemy_down");
	ReadClip("Enemy_damaged");
	ReadClip("Enemy_Dying");
	CreateTexture();
	reader->GetMaterial()[0]->Load(L"Enemy1.mat");
	reader->GetMaterial()[1]->Load(L"Enemy1.mat");
	reader->GetMaterial()[2]->Load(L"Enemy3.mat");

	clips[JUMPUP]->SetEndEvent(0.0f, bind(&Enemy::SetClip, this, JUMPING, 2.0f));
	clips[JUMPDOWN]->SetEndEvent(0.0f, bind(&Enemy::IsJumped, this, false));
	clips[NORATTACK1]->SetEndEvent(0.8f, bind(&Enemy::AttackEndEvent, this));
	clips[NORATTACK2]->SetEndEvent(0.8f, bind(&Enemy::AttackEndEvent, this));
	clips[NORATTACK3]->SetEndEvent(0.8f, bind(&Enemy::AttackEndEvent, this));
	clips[POWATTACK1]->SetEndEvent(0.8f, bind(&Enemy::AttackEndEvent, this));
	clips[POWATTACK2]->SetEndEvent(0.8f, bind(&Enemy::AttackEndEvent, this));
	clips[DOWN]->SetEndEvent(0.4f, bind(&Enemy::SetClip, this, DYING, 0.0f));
	clips[DOWN]->SetEndEvent(0.0f, bind(&Enemy::Dead, this));

	colliderBody = new ColliderBox();
	colliderBody->SetParent(this);
	colliderBody->scale.x = 60.0f;
	colliderBody->scale.y = 60.0f;
	colliderBody->scale.y *= 1.25f;
	colliderBody->scale.z = 60.0f;
	colliderBody->translation += Vector3(0.0f, 60.0f, 0.0f);

	collidersensor = new ColliderBox();
	collidersensor->SetParent(this);
	collidersensor->scale.x = 120.0f;
	collidersensor->scale.y = 120.0f;
	collidersensor->scale.z = 120.0f;
	collidersensor->translation += Vector3(0.0f, 60.0f, 0.0f);

	colliderFoot = new ColliderSphere();
	colliderFoot->SetParent(this);
	colliderFoot->scale.x = 20.0f;
	colliderFoot->scale.y = 20.0f;
	colliderFoot->scale.z = 20.0f;
	colliderFoot->translation += Vector3(0.0f, 20.0f, 0.0f);

	LHand = new Transform();
	colliderLHand = new ColliderSphere();
	colliderLHand->SetParent(LHand);
	colliderLHand->scale.x = 20.0f;
	colliderLHand->scale.y = 20.0f;
	colliderLHand->scale.z = 20.0f;

	RHand = new Transform();
	colliderRHand = new ColliderSphere();
	colliderRHand->SetParent(RHand);
	colliderRHand->scale.x = 20.0f;
	colliderRHand->scale.y = 20.0f;
	colliderRHand->scale.z = 20.0f;

	SetClip(IDLE);

	static int count = 1;
	hpBar = new HPBar_amped(L"UI/CharacterBoxEnemy.png");
	hpBar->SetLabel("HP Bar Enemy" + count++);
	hpBar->translation.x = 250.0f * count;
	hpBar->translation.y = 150.0f;
}

Enemy::~Enemy()
{
	delete colliderBody;
	delete collidersensor;
	delete colliderFoot;
	delete colliderLHand;
	delete colliderRHand;
	delete LHand;
	delete RHand;
	delete hpBar;
}

void Enemy::Update()
{
	if (!isLive) return;

	ModelAnimator::Update();
	colliderBody->Update();
	collidersensor->Update();
	colliderFoot->Update();
	colliderLHand->Update();
	colliderRHand->Update();
	LHand->Update();
	RHand->Update();
	hpBar->Update();
	UpdateHand();
	damagedCoolTime -= Time::Delta();

	if (!SCENE->IsPlay()) return;

	if (isDead)
	{
		if (isDeadTime <= 2.0f) isDeadTime += Time::Delta();
		else isLive = false;
	}

	isCollision = false;
	gravity();

	if (gravityspeed < -1.0f) isJump = true;
	else if (gravityspeed > 1.0f) isJump = true;

	if (Hp_cur <= 0)
	{
		return;
	}


	if (stuncoolTime > 0.0f)
	{
		stuncoolTime -= Time::Delta();
		moveForward = 0.0f;
		attackcoolTime = 0.75f;
		movecoolTime = 0.75f;
		isAttacked = false;
		return;
	}


	Attack();

	if (moveForward > 0)
	{
		moveForward -= Time::Delta();

		translation += lastrotate;

		if (GAMEMANAGER->GetPlayer()->GetCollider()->Collision(this->colliderBody))
		{
			translation -= lastrotate;
		}
	}

	if (isAttacked)
	{
		if (!isJump) lastmove = {};

		speed = moveSpeed;

		return;
	}


	jump();

	if (movecoolTime >= 0)
	{
		movecoolTime -= Time::Delta();

		return;
	}

	if (!isAttacked)
	{
		move();
	}

	CollisionEvent();
}

void Enemy::Render()
{
	if (!isLive) return;

	ModelAnimator::Render();
	colliderBody->Render();
	collidersensor->Render();
	colliderFoot->Render();
	colliderLHand->Render();
	colliderRHand->Render();
}

void Enemy::PostRender()
{
	Debug();

	hpBar->Render();
}

void Enemy::Debug()
{
	float distance = Distance(GAMEMANAGER->GetPlayer()->GetGlobalPosition(), globalPosition);
	ImGui::Text("Distance : %f", distance);
	ImGui::Checkbox("Jumped", &isJump);
	ImGui::Checkbox("isattack", &isAttacked);

}

void Enemy::SetClip(AnimState state, float speed)
{
	if (curState != state)
	{
		if (state == JUMPDOWN) PlayClip(state, 1.75f, 0.05f);
		else if (state == NORATTACK1) PlayClip(state, 1.5f, 0.05f);
		else if (state == NORATTACK2) PlayClip(state, 1.5f, 0.05f);
		else if (state == NORATTACK3) PlayClip(state, 2.0f, 0.05f);
		else if (state == POWATTACK1) PlayClip(state, 0.75f, 0.1f);
		else if (state == POWATTACK2) PlayClip(state, 1.0f, 0.1f);
		else PlayClip(state, speed, takeTime);
		curState = state;
	}
}

void Enemy::IsDamaged(float damage, float damageCooltime, bool isdown)
{
	if (damagedCoolTime > 0.0f || Hp_cur <= 0.0f) return;

	SoundManager::GetInstance()->Play("Damaged", 0.5f);
	ParticleManager::GetInstance()->Add(L"_TextData/Hit.fx", 0.15f, GetCollider()->GetGlobalPosition());
	Hp_cur -= damage;
	if (Hp_cur < 0.0f) Hp_cur = 0.0f;

	float value = 0.0f;
	value = Hp_cur / Hp_max;

	if (isdown)
	{
		damagedCoolTime = 1.25f;
		stuncoolTime = 1.5f;
		moveForward = 1.0f;
	}
	else
	{
		damagedCoolTime = damageCooltime;
		stuncoolTime = 0.3f;
		moveForward = 0.5f;
	}

	SetValue(value);
	if (isdown || Hp_cur <= 0.0f)
	{
		DamagedBaackWard(true);
		SetClip(DOWN);
	}
	else
	{
		DamagedBaackWard(false);
		SetClip(DAMAGED);
	}

}

void Enemy::move()
{
	float distance = Distance(GAMEMANAGER->GetPlayer()->translation, translation);

	if (distance > 5.0f)
	{
		isRun = true;
		speed = moveRunSpeed;
	}

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


	if (!isJump)
	{
		lastmove = dir * speed * Time::Delta();
		translation += lastmove;
	}
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

	if (!isJump && !isAttacked) isRun ? SetClip(RUN) : SetClip(WALK);

	if (GAMEMANAGER->GetPlayer()->GetCollider()->Collision(this->colliderBody))
	{
		translation -= lastmove;
	}
}

void Enemy::jump()
{
	if (isCollision && !isJump && !isAttacked)
	{
		if (curState != DOWN) SetClip(JUMPUP);
		isJump = true;

		gravityspeed = -4.0f;
	}
}

void Enemy::Attack()
{

	if (hand_leftEnable > 0) hand_leftEnable -= Time::Delta();
	if (hand_rightEnable > 0) hand_rightEnable -= Time::Delta();

	if (colliderLHand->Collision(GAMEMANAGER->GetPlayer()->GetCollider()) && hand_leftEnable > 0)
		GAMEMANAGER->GetPlayer()->IsDamaged(10.0f, hand_leftEnable + 0.01f, this->translation, bAttackPower);

	if (colliderRHand->Collision(GAMEMANAGER->GetPlayer()->GetCollider()) && hand_rightEnable > 0)
		GAMEMANAGER->GetPlayer()->IsDamaged(10.0f, hand_rightEnable + 0.01f, this->translation, bAttackPower);

	if (attackCoolTime > 0)
	{
		attackCoolTime -= Time::Delta();
		return;
	}

	if (isJump) return;


	if (collidersensor->Collision(GAMEMANAGER->GetPlayer()->GetCollider()))
	{
		float attackEvent = Random(0.0f, 100.0f);

		if (attackEvent < 50 && !firstAttackPow)
		{
			firstAttackNor = true;
			nextAttackedNor = true;

			if (!isAttacked) ComboEvent();
		}
		if (attackEvent >= 50 && !firstAttackNor)
		{
			firstAttackPow = true;
			nextAttackedPow = true;

			if (!isAttacked) ComboEvent();
		}
	}

}

void Enemy::gravity()
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
			isCollision = true;
		}

		if (colliderFoot->Collision(colliderBox) && colliderBox->WallTypes() == Collider::WallType::FLOOR)
		{
			float isgravity = gravityPower * gravityspeed * Time::Delta();
			if (isgravity > 0)
			{
				gravityspeed = 0.0f;

				if (isJump)
				{
					if (curState != DOWN) SetClip(JUMPDOWN);
					isAttacked = false;
					isJump = false;
				}
			}			
			translation.y += 2.0f * Time::Delta();

			if (isJump) isJump = false;
		}
	}

	if (isJump) translation += lastmove;
	translation.y -= gravityPower * gravityspeed * Time::Delta();

}

void Enemy::CollisionEvent()
{
	for (Enemy* enemy : GAMEMANAGER->GetEnemy())
	{
		if (colliderBody->Collision(enemy->GetCollider()))
		{
			if (translation.x < enemy->translation.x)
				translation.x -= 10.0f * Time::Delta();

			if (translation.x > enemy->translation.x)
				translation.x += 10.0f * Time::Delta();

			if (translation.z < enemy->translation.z)
				translation.z -= 10.0f * Time::Delta();

			if (translation.z > enemy->translation.z)
				translation.z += 10.0f * Time::Delta();
		}
	}

	if (colliderBody->Collision(GAMEMANAGER->GetPlayer()->GetCollider()))
	{
		if (translation.x < GAMEMANAGER->GetPlayer()->translation.x)
			translation.x -= 5.0f * Time::Delta();

		if (translation.x > GAMEMANAGER->GetPlayer()->translation.x)
			translation.x += 5.0f * Time::Delta();

		if (translation.z < GAMEMANAGER->GetPlayer()->translation.z)
			translation.z -= 5.0f * Time::Delta();

		if (translation.z > GAMEMANAGER->GetPlayer()->translation.z)
			translation.z += 5.0f * Time::Delta();
	}
}

void Enemy::UpdateHand()
{
	UINT nodeIndexR = reader->GetNodeIndex("mixamorig:LeftHand");
	Matrix nodeTransformR = GetTransformByNode(nodeIndexR);
	LHand->GetWorld() = nodeTransformR * world;

	UINT nodeIndexL = reader->GetNodeIndex("mixamorig:RightHand");
	Matrix nodeTransformL = GetTransformByNode(nodeIndexL);
	RHand->GetWorld() = nodeTransformL * world;
}

void Enemy::DamagedBaackWard(bool bDown)
{
	Vector3 dir = this->translation - GAMEMANAGER->GetPlayer()->translation;
	dir.Normalize();

	if (bDown)
	{
		translation += dir * 200.0f * Time::Delta();
	}
	else
	{
		translation += dir * 100.0f * Time::Delta();
	}
}

void Enemy::Dead()
{
	if (Hp_cur <= 0.0f)	isDead = true;
}

void Enemy::AttackEndEvent()
{
	if (isAttacked)
	{
		if ((nextAttackedNor && firstAttackNor) || (nextAttackedPow && firstAttackPow))
			ComboEvent();
		else
		{
			isAttacked = false;

			firstAttackNor = false;
			firstAttackPow = false;
			bAttackPower = false;

			attackNorCombo = 0;
			attackPowCombo = 0;
		}
	}
}

void Enemy::ComboEvent()
{
	isAttacked = true;

	nextAttackedNor = false;
	nextAttackedPow = false;

	if (firstAttackNor)
	{
		if (attackNorCombo >= 3)
		{
			isAttacked = false;
			firstAttackNor = false;
			bAttackPower = false;
			attackNorCombo = 0;
			attackCoolTime = 2.0f;
			return;
		}

		if (attackNorCombo == 0)
		{
			bAttackPower = false;
			attackNorCombo++;
			moveForward = 0.20f;
			hand_rightEnable = 0.5f;

			SetClip(NORATTACK1);
		}
		else if (attackNorCombo == 1)
		{
			bAttackPower = false;
			attackNorCombo++;
			moveForward = 0.2f;
			hand_leftEnable = 0.3f;

			SetClip(NORATTACK2);
		}
		else if (attackNorCombo == 2)
		{
			bAttackPower = true;
			attackNorCombo++;
			moveForward = 0.4f;
			hand_rightEnable = 1.0f;

			SetClip(NORATTACK3);
		}
	}
	else if (firstAttackPow)
	{

		if (attackPowCombo >= 2)
		{
			isAttacked = false;
			firstAttackPow = false;
			bAttackPower = false;
			attackPowCombo = 0;
			attackCoolTime = 2.0f;
			return;
		}

		if (attackPowCombo == 0)
		{
			bAttackPower = false;
			attackPowCombo++;

			moveForward = 0.4f;
			hand_rightEnable = 1.0f;

			SetClip(POWATTACK1);
		}
		else if (attackPowCombo == 1)
		{
			bAttackPower = true;
			attackPowCombo++;

			moveForward = 0.4f;
			hand_leftEnable = 1.0f;

			SetClip(POWATTACK2);
		}
	}
}
