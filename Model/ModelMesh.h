#pragma once

class ModelMesh
{
	friend class ModelReader;
public:
	ModelMesh();
	~ModelMesh();

	void Create();

	void Render();
	void RenderInstanced(UINT intanceCount);

private:
	string name;

	Material* material;

	Mesh* mesh;

	vector<ModelVertex> vertices;
	vector<UINT>         indices;
};
