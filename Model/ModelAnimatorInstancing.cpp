#include "Framework.h"
#include "ModelAnimatorInstancing.h"



ModelAnimatorInstancing::ModelAnimatorInstancing(string name)
	:ModelAnimator(name), name(name)
{
	instanceDatas.resize(MAX_INSTANCE);

	instanceBuffer = new VertexBuffer(instanceDatas);

	reader->SetShader(L"08ModelAnimatorInstancing");

	frameInstancingBuffer = new FrameInstancingBuffer();
}

ModelAnimatorInstancing::~ModelAnimatorInstancing()
{
	delete instanceBuffer;

	for (Transform* transform : transforms)
		delete transform;
	transforms.clear();

	delete frameInstancingBuffer;
}

void ModelAnimatorInstancing::Update()
{
	UpdateTrnasoforms();
}

void ModelAnimatorInstancing::Render()
{
	if (texture == nullptr)
		CreateTexture();

	instanceBuffer->IASetBuffer(1);
	frameInstancingBuffer->SetVSBuffer(4);

	ModelAnimator::RenderInstanced(drawCount);
}

void ModelAnimatorInstancing::Debug()
{
	ImGui::Text("DrawCount : %d", drawCount);

	for (Transform* transform : transforms)
		transform->Debug();
}

Transform* ModelAnimatorInstancing::Add()
{
	Transform* transform = new Transform();

	transform->SetLabel(name + "_" + to_string(transforms.size()));
	transforms.push_back(transform);

	return transform;
}

void ModelAnimatorInstancing::PlayClip(UINT instanceIndex, int clip, float speed, float takeTime)
{																  
	frameInstancingBuffer->data.motion[instanceIndex].next.clip   = clip;
	frameInstancingBuffer->data.motion[instanceIndex].next.speed  = speed;
	frameInstancingBuffer->data.motion[instanceIndex].takeTime    = takeTime;
	frameInstancingBuffer->data.motion[instanceIndex].runningTime = 0.0f;

	clips[clip]->Init();
}

Matrix ModelAnimatorInstancing::GetTransformByNode(UINT instanceIndex, int nodeIndex)
{
	FrameInstancingBuffer::Frame& curClip = frameInstancingBuffer->data.motion[instanceIndex].cur;
	Matrix cur = nodeTransform[curClip.clip].transform[curClip.curFrame + 0][nodeIndex];
	Matrix next = nodeTransform[curClip.clip].transform[curClip.curFrame + 1][nodeIndex];
	Matrix curAnim = LERP(cur, next, curClip.time);

	FrameInstancingBuffer::Frame& nextClip = frameInstancingBuffer->data.motion[instanceIndex].next;
	if (nextClip.clip == -1) return curAnim;
	cur = nodeTransform[nextClip.clip].transform[nextClip.curFrame + 0][nodeIndex];
	next = nodeTransform[nextClip.clip].transform[nextClip.curFrame + 1][nodeIndex];
	Matrix nextAnim = LERP(cur, next, nextClip.time);

	return LERP(curAnim, nextAnim, frameInstancingBuffer->data.motion[instanceIndex].tweenTime);
}

void ModelAnimatorInstancing::UpdateFrame(UINT instanceIndex)
{
	FrameInstancingBuffer::Motion& frameData = frameInstancingBuffer->data.motion[instanceIndex];

	ModelClip* clip = clips[frameData.cur.clip];

	frameData.cur.time += Time::Delta() * clip->ticksPerSecond * frameData.cur.speed;

	if (frameData.cur.time >= 1.0f)
	{
		++frameData.cur.curFrame %= (clip->frameCount - 1);
		frameData.cur.time = 0.0f;

		if (frameData.cur.curFrame == 0)
			clip->Init();

		float animRatio = (float)frameData.cur.curFrame / clips[frameData.cur.clip]->frameCount;

		clip->Execute(animRatio);
	}

	///////Next Clip
	if (frameData.next.clip < 0) return;

	frameData.tweenTime += Time::Delta() / frameData.takeTime;

	clip = clips[frameData.next.clip];

	if (frameData.tweenTime >= 1.0f)
	{
		frameData.cur = frameData.next;
		frameData.tweenTime = 0.0f;

		frameData.next.clip = -1;
		frameData.next.curFrame = 0;
		frameData.next.time = 0.0f;
	}
	else
	{
		frameData.next.time += Time::Delta() * clip->ticksPerSecond * frameData.next.speed;

		if (frameData.next.time >= 1.0f)
		{
			++frameData.next.curFrame %= (clip->frameCount - 1);
			frameData.next.time = 0.0f;
		}
	}
}

void ModelAnimatorInstancing::UpdateTrnasoforms()
{
	drawCount = 0;

	for (UINT i = 0; i < transforms.size(); i++)
	{
		if (!transforms[i]->IsActive()) continue;

		UpdateFrame(i);

		transforms[i]->UpdateWorld();

		instanceDatas[drawCount].world = XMMatrixTranspose(transforms[i]->GetWorld());
		instanceDatas[drawCount].index = i;

		drawCount++;
	}

	instanceBuffer->UpdateVertex(instanceDatas.data(), drawCount);
}
