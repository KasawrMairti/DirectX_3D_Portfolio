#pragma once

struct Ray
{
	Vector3 origin;
	Vector3 direction;
};

struct Contact
{
	Vector3 hitPoint;
	float	distance;
};

class Collider : public Transform
{
protected:
	typedef Vertex VertexType;

public:
	enum Type
	{
		BOX, SPHERE, CAPSHULE
	};

	enum WallType
	{
		FLOOR, WALL, CEILING, SIZE
	};

	static bool hiddenInGame;

public:
	Collider();
	virtual ~Collider();

	bool Collision(Collider* other);

	virtual bool Collision(IN Ray& ray, OUT Contact* contact = nullptr) = 0;
	virtual bool Collision(class ColliderBox*	  other) = 0;
	virtual bool Collision(class ColliderSphere*  other) = 0;
	virtual bool Collision(class ColliderCapsule* other) = 0;

	virtual void CreateMesh() = 0;

	void Render();

	void SetColor(float r, float g, float b) { material->GetBuffer()->data.diffuse = { r, g, b, 1.0f }; }

	WallType& WallTypes() { return wallType; }

protected:
	Material* material;
	Mesh* mesh;

	Type type;
	WallType wallType;

	vector<VertexType> vertices;
	vector<UINT>		indices;
};