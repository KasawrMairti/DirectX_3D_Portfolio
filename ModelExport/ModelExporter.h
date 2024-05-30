#pragma once

class ModelExporter
{
public:
	ModelExporter(string name);
	~ModelExporter();
	
	void ExportModel();

	void ExportClip(string file);
	
private:
	void ExportMaterial();
	void ExportMesh();

	wstring CreateTexture(string file);

	void ReadMesh(aiNode* node);
	void ReadNode(aiNode* node, int index, int parent);
	void ReadBone(aiMesh* mesh, vector<VertexWeights>& vertexWeights);

	void WriteMesh();

	Clip* ReadClip(aiAnimation* animation);
	void WriteClip(Clip* clip, string file);

	void ReadKeyFrame(Clip* clip, aiNode* node, vector<ClipNode>& clipNodes);

private:
	string name;

	Assimp::Importer* importer;

	const aiScene* scene;

	vector<MeshData*> meshes;

	vector<string> materialNames;

	vector<NodeData*> nodes;
	vector<BoneData*> bones;

	map<string, UINT> boneMap;

	UINT boneCount = 0;
};
