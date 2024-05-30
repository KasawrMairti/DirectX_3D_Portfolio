#include "Framework.h"
#include "ModelInstancing.h"


ModelInstancing::ModelInstancing(string name)
	:Model(name), name(name)
{
	instanceDatas.resize(MAX_INSTANCE);

	instanceBuffer = new VertexBuffer(instanceDatas);

	reader->SetShader(L"07ModelInstancing");
}

ModelInstancing::~ModelInstancing()
{
	delete instanceBuffer;
	
	for (Transform* transform : transforms)
		delete transform;
	transforms.clear();
}

void ModelInstancing::Update()
{
	drawCount = 0;

	for (UINT i = 0; i < transforms.size(); i++)
	{
		if (!transforms[i]->IsActive()) continue;

		transforms[i]->UpdateWorld();

		instanceDatas[drawCount].world = XMMatrixTranspose(transforms[i]->GetWorld());
		instanceDatas[drawCount].index = i;

		drawCount++;
	}

	instanceBuffer->UpdateVertex(instanceDatas.data(), drawCount);
}

void ModelInstancing::Render()
{
	instanceBuffer->IASetBuffer(1);

	Model::RenderInstanced(drawCount);
}

void ModelInstancing::Debug()
{
	ImGui::Text("DrawCount : %d", drawCount);

	for (Transform* transform : transforms)
		transform->Debug();
}

Transform* ModelInstancing::Add()
{
	Transform* transform = new Transform();

	transform->SetLabel(name + "_" + to_string(transforms.size()));
	transforms.push_back(transform);

	return transform;
}

