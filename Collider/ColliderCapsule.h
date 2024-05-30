#pragma once

class ColliderCapsule : public Collider
{
public:
	ColliderCapsule(float radius = 1.0f, float height = 2.0f,
		UINT stackCount = 15, UINT sliceCount = 30);
	~ColliderCapsule();

	virtual bool Collision(IN Ray& ray, OUT Contact* contact) override;
	virtual bool Collision(ColliderBox* other) override;
	virtual bool Collision(ColliderSphere* other) override;
	virtual bool Collision(ColliderCapsule* other) override;

	float Radius() { return radius * Max(globalScale.x, globalScale.y, globalScale.z); }
	float Height() { return height * globalScale.y; }

private:
	virtual void CreateMesh() override;

	float radius;
	float height;
	UINT stackCount;
	UINT sliceCount;
};