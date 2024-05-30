#pragma once

class ColliderBox : public Collider
{
public:
	struct Obb
	{
		Vector3 position;

		Vector3 axis[3];
		Vector3 halfSize;
	};

public:
	ColliderBox(Vector3 size = Vector3(1.0f, 1.0f, 1.0f));
	~ColliderBox();

	virtual bool Collision(IN Ray& ray, OUT Contact* contact) override;
	virtual bool Collision(ColliderBox* other) override;
	virtual bool Collision(ColliderSphere* other) override;
	virtual bool Collision(ColliderCapsule* other) override;

	Obb GetOBB();

	bool SeparatedAxis(Vector3 D, Vector3 axis, Obb box1, Obb box2);

private:
	virtual void CreateMesh() override;
	
private:
	Vector3 size;
};