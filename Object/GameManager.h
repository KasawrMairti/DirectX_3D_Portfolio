#pragma once

class GameManager : public Singleton<GameManager>
{
public:
	void SetBackground(BackgroundMap* background) { this->background = background; };
	BackgroundMap* GetBackground() { return background; }

	void SetPlayer(Jacky* jacky) { this->jacky = jacky; };
	Jacky* GetPlayer() { return jacky; }

	void SetEnemy(Enemy* enemy) { enemies.push_back(enemy); }
	vector<Enemy*> GetEnemy() { return enemies; }

	void SetWarrior(Warrior* warrior) { this->warrior = warrior; }
	Warrior* GetWarrior() { return warrior; }

	void SetScene(Scene* scene) { this->curScene = scene; }
	Scene* GetScene() { return curScene; }

	void SetHealKit(Heal_Kit_Manager* heal) { this->heal = heal; }
	Heal_Kit_Manager* GetHeal() { return heal; }

private:
	BackgroundMap* background;
	Jacky* jacky;
	vector<Enemy*> enemies;
	Warrior* warrior;
	Heal_Kit_Manager* heal;
	Scene* curScene;
};