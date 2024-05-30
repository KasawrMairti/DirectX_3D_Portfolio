#include "Framework.h"
#include "Particle.h"

Particle::Particle(wstring file, float timelimit, Vector3 pos)
	:timelimit(timelimit)
{
	translation = pos;


	instanceData.resize(MAX_COUNT);

	instanceBuffer = new VertexBuffer(instanceData);

	Init();
	Load(file);

}

Particle::~Particle()
{
	delete instanceBuffer;
}

void Particle::Update()
{
	lifeTime += Time::Delta();

	UpdatePhysics();
	UpdateColor();

	Quad::Update();

	if (lifeTime > particleData.duration)
		Init();

	timelimit -= Time::Delta();
}

void Particle::Render()
{
	//Quad::Render();

	if (particleData.isAdditive)
		STATE->AdditiveBegin();
	else
		STATE->AlphaToCoverageEnable();

	STATE->DepthWriteMaskZero();

	instanceBuffer->IASetBuffer(1);

	RenderInstanced(drawCount);

	STATE->AlphaEnd();
	STATE->DepthEnable();
}

void Particle::UpdatePhysics()
{
	drawCount = 0;

	for (ParticleInfo& info : particleInfos)
	{
		if (info.startTime > lifeTime)
			continue;

		info.velocity += info.acceleration * Time::Delta();

		info.transform.translation += info.velocity * info.speed * Time::Delta();

		info.transform.rotation.z += info.angularVelocity * Time::Delta();

		if (particleData.isBillboard)
		{
			info.transform.rotation.y = CAMERA->rotation.y;

			Vector3 dir = info.transform.GetGlobalPosition() - CAMERA->GetGlobalPosition();

			float angle = atan2f(dir.x, dir.z);

			info.transform.rotation.y = angle;
		}

		float timeScale = (lifeTime - info.startTime) / (particleData.duration - info.startTime);

		info.transform.scale = LERP(info.startScale, info.endScale, timeScale);

		info.transform.Update();

		instanceData[drawCount++].transform = XMMatrixTranspose(info.transform.GetWorld());
	}

	instanceBuffer->UpdateVertex(instanceData.data(), drawCount);
}

void Particle::UpdateColor()
{
	float timeScale = lifeTime / particleData.duration;

	Vector4 color;
	color.x = LERP(particleData.startColor.x, particleData.endColor.x, timeScale);
	color.y = LERP(particleData.startColor.y, particleData.endColor.y, timeScale);
	color.z = LERP(particleData.startColor.z, particleData.endColor.z, timeScale);
	color.w = LERP(particleData.startColor.w, particleData.endColor.w, timeScale);

	GetMaterial()->GetBuffer()->data.diffuse = color;
}

void Particle::Init()
{
	lifeTime = 0.0f;
	drawCount = 0;
	particleData.count = particleCount;

	 instanceData.clear();
	particleInfos.clear();

	 instanceData.resize(particleData.count);
	particleInfos.resize(particleData.count);

	UINT count = 0;

	for (ParticleInfo& info : particleInfos)
	{
		info.transform.translation = Random(particleData.minStartPos,        particleData.maxStartPos);
		info.velocity              = Random(particleData.minVelocity,        particleData.maxVelocity);
		info.acceleration          = Random(particleData.minAcceleration,    particleData.maxAcceleration);
		info.startScale            = Random(particleData.minStartScale,      particleData.maxStartScale);
		info.endScale              = Random(particleData.minEndScale,        particleData.maxEndScale);
		info.speed                 = Random(particleData.minSpeed,           particleData.maxSpeed);
		info.angularVelocity       = Random(particleData.minAngularVelocity, particleData.maxAngularVelocity);
		info.startTime             = Random(particleData.minStartTime,       particleData.maxStartTime);

		info.velocity.Normalize();

		count++;
	}
}

void Particle::Load(wstring file)
{
	BinaryReader* data = new BinaryReader(file);

	wstring diffusePath = data->ReadWString();

	GetMaterial()->SetShader(L"18Particle");
	GetMaterial()->SetDiffuseMap(diffusePath);

	ParticleData* temp = new ParticleData();

	data->ReadData((void**)&temp, sizeof(ParticleData));

	particleData = *temp;

	particleCount = particleData.count;

	delete temp;
	delete data;
}