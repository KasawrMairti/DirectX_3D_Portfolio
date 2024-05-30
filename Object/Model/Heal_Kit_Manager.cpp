#include "Framework.h"
#include "Heal_Kit_Manager.h"


Heal_Kit_Manager::Heal_Kit_Manager()
{
	GAMEMANAGER->SetHealKit(this);

	for (UINT i = 0; i < 10; i++)
	{
		Heal_Kit* heal = new Heal_Kit();
		heal->IsActive() = false;
		heals.push_back(heal);
	}
}

Heal_Kit_Manager::~Heal_Kit_Manager()
{
	for (Heal_Kit* heal : heals)
		delete heal;
	heals.clear();
}

void Heal_Kit_Manager::Update()
{
	for (Heal_Kit* heal : heals)
		heal->Update();

	time += Time::Delta();
	if (time >= timeMax)
	{
		timeMax = Random(5.0f, 25.0f);
		time = 0.0f;

		for (Heal_Kit* heal : heals)
		{
			if (!heal->IsActive())
			{
				heal->IsActive() = true;
				heal->translation.x = Random(-6.0f, 6.0f);
				heal->translation.z = Random(-6.0f, 6.0f);
				break;
			}
		}
	}
}

void Heal_Kit_Manager::Render()
{
	for (Heal_Kit* heal : heals)
		heal->Render();
}
