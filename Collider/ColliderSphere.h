#pragma once

class ColliderSphere : public Collider
{
public:
	ColliderSphere(float radius = 1.0f, UINT stackCount = 15, UINT sliceCount = 30);
	~ColliderSphere();

	virtual bool Collision(IN Ray& ray, OUT Contact* contact) override;
	virtual bool Collision(ColliderBox* other) override;
	virtual bool Collision(ColliderSphere* other) override;
	virtual bool Collision(ColliderCapsule* other) override;

	float Radius() { return radius * Max(globalScale.x, globalScale.y, globalScale.z); }

private:
	virtual void CreateMesh() override;

	float radius;
	UINT stackCount;
	UINT sliceCount;

};