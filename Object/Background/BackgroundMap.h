#pragma once

class BackgroundMap : public Model
{
public:
	BackgroundMap();
	~BackgroundMap();

	void Update();
	void Render();
	void Debug();

	void Initialize();

	void save_Collider();
	void load_Collider();

	vector<Collider*> GetColliders() { return colliders; }

private:
	vector<Collider*> colliders;

	UINT collider_count = 0;
};