#pragma once

class ModelAnimatorInstancing : public ModelAnimator
{
public:
	ModelAnimatorInstancing(string name);
	~ModelAnimatorInstancing();

	void Update();
	void Render();

	void Debug();

	Transform* Add();

	void PlayClip(UINT instanceIndex, int clip, float speed = 1.0f, float takeTime = 0.1f);

	Matrix GetTransformByNode(UINT instanceIndex, int nodeIndex);

private:
	void UpdateFrame(UINT instanceIndex);
	void UpdateTrnasoforms();

private:
	vector<Transform*> transforms;

	vector<InstanceData> instanceDatas;

	VertexBuffer* instanceBuffer;

	UINT drawCount = 0;

	string name;

	FrameInstancingBuffer* frameInstancingBuffer;
};