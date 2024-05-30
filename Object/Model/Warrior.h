#pragma once

class Warrior : public ModelAnimator
{
	enum AnimState
	{
		IDLE, WALK, NORATTACK, POWATTACK, DAMAGED, DIE, DYING, SIZE
	} curState = SIZE;

public:
	Warrior();
	~Warrior();

	void Update();
	void Render();
	void PostRender();

	void Debug();

	void SetClip(AnimState state, float speed = 1.0f);

	void IsDamaged(float damage, float damageCooltime = 0.5f, bool isdown = false);

	Collider* GetCollider() { return colliderBody; }

	float GetHP() { return Hp_cur; }

	bool& IsActive() { return isActive; }

private:
	void move();
	void Attack();

	void AttackNorEndEvent();
	void AttackPowDamageEvent();
	void AttackPowEndEvent();

	void gravity();

	void UpdateHand();

	void SetValue(float value) { hpBar->SetValue(value); }

private:
	Collider* colliderBody;
	Collider* colliderFoot;
	Collider* colliderLHand;
	Collider* colliderRHand;
	Collider* collidersensornear;
	Collider* collidersensorfar;
	Transform* LHand;
	Transform* RHand;
	bool isActive = false;

	// hp
	HPBar_boss* hpBar;
	float Hp_cur = 100.0f;
	float Hp_max = 100.0f;

	// gravity
	float gravityPower = 3.14f;
	float gravityspeed = 0.0f;

	// Walk
	float speed = 1.0f;
	float moveSpeed = 1.75f;
	float moveForward = 0.0f;
	Vector3 lastmove = {};
	Vector3 lastrotate = {};

	// Attack
	bool isAttack = false;
	float damagedPow = 15.0f;
	float attackCoolTime = 0.0f;
	float damagedCoolTime = 0.0f;
	float hand_leftEnable = 0.0f;
	float hand_rightEnable = 0.0f;
};