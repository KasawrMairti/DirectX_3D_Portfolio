#pragma once

class Quad : public Transform
{
	typedef VertexTextureNormalTangent VertexType;
public:
	Quad(Vector2 size = { 1, 1 });
	Quad(wstring file);
	~Quad();

	void Render();
	void RenderInstanced(UINT instanceCount);

	Material* GetMaterial() { return material; }
	bool& IsActive() { return isActive;	}

protected:
	Material* material = nullptr;
	Mesh*     mesh     = nullptr;

	vector<VertexType> vertices;
	vector<UINT>        indices;

	bool isActive = true;
};
