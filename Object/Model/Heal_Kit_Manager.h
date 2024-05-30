#pragma once

class Heal_Kit_Manager
{
public:
	Heal_Kit_Manager();
	~Heal_Kit_Manager();

	void Update();
	void Render();

	vector<Heal_Kit*> GetHeal() { return heals; }

private:
	vector<Heal_Kit*> heals;

	float time = 0.0f;
	float timeMax = 10.0f;
};