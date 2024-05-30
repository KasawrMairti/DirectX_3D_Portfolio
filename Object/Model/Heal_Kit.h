#pragma once

class Heal_Kit : public TextureCube
{
public:
	Heal_Kit();
	~Heal_Kit();

	void Update();
	void Render();

	bool& IsActive() { return isActive; }
	Collider* GetCollider() { return collider; }

private:
	Collider* collider;

	bool isActive = false;
};