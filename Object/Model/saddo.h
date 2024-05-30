#pragma once

class Saddo : public ModelAnimator
{
public:
	Saddo();
	~Saddo();

	void Update();
	void Render();

	void Debug();

	void UpdateRightHand();

private:
	float speed = 1.0f;
	float takeTime = 0.2f;

	Sphere* weapon;

	Transform* rightHand;
};