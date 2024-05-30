#include "Framework.h"
#include "ColliderBox.h"


ColliderBox::ColliderBox(Vector3 size)
	:size(size)
{
	type = BOX;

	CreateMesh();
}

ColliderBox::~ColliderBox()
{
}

bool ColliderBox::Collision(IN Ray& ray, OUT Contact* contact)
{
	Transform::UpdateWorld();

	Contact temp;

	temp.distance = FLT_MAX;
	
	UINT faces[] =
	{
		0, 1, 2, 3,
		4, 5, 6, 7,
		1, 5, 3, 7,
		0, 4, 2, 6,
		0, 1, 4, 5,
		2, 3, 6, 7
	};

	for (UINT i = 0; i < 6; i++)
	{
		Vector3 p[4];

		for (UINT j = 0; j < 4; j++)
		{
			p[j] = vertices[faces[4 * i + j]].pos;
			p[j] = XMVector3TransformCoord(p[j], world);
		}

		float distance = 0.0f;

		if (TriangleTests::Intersects(ray.origin, ray.direction, p[0], p[1], p[2], distance))
		{
			if (temp.distance > distance)
			{
				temp.distance = distance;
				temp.hitPoint = ray.origin + ray.direction * distance;
			}
		}

		if (TriangleTests::Intersects(ray.origin, ray.direction, p[3], p[1], p[2], distance))
		{
			if (temp.distance > distance)
			{
				temp.distance = distance;
				temp.hitPoint = ray.origin + ray.direction * distance;
			}
		}
	}

	if (temp.distance == FLT_MAX)
		return false;

	if (contact != nullptr)
		*contact = temp;

	return true;
}

bool ColliderBox::Collision(ColliderBox* other)
{
	Obb box1 = this->GetOBB();
	Obb box2 = other->GetOBB();

	Vector3 D = box2.position - box1.position;

	for (UINT i = 0; i < 3; i++)
	{
		if (SeparatedAxis(D, box1.axis[i], box1, box2)) return false;
		if (SeparatedAxis(D, box2.axis[i], box1, box2)) return false;
	}

	for (UINT i = 0; i < 3; i++)
	{
		for (UINT j = 0; j < 3; j++)
		{
			if (box1.axis[i] == box2.axis[i]) return true;
		}
	}

	for (UINT i = 0; i < 3; i++)
	{
		for (UINT j = 0; j < 3; j++)
		{
			Vector3 cross = Vector3::Cross(box1.axis[i], box2.axis[j]);

			if (SeparatedAxis(D, cross, box1, box2)) return false;
		}
	}

	return true;
}

bool ColliderBox::Collision(ColliderSphere* other)
{
	Obb box = this->GetOBB();

	Vector3 pos = box.position;

	for (UINT i = 0; i < 3; i++)
	{
		float length = Vector3::Dot(box.axis[i], other->GetGlobalPosition() - this->globalPosition);

		float mult = (length < 0.0f) ? -1.0f : 1.0f;

		length = min(abs(length), box.halfSize[i]);

		pos += box.axis[i] * length * mult;
	}

	float distance = Distance(other->GetGlobalPosition(), pos);

	return distance <= other->Radius();
}

bool ColliderBox::Collision(ColliderCapsule* other)
{
	Obb box = GetOBB();

	Vector3 pos = box.position;

	Vector3 direction = other->Up();
	Vector3 a = other->GetGlobalPosition() - direction * other->Height() * 0.5f;
	Vector3 b = other->GetGlobalPosition() + direction * other->Height() * 0.5f;

	Vector3 pointOnLine = ClosestPointOnLine(a, b, pos);

	for (UINT i = 0; i < 3; i++)
	{
		float length = Vector3::Dot(box.axis[i], pointOnLine - box.position);

		float mult = (length < 0.0f) ? -1.0f : 1.0f;

		length = min(abs(length), box.halfSize[i]);

		pos += box.axis[i] * length * mult;
	}

	float distance = Distance(pointOnLine, pos);

	return distance <= other->Radius();
}

ColliderBox::Obb ColliderBox::GetOBB()
{
	Obb obb;
	obb.position = this->globalPosition;

	obb.axis[0] = this->right;
	obb.axis[1] = this->up;
	obb.axis[2] = this->forward;

	obb.halfSize = size * 0.5f * globalScale;

	return obb;
}

bool ColliderBox::SeparatedAxis(Vector3 D, Vector3 axis, Obb box1, Obb box2)
{
	float distance = abs(Vector3::Dot(D, axis));

	float sum = 0.0f;

	for (UINT i = 0; i < 3; i++)
	{
		sum += abs(Vector3::Dot(box1.axis[i] * box1.halfSize[i], axis));
		sum += abs(Vector3::Dot(box2.axis[i] * box2.halfSize[i], axis));
	}

	return distance > sum;
}

void ColliderBox::CreateMesh()
{
	Vector3 halfSize = size * 0.5f;

	vertices =
	{
		{ -halfSize.x, +halfSize.y, -halfSize.z },
		{ +halfSize.x, +halfSize.y, -halfSize.z },
		{ -halfSize.x, -halfSize.y, -halfSize.z },
		{ +halfSize.x, -halfSize.y, -halfSize.z },
		{ -halfSize.x, +halfSize.y, +halfSize.z },
		{ +halfSize.x, +halfSize.y, +halfSize.z },
		{ -halfSize.x, -halfSize.y, +halfSize.z },
		{ +halfSize.x, -halfSize.y, +halfSize.z }
	};

	indices =
	{
		0, 1, 2, 3, 4, 5, 6, 7,
		0, 4, 1, 5, 2, 6, 3, 7,
		0, 2, 1, 3, 4, 6, 5, 7
	};

	mesh = new Mesh(vertices, indices);
}
