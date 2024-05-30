#include "Framework.h"
#include "Model.h"

Model::Model(string name, wstring shaderfile)
{
	reader = new ModelReader(name);
	reader->SetShader(shaderfile);
}

Model::~Model()
{
	delete reader;
}

void Model::Update()
{
	Transform::Update();
}

void Model::Render()
{
	Transform::SetWorld();

	reader->Render();
}

void Model::RenderInstanced(UINT instanceCount)
{
	Transform::SetWorld();

	reader->RenderInstanced(instanceCount);
}
