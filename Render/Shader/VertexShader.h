#pragma once

class VertexShader : public Shader
{
	friend class Shader;
private:
	VertexShader(wstring file);
	~VertexShader();

	void CreateInputLayout();

public:
	virtual void SetShader() override;


private:
	ID3D11InputLayout*   inputLayout;

	ID3D11VertexShader* vertexShader;

	ID3D11ShaderReflection* reflection;
};
