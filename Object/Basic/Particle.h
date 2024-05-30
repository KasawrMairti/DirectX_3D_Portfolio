#pragma once

class Particle : public Quad
{
	const UINT MAX_COUNT = 1000;

	struct ParticleData
	{
		bool isLoop = false;
		bool isAdditive = true;
		bool isBillboard = true;

		UINT count = 100;

		float duration = 1.0f;

		Vector3 minStartPos = {};
		Vector3 maxStartPos = {};

		Vector3 minVelocity = { -1, -1, -1 };
		Vector3 maxVelocity = { +1, +1, +1 };

		Vector3 minAcceleration = {};
		Vector3 maxAcceleration = {};

		Vector3 minStartScale = { 1, 1, 1 };
		Vector3 maxStartScale = { 1, 1, 1 };

		Vector3 minEndScale = { 1, 1, 1 };
		Vector3 maxEndScale = { 1, 1, 1 };

		float minSpeed = 1.0f;
		float maxSpeed = 3.0f;

		float minAngularVelocity = -10.0f;
		float maxAngularVelocity = +10.0f;

		float minStartTime = 0.0f;
		float maxStartTime = 0.0f;

		Vector4 startColor = { 1, 1, 1, 1 };
		Vector4   endColor = { 1, 1, 1, 1 };
	};

	struct ParticleInfo
	{
		Transform transform;

		Vector3     velocity;
		Vector3 acceleration;
		Vector3   startScale;
		Vector3     endScale;

		float speed = 1.0f;
		float angularVelocity = 0.0f;
		float startTime = 0.0f;
	};

	struct InstanceData
	{
		Matrix  transform;
		Vector4 color = { 1, 1, 1, 1 };
	};

public:
	Particle(wstring file, float timelimit, Vector3 pos);
	~Particle();

	void Update();
	void Render();

	float TimeLimit() { return timelimit; }

private:
	void UpdatePhysics();
	void UpdateColor();

	void Init();

	void Load(wstring file);

private:
	vector<InstanceData> instanceData;
	vector<ParticleInfo> particleInfos;

	VertexBuffer* instanceBuffer;

	ParticleData particleData;

	float     lifeTime = 0.0f;
	UINT     drawCount = 0;
	UINT particleCount = 100;

	float timelimit = 0.0f;
};