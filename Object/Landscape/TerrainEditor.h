#pragma once


class TerrainEditor : public Transform
{
	typedef VertexTextureNormalTangentAlpha VertexType;
public:
	TerrainEditor(UINT height = 100, UINT width = 100);
	~TerrainEditor();

	void Update();
	void Render();

	void Debug();

	Material* GetMaterial() { return material; }

	bool Picking(OUT Vector3* position);

	void SaveHeightMap(wstring file);
	void LoadHeightMap(wstring file);

	void SaveHeightDialog();
	void LoadHeightDialog();

private:
	void CreateMesh();
	void CreateNormal();
	void CreateTangent();
	void CreateCompute();

	void AdjustHeight();
	void AdjustAlpha();

private:
	vector<VertexType> vertices;
	vector<UINT>        indices;

	Material* material;
	Mesh*     mesh;

	UINT  width;
	UINT height;

	Texture* heightMap;

	const float MAP_HEIGHT = 20.0f;

	////ComputeShader

	struct InputDesc
	{
		UINT index;

		Vector3 v0, v1, v2;
	};

	struct OutputDesc
	{
		int isPicked = 0;

		float u, v;

		float distance;
	};

	StructuredBuffer* structuredBuffer;
	       RayBuffer*        rayBuffer;

	ComputeShader* computeShader;

	 InputDesc*  input = nullptr;
	OutputDesc* output = nullptr;

	UINT polygonCount;

	Vector3 pickedPos;

	BrushBuffer* brushBuffer;

	float adjustValue = 20.0f;

	bool isRaise = true;

	////alphaMap

	Texture*  alphaMap = nullptr;
	Texture* secondMap = nullptr;

	UINT selectedMap = 0;

	bool adjustAlpha = true;
};
