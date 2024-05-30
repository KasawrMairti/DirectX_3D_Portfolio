#include "Framework.h"
#include "ColliderCapsule.h"


ColliderCapsule::ColliderCapsule(float radius, float height, UINT stackCount, UINT sliceCount)
	:radius(radius), height(height), stackCount(stackCount), sliceCount(sliceCount)
{
	type = CAPSHULE;

	CreateMesh();
}

ColliderCapsule::~ColliderCapsule()
{
}

bool ColliderCapsule::Collision(IN Ray& ray, OUT Contact* contact)
{
    UpdateWorld();

    Vector3 direction = Up();

    Vector3 pa = globalPosition - direction * Height() * 0.5f;
    Vector3 pb = globalPosition + direction * Height() * 0.5f;

    float r = Radius();

    Vector3 ro = ray.origin;
    Vector3 rd = ray.direction;

    Vector3 ba = pb - pa;
    Vector3 oa = ro - pa;

    float baba = Vector3::Dot(ba, ba);
    float bard = Vector3::Dot(ba, rd);
    float baoa = Vector3::Dot(ba, oa);
    float rdoa = Vector3::Dot(rd, oa);
    float oaoa = Vector3::Dot(oa, oa);

    float a = baba - bard * bard;
    float b = baba * rdoa - baoa * bard;
    float c = baba * oaoa - baoa * baoa - r * r * baba;
    float h = b * b - a * c;

    if (h >= 0.0f)
    {
        float t = (-b - sqrt(h)) / a;

        float y = baoa + t * bard;

        if (y > 0.0f && y < baba)
        {
            if (contact)
            {
                contact->distance = t;
                contact->hitPoint = ray.origin + ray.direction * t;
            }

            return true;
        }

        Vector3 oc = (y <= 0.0f) ? oa : ro - pb;
        b = Vector3::Dot(rd, oc);
        c = Vector3::Dot(oc, oc) - r * r;
        h = b * b - c;

        if (h > 0.0f)
        {
            if (contact)
            {
                contact->distance = t;
                contact->hitPoint = ray.origin + ray.direction * t;
            }

            return true;
        }
    }

    return false;
}

bool ColliderCapsule::Collision(ColliderBox* other)
{
    return other->Collision(this);
}

bool ColliderCapsule::Collision(ColliderSphere* other)
{
    Vector3 direction = Up();
    Vector3 A = globalPosition - direction * Height() * 0.5f;
    Vector3 B = globalPosition + direction * Height() * 0.5f;

    Vector3 P = other->GetGlobalPosition();

    Vector3 pointOnLine = ClosestPointOnLine(A, B, P);

    float distance = Distance(P, pointOnLine);

    return distance <= (Radius() + other->Radius());
}

bool ColliderCapsule::Collision(ColliderCapsule* other)
{
    Vector3 aDirection = Up();
    Vector3 aA = globalPosition - aDirection * Height() * 0.5f;
    Vector3 aB = globalPosition + aDirection * Height() * 0.5f;

    Vector3 bDirection = other->Up();
    Vector3 bA = other->globalPosition - bDirection * other->Height() * 0.5f;
    Vector3 bB = other->globalPosition + bDirection * other->Height() * 0.5f;

    Vector3 v0 = bA - aA;
    Vector3 v1 = bB - aA;
    Vector3 v2 = bA - aB;
    Vector3 v3 = bB - aB;

    float d0 = Vector3::Dot(v0, v0);
    float d1 = Vector3::Dot(v1, v1);
    float d2 = Vector3::Dot(v2, v2);
    float d3 = Vector3::Dot(v3, v3);

    Vector3 bestA;
    if (d2 < d0 || d2 < d1 || d3 < d0 || d3 > d1)
        bestA = aB;
    else
        bestA = aA;

    Vector3 bestB = ClosestPointOnLine(bA, bB, bestA);
    bestA = ClosestPointOnLine(aA, aB, bestB);

    float distance = Distance(bestA, bestB);

    return distance <= (Radius() + other->Radius());
}

void ColliderCapsule::CreateMesh()
{
    float   phiStep = XM_PI / stackCount;
    float thetaStep = XM_2PI / sliceCount;

    //Vertices
    for (UINT i = 0; i <= stackCount; i++)
    {
        float phi = i * phiStep;

        for (UINT j = 0; j <= sliceCount; j++)
        {
            float theta = j * thetaStep;

            Vertex vertex;
            vertex.pos.x = sin(phi) * cos(theta) * radius;
            vertex.pos.y = cos(phi) * radius;
            vertex.pos.z = sin(phi) * sin(theta) * radius;

            if (vertex.pos.y > 0)
                vertex.pos.y += height * 0.5f;
            else if (vertex.pos.y < 0)
                vertex.pos.y -= height * 0.5f;

            vertices.push_back(vertex);
        }
    }

    for (UINT i = 0; i < stackCount; i++)
    {
        for (UINT j = 0; j < sliceCount; j++)
        {
            indices.push_back((sliceCount + 1) * (i + 0) + j + 0);//0
            indices.push_back((sliceCount + 1) * (i + 0) + j + 1);//1            
            indices.push_back((sliceCount + 1) * (i + 0) + j + 0);//0
            indices.push_back((sliceCount + 1) * (i + 1) + j + 0);//2            
        }
    }

    mesh = new Mesh(vertices, indices);	
}
