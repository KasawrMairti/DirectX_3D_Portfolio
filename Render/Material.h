#pragma once

class Material
{
public:
	Material();
	Material(wstring file);
	~Material();

	void SetVertexShader(wstring file);
	void SetPixelShader(wstring file);

	void SetShader(wstring file);

	void SetMaterial();

	void SetDiffuseMap (Texture* texture);
	void SetDiffuseMap (wstring file);
	void SetSpecularMap(wstring file);
	void SetNormalMap  (wstring file);

	MaterialBuffer* GetBuffer() { return buffer; }

	void Debug();

	void SelectMap();

	void Save(wstring file);
	void Load(wstring file);

	void SaveDialog();
	void LoadDialog();

	void SetLabel(string label) { this->label = label; }

	string GetLabel() { return label; }

	Texture* GetDiffuseMap() { return diffuseMap; }

private:
    VertexShader* vertexShader = nullptr;
     PixelShader*  pixelShader = nullptr;

	 Texture*  diffuseMap = nullptr;
	 Texture* specularMap = nullptr;
	 Texture*   normalMap = nullptr;

	 MaterialBuffer* buffer = nullptr;

	 string projectDir;
	 
	 static int ID;

	 string label = "";
};
