#pragma once

class ParticleManager : public Singleton<ParticleManager>
{
	friend class Singleton;
private:
	ParticleManager();
	~ParticleManager();

public:
	void Update();
	void Render();

	void Add(wstring file, float time, Vector3 pos, float scale = 1.0f);

private:
	vector<Particle*> particles;
};