#include "Framework.h"
#include "Jacky.h"


Jacky::Jacky()
	:ModelAnimator("jacky noboru")
{
	GAMEMANAGER->SetPlayer(this);

	scale *= 0.01f;

	ReadClip("jacky_Idle");
	ReadClip("jacky_walk");
	ReadClip("jacky_run");
	ReadClip("jacky_norAttack1");
	ReadClip("jacky_norAttack2");
	ReadClip("jacky_norAttack3");
	ReadClip("jacky_powAttack1");
	ReadClip("jacky_powAttack2");
	ReadClip("jacky_jumpup");
	ReadClip("jacky_jumping");
	ReadClip("jacky_jumpdown");
	ReadClip("jacky_jumpkick");
	ReadClip("jacky_down");
	ReadClip("jacky_damaged");
	CreateTexture();
	reader->GetMaterial()[0]->Load(L"Jacky.mat");

	clips[JUMPUP]->SetEndEvent(0.0f, bind(&Jacky::SetClip, this, JUMPING, 2.0f));
	clips[NORATTACK1]->SetEndEvent(0.8f, bind(&Jacky::AttackEndEvent, this));
	clips[NORATTACK2]->SetEndEvent(0.8f, bind(&Jacky::AttackEndEvent, this));
	clips[NORATTACK3]->SetEndEvent(0.8f, bind(&Jacky::AttackEndEvent, this));
	clips[POWATTACK1]->SetEndEvent(0.8f, bind(&Jacky::AttackEndEvent, this));
	clips[POWATTACK2]->SetEndEvent(0.8f, bind(&Jacky::AttackEndEvent, this));

	SetClip(IDLE);

	colliderBody = new ColliderBox();
	colliderBody->SetParent(this);
	colliderBody->scale *= 35.0f;
	colliderBody->scale.y *= 3.0f;
	colliderBody->translation += Vector3(0.0f, 60.0f, 0.0f);

	colliderFoot = new ColliderSphere();
	colliderFoot->SetParent(this);
	colliderFoot->scale *= 20.0f;
	colliderFoot->translation += Vector3(0.0f, 20.0f, 0.0f);

	LHand = new Transform();
	colliderLHand = new ColliderSphere();
	colliderLHand->SetParent(LHand);
	colliderLHand->scale *= 15.0f;

	RHand = new Transform();
	colliderRHand = new ColliderSphere();
	colliderRHand->SetParent(RHand);
	colliderRHand->scale *= 15.0f;

	hpBar = new HPBar_amped(L"UI/CharacterBox.png");
	hpBar->SetLabel("HP Bar");
	hpBar->translation.x = 250.0f;
	hpBar->translation.y = 150.0f;

	CAMERA->SetTarget(this);

	Update();
	Render();
}

Jacky::~Jacky()
{
	delete colliderBody;
	delete colliderFoot;
	delete colliderLHand;
	delete colliderRHand;
	delete LHand;
	delete RHand;
	delete hpBar;
}

void Jacky::Update()
{
	ModelAnimator::Update();
	colliderBody->Update();
	colliderFoot->Update(); 
	colliderLHand->Update();
	colliderRHand->Update();
	LHand->Update();
	RHand->Update();
	hpBar->Update();
	UpdateHand();
	damagedCoolTime -= Time::Delta();

	if (!SCENE->IsPlay()) return;

	gravity();

	for (Heal_Kit* heal : GAMEMANAGER->GetHeal()->GetHeal())
	{
		if (heal->IsActive())
		{
			if (heal->GetCollider()->Collision(colliderBody))
			{
				heal->IsActive() = false;

				Hp_cur += 20.0f;
				if (Hp_cur > Hp_max) Hp_cur = Hp_max;

				float value = 0.0f;
				value = Hp_cur / Hp_max;

				SetValue(value);
			}
		}
	}

	if (stuncoolTime > 0) 
	{
		stuncoolTime -= Time::Delta();

		isAttacked = false;
		return;
	}


	Attack();
	jump();
	if (!isAttacked) move();

	if (moveForward > 0)
	{
		moveForward -= Time::Delta();

		translation += lastrotate * 1.0f;

		for (auto enemy : GAMEMANAGER->GetEnemy())
		{
			if (colliderBody->Collision(enemy->GetCollider()) && enemy->IsLive())
			{
				translation -= lastrotate * 5.0f;
			}
		}

		//if (colliderBody->Collision(GAMEMANAGER->GetWarrior()->GetCollider()) && GAMEMANAGER->GetWarrior()->IsActive())
		//	translation -= lastrotate * 5.0f;
	}
}

void Jacky::Render()
{
	ModelAnimator::Render();
	colliderBody->Render();
	colliderFoot->Render();
	colliderLHand->Render();
	colliderRHand->Render();
}

void Jacky::PostRender()
{
	Debug();

	hpBar->Render();
}

void Jacky::Debug()
{
	const char* list[] = { "IDLE", "WALK", "RUN", "NORATTACK1", "NORATTACK2", "NORATTACK3", "POWATTACK1", "POWATTACK2", "JUMPUP", "JUMPING", "JUMPDOWN", "JUMPKICK", "SIZE" };
	//ImGui::Combo("Animation", (int*)&curState, list, (int)SIZE);
	ImGui::SliderFloat("gravityspeed", &gravityspeed, -1000, 1000);
	ImGui::Checkbox("Jumped", &isJump);
	ImGui::Checkbox("isattack", &isAttacked);

	ImGui::SliderFloat3("Jacky rotation", (float*)&rotation, -3.14f, 3.14f);
}

void Jacky::SetClip(AnimState state, float speed)
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

void Jacky::IsDamaged(float damage, float damageCooltime, Vector3 enemyTranslation, bool isdown)
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
		damagedCoolTime = 1.0f;
		stuncoolTime = 1.05f;
		moveForward = 1.0f;
	}
	else
	{
		damagedCoolTime = damageCooltime;
		stuncoolTime = 0.3f;
	}

	SetValue(value);
	if (isdown || Hp_cur <= 0.0f)
	{
		DamagedBaackWard(true, enemyTranslation);
		SetClip(DOWN);
	}
	else
	{
		DamagedBaackWard(false, enemyTranslation);
		SetClip(DAMAGED);
	}
}

void Jacky::move()
{
	bool isRun = false;
	float speed = moveSpeed;
	Vector3 rotate = { 0, 0, 0 };

	if (KEY_PRESS(VK_SHIFT))
	{
		isRun = true;
		speed = moveRunSpeed;
	}

	if (KEY_PRESS(VK_UP) && KEY_PRESS(VK_RIGHT))
	{
		rotate = Vector3(0.7f, 0.0f, 0.7f) * Time::Delta();
		if (!isJump)
		{
			lastmove = rotate * speed;
		}
		lastrotate = rotate;
		rotation.y = XMConvertToRadians(-135);

		if (!isJump && !isAttacked) isRun ? SetClip(RUN) : SetClip(WALK);
	}
	else if (KEY_PRESS(VK_UP) && KEY_PRESS(VK_LEFT))
	{
		rotate = Vector3(-0.7f, 0.0f, 0.7f) * Time::Delta();
		if (!isJump)
		{
			lastmove = rotate * speed;
		}
		lastrotate = rotate;
		rotation.y = XMConvertToRadians(135);

		if (!isJump && !isAttacked) isRun ? SetClip(RUN) : SetClip(WALK);
	}
	else if (KEY_PRESS(VK_DOWN) && KEY_PRESS(VK_LEFT))
	{
		rotate = Vector3(-0.7f, 0.0f, -0.7f) * Time::Delta();
		if (!isJump)
		{
			lastmove = rotate * speed;
		}
		lastrotate = rotate;
		rotation.y = XMConvertToRadians(45);

		if (!isJump && !isAttacked) isRun ? SetClip(RUN) : SetClip(WALK);
	}
	else if (KEY_PRESS(VK_DOWN) && KEY_PRESS(VK_RIGHT))
	{
		rotate = Vector3(0.7f, 0.0f, -0.7f) * Time::Delta();
		if (!isJump)
		{
			lastmove = rotate * speed;
		}
		lastrotate = rotate;
		rotation.y = XMConvertToRadians(-45);

		if (!isJump && !isAttacked) isRun ? SetClip(RUN) : SetClip(WALK);
	}
	else if (KEY_PRESS(VK_UP))
	{
		rotate = Vector3(0.0f, 0.0f, 1.0f) * Time::Delta();
		if (!isJump)
		{
			lastmove = rotate * speed;
		}
		lastrotate = rotate;
		rotation.y = XMConvertToRadians(180);

		if (!isJump && !isAttacked) isRun ? SetClip(RUN) : SetClip(WALK);
	}
	else if (KEY_PRESS(VK_DOWN))
	{
		rotate = Vector3(0.0f, 0.0f, -1.0f)* Time::Delta();
		if (!isJump)
		{
			lastmove = rotate * speed;	
		}
		lastrotate = rotate;
		rotation.y = XMConvertToRadians(0);

		if (!isJump && !isAttacked) isRun ? SetClip(RUN) : SetClip(WALK);
	}
	else if (KEY_PRESS(VK_LEFT))
	{
		rotate = Vector3(-1.0f, 0.0f, 0.0f) * Time::Delta();
		if (!isJump)
		{
			lastmove = rotate * speed;
		}
		lastrotate = rotate;
		rotation.y = XMConvertToRadians(90);

		if (!isJump && !isAttacked) isRun ? SetClip(RUN) : SetClip(WALK);
	}
	else if (KEY_PRESS(VK_RIGHT))
	{
		rotate = Vector3(1.0f, 0.0f, 0.0f) * Time::Delta();
		if (!isJump)
		{
			lastmove = rotate * speed;
		}
		lastrotate = rotate;
		rotation.y = XMConvertToRadians(-90);

		if (!isJump && !isAttacked) isRun ? SetClip(RUN) : SetClip(WALK);
	}
	else
	{
		if (!isJump && !isAttacked) SetClip(IDLE);
		
		if (!isJump) lastmove = { 0, 0, 0 };
	}

	translation += lastmove;
	for (auto enemy : GAMEMANAGER->GetEnemy())
	{
		if (colliderBody->Collision(enemy->GetCollider()) && enemy->IsLive())
		{
			if (translation.x < enemy->translation.x && rotate.x > 0)
				translation.x -= speed * Time::Delta();

			if (translation.x > enemy->translation.x && rotate.x < 0)
				translation.x += speed * Time::Delta();

			if (translation.z < enemy->translation.z && rotate.z > 0)
				translation.z -= speed * Time::Delta();

			if (translation.z > enemy->translation.z && rotate.z < 0)
				translation.z += speed * Time::Delta();
		}
	}

	//if (colliderBody->Collision(GAMEMANAGER->GetWarrior()->GetCollider()))
	//{
	//	if (translation.x < GAMEMANAGER->GetWarrior()->translation.x && rotate.x > 0)
	//		translation.x -= speed * Time::Delta();
	//
	//	if (translation.x > GAMEMANAGER->GetWarrior()->translation.x && rotate.x < 0)
	//		translation.x += speed * Time::Delta();
	//
	//	if (translation.z < GAMEMANAGER->GetWarrior()->translation.z && rotate.z > 0)
	//		translation.z -= speed * Time::Delta();
	//
	//	if (translation.z > GAMEMANAGER->GetWarrior()->translation.z && rotate.z < 0)
	//		translation.z += speed * Time::Delta();
	//}
}

void Jacky::jump()
{
	if (KEY_DOWN('V') && !isJump && !isAttacked)
	{
		SetClip(JUMPUP);
		isJump = true;

		gravityspeed = -4.0f;
	}

}

void Jacky::Attack()
{
	if (hand_leftEnable > 0) hand_leftEnable -= Time::Delta();
	if (hand_rightEnable > 0) hand_rightEnable -= Time::Delta();

	for (auto enemy : GAMEMANAGER->GetEnemy())
	{
		if (colliderLHand->Collision(enemy->GetCollider()) && hand_leftEnable > 0 && enemy->IsLive())
			enemy->IsDamaged(10.0f, hand_leftEnable + 0.001f, bAttackPower);

		if (colliderRHand->Collision(enemy->GetCollider()) && hand_rightEnable > 0 && enemy->IsLive())
			enemy->IsDamaged(10.0f, hand_rightEnable + 0.001f, bAttackPower);
	}

	//if (colliderLHand->Collision(GAMEMANAGER->GetWarrior()->GetCollider()) && hand_leftEnable > 0)
	//	GAMEMANAGER->GetWarrior()->IsDamaged(10.0f, hand_leftEnable + 0.001f, bAttackPower);
	//
	//if (colliderRHand->Collision(GAMEMANAGER->GetWarrior()->GetCollider()) && hand_rightEnable > 0)
	//	GAMEMANAGER->GetWarrior()->IsDamaged(10.0f, hand_rightEnable + 0.001f, bAttackPower);

	if (attackCoolTime > 0)
	{
		attackCoolTime -= Time::Delta();
		return;
	}

	if (isJump) return;

	if (KEY_DOWN('X') && !firstAttackPow)
	{
		firstAttackNor = true;
		nextAttackedNor = true;

		if (!isAttacked) ComboEvent();
	}
	if (KEY_DOWN('C') && !firstAttackNor)
	{
		firstAttackPow = true;
		nextAttackedPow = true;

		if (!isAttacked) ComboEvent();
	}
}

void Jacky::gravity()
{
	gravityspeed += 9.6f * Time::Delta();

	for (Collider* colliderBox : GAMEMANAGER->GetBackground()->GetColliders())
	{
		if (colliderFoot->Collision(colliderBox) && colliderBox->WallTypes() == Collider::WallType::FLOOR)
		{
			float isgravity = gravityPower * gravityspeed * Time::Delta();
			if (isgravity > 0)
			{
				gravityspeed = 0.0f;

				if (isJump)
				{
					if (curState != DOWN) SetClip(JUMPDOWN);
				}
			}
			translation.y += 2.0f * Time::Delta();

			if (isJump) isJump = false;
		}

		if (colliderBox->Collision(colliderBody) && colliderBox->WallTypes() == Collider::WallType::WALL)
		{
			Vector3 dir = colliderBox->Forward();

			float speed = 6.0f;
			translation.x += dir.x * speed * Time::Delta();
			translation.z += dir.z * speed * Time::Delta();
		}

	}

	translation.y -= gravityPower * gravityspeed * Time::Delta();

	if (gravityspeed < -1.0f) isJump = true;
	else if (gravityspeed > 1.0f) isJump = true;
}

void Jacky::UpdateHand()
{
	UINT nodeIndexR = reader->GetNodeIndex("mixamorig:LeftHand");
	Matrix nodeTransformR = GetTransformByNode(nodeIndexR);
	LHand->GetWorld() = nodeTransformR * world;

	UINT nodeIndexL = reader->GetNodeIndex("mixamorig:RightHand");
	Matrix nodeTransformL = GetTransformByNode(nodeIndexL);
	RHand->GetWorld() = nodeTransformL * world;
}

void Jacky::DamagedBaackWard(bool bDown, Vector3 enemytranslation)
{
	Vector3 dir = this->translation - enemytranslation;
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

void Jacky::AttackEndEvent()
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

void Jacky::ComboEvent()
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
			attackCoolTime = 0.5f;
			return;
		}

		if (attackNorCombo == 0)
		{
			bAttackPower = false;
			attackNorCombo++;
			moveForward = 0.20f;
			hand_rightEnable = 0.5f;

			SetClip(NORATTACK2);
		}
		else if (attackNorCombo == 1)
		{
			bAttackPower = false;
			attackNorCombo++;
			moveForward = 0.2f;
			hand_leftEnable = 0.3f;

			SetClip(NORATTACK1);
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
			attackCoolTime = 0.5f;
			return;
		}

		if (attackPowCombo == 0)
		{
			bAttackPower = false;
			attackPowCombo++;

			moveForward = 0.4f;
			hand_rightEnable = 0.6f;

			SetClip(POWATTACK1, 2.0f);
		}
		else if (attackPowCombo == 1)
		{
			bAttackPower = true;
			attackPowCombo++;

			moveForward = 0.4f;
			hand_rightEnable = 0.6f;

			SetClip(POWATTACK2, 2.0f);
		}
	}
}
