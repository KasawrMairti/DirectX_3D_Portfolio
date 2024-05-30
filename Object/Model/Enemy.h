#pragma once

class Enemy : public ModelAnimator
{
	enum AnimState
	{
		IDLE, WALK, RUN, NORATTACK1, NORATTACK2, NORATTACK3, POWATTACK1, POWATTACK2, JUMPUP, JUMPING, JUMPDOWN, DOWN, DAMAGED, DYING, SIZE
	} curState = SIZE;

public:
	Enemy();
	~Enemy();

	void Update();
	void Render();
	void PostRender();

	void Debug();

	void SetClip(AnimState state, float speed = 1.0f);

	void IsDamaged(float damage, float damageCooltime = 0.5f, bool isdown = false);

	Collider* GetCollider() { return colliderBody; }

	bool& IsLive() { return isLive; }

private:
	void move();
	void jump();
	void Attack();

	void gravity();

	void CollisionEvent();

	void UpdateHand();

	void DamagedBaackWard(bool bDown);

	void Dead();

	void SetValue(float value) { hpBar->SetValue(value); }

	void IsJumped(bool isjumped) { isJump = isjumped; }

private:
	void AttackEndEvent();
	void ComboEvent();

private:
	bool isDead = false;
	bool isLive = true;
	float isDeadTime = 0.0f;

	bool isRun = false;
	float speed = 1.0f;
	float takeTime = 0.2f;

	bool isCollision = false;
	float movecoolTime = 0.0f;
	float attackcoolTime = 0.0f;
	float stuncoolTime = 0.0f;

	float moveSpeed = 1.75f;
	float moveRunSpeed = 2.5f;
	float moveForward = 0.0f;
	Vector3 lastmove = {};
	Vector3 lastrotate = {};

	bool isJump = false;
	float gravityPower = 3.14f;
	float gravityspeed = 0.0f;

	bool isAttacked = false;
	bool firstAttackNor = false;
	bool firstAttackPow = false;
	bool nextAttackedNor = false;
	bool nextAttackedPow = false;
	bool bAttackPower = false;
	float hand_leftEnable = 0.0f;
	float hand_rightEnable = 0.0f;
	UINT attackNorCombo = 0;
	UINT attackPowCombo = 0;
	float attackCoolTime = 0.0f;

	Collider* colliderBody;
	Collider* collidersensor;
	Collider* colliderFoot;
	Collider* colliderLHand;
	Collider* colliderRHand;
	Transform* LHand;
	Transform* RHand;

	HPBar_amped* hpBar;
	float Hp_cur = 20.0f;
	float Hp_max = 20.0f;

	float damagedCoolTime = 0.0f;
};