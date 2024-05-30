#include "Framework.h"
#include "ModelAnimator.h"


ModelAnimator::ModelAnimator(string name, wstring shaderFile)
	:name(name)
{
	reader = new ModelReader(name);
	reader->SetShader(shaderFile);

	frameBuffer = new FrameBuffer();
}

ModelAnimator::~ModelAnimator()
{
	delete reader;

	delete frameBuffer;
}

void ModelAnimator::Update()
{
	Transform::Update();

	if (!isPlay) return;

	UpdateFrame();
}

void ModelAnimator::Render()
{
	Transform::SetWorld(true);

	frameBuffer->SetVSBuffer(3);

	DC->VSSetShaderResources(0, 1, &srv);

	reader->Render();
}

void ModelAnimator::RenderInstanced(UINT instanceCount)
{
	Transform::SetWorld(true);

	frameBuffer->SetVSBuffer(3);

	DC->VSSetShaderResources(0, 1, &srv);

	reader->RenderInstanced(instanceCount);
}

void ModelAnimator::ReadClip(string file, UINT clipIndex)
{
	string path = "_ModelData/Clip/" + name + "/" + file + to_string(clipIndex) + ".clip";

	BinaryReader data(path);

	ModelClip* clip = new ModelClip();
	clip->name				= data.ReadString();
	clip->ticksPerSecond	= data.ReadFloat();
	clip->frameCount		= data.ReadUINT();
	clip->duration			= data.ReadFloat();

	UINT keyframeCount = data.ReadUINT();
	for (UINT i = 0; i < keyframeCount; i++)
	{
		KeyFrame* keyFrame = new KeyFrame();
		keyFrame->boneName = data.ReadString();

		UINT keytTransformCount = data.ReadUINT();
		if (keytTransformCount > 0)
		{
			keyFrame->transforms.resize(keytTransformCount);

			void* ptr = (void*)keyFrame->transforms.data();
			data.ReadData(&ptr, sizeof(KeyTransform) * keytTransformCount);
		}

		clip->keyFrames.emplace(keyFrame->boneName, keyFrame);
	}

	clips.emplace_back(clip);
}

void ModelAnimator::PlayClip(UINT clipIndex, float speed, float takeTime)
{
	frameBuffer->data.next.clip    = clipIndex;
	frameBuffer->data.next.speed   = speed;
	frameBuffer->data.takeTime     = takeTime;
	frameBuffer->data.runningTime  = 0.0f;

	clips[clipIndex]->Init();
}

void ModelAnimator::CreateTexture()
{
	UINT clipCount = clips.size();

	clipTransform = new ClipTransform[clipCount];
	nodeTransform = new ClipTransform[clipCount];


	for (UINT i = 0; i < clipCount; i++)
	{
		CreateClipTransform(i);
	}

	D3D11_TEXTURE2D_DESC desc = {};
	desc.Width			  = MAX_BONE * 4;
	desc.Height			  = MAX_FRAME_KEY;
	desc.ArraySize		  = clipCount;
	desc.Format			  = DXGI_FORMAT_R32G32B32A32_FLOAT;
	desc.Usage			  = D3D11_USAGE_IMMUTABLE;
	desc.BindFlags		  = D3D11_BIND_SHADER_RESOURCE;
	desc.MipLevels		  = 1;
	desc.SampleDesc.Count = 1;

	UINT pageSize = MAX_BONE * sizeof(Matrix) * MAX_FRAME_KEY;

	void* ptr = VirtualAlloc(nullptr, pageSize * clipCount, MEM_RESERVE, PAGE_READWRITE);

	for (UINT c = 0; c < clipCount; c++)
	{
		UINT start = c * pageSize;

		for (UINT i = 0; i < MAX_FRAME_KEY; i++)
		{
			void* temp = (BYTE*)ptr + MAX_BONE * i * sizeof(Matrix) + start;

			VirtualAlloc(temp, MAX_BONE * sizeof(Matrix), MEM_COMMIT, PAGE_READWRITE);
			memcpy(temp, clipTransform[c].transform[i], MAX_BONE * sizeof(Matrix));
		}
	}

	D3D11_SUBRESOURCE_DATA* subResource = new D3D11_SUBRESOURCE_DATA[clipCount];

	for (UINT c = 0; c < clipCount; c++)
	{
		void* temp = (BYTE*)ptr + c * pageSize;

		subResource[c].pSysMem			= temp;
		subResource[c].SysMemPitch		= MAX_BONE * sizeof(Matrix);
		subResource[c].SysMemSlicePitch = pageSize;
	}

	DEVICE->CreateTexture2D(&desc, subResource, &texture);

	delete[] subResource;
	VirtualFree(ptr, 0, MEM_RELEASE);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format					 = DXGI_FORMAT_R32G32B32A32_FLOAT;
	srvDesc.ViewDimension			 = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	srvDesc.Texture2DArray.MipLevels = 1;
	srvDesc.Texture2DArray.ArraySize = clipCount;

	DEVICE->CreateShaderResourceView(texture, &srvDesc, &srv);
}

void ModelAnimator::UpdateFrame()
{
	FrameBuffer::Data& frameData = frameBuffer->data;

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

void ModelAnimator::Debug()
{
	reader->Debug();

	ImGui::Checkbox("IsPlay", &isPlay);
}

Matrix ModelAnimator::GetTransformByBone(UINT boneIndex)
{
	FrameBuffer::Frame& curClip = frameBuffer->data.cur;

	Matrix cur = clipTransform[curClip.clip].transform[curClip.curFrame][boneIndex];

	return cur;
}

Matrix ModelAnimator::GetTransformByNode(UINT nodeIndex)
{
	FrameBuffer::Frame& curClip = frameBuffer->data.cur;
	Matrix cur  = nodeTransform[curClip.clip].transform[curClip.curFrame + 0][nodeIndex];
	Matrix next = nodeTransform[curClip.clip].transform[curClip.curFrame + 1][nodeIndex];
	Matrix curAnim = LERP(cur, next, curClip.time);
	
	FrameBuffer::Frame& nextClip = frameBuffer->data.next;
	if (nextClip.clip == -1) return curAnim;
	cur  = nodeTransform[nextClip.clip].transform[nextClip.curFrame + 0][nodeIndex];
	next = nodeTransform[nextClip.clip].transform[nextClip.curFrame + 1][nodeIndex];
	Matrix nextAnim = LERP(cur, next, nextClip.time);

	return LERP(curAnim, nextAnim, frameBuffer->data.tweenTime);
}

void ModelAnimator::CreateClipTransform(UINT index)
{
	ModelClip* clip = clips[index];

	for (UINT f = 0; f < clip->frameCount; f++)
	{
		UINT nodeIndex = 0;

		for (NodeData node : reader->nodes)
		{
			Matrix animWorld;

			KeyFrame* frame = clip->GetKeyFrames(node.name);

			if (frame != nullptr)
			{
				KeyTransform transform = frame->transforms[f];

				Matrix S = XMMatrixScaling(transform.scale.x, transform.scale.y, transform.scale.z);
				Matrix R = XMMatrixRotationQuaternion(XMLoadFloat4(&transform.rotation));
				Matrix T = XMMatrixTranslation(transform.position.x, transform.position.y, transform.position.z);

				animWorld = S * R * T;
			}
			else
			{
				animWorld = XMMatrixIdentity();
			}

			Matrix parentWorld;

			int parentIndex = node.parent;

			if (parentIndex < 0)
				parentWorld = XMMatrixIdentity();
			else
				parentWorld = nodeTransform[index].transform[f][parentIndex];

			nodeTransform[index].transform[f][nodeIndex] = animWorld * parentWorld;

			vector<BoneData> bones = reader->bones;

			if (reader->HasBone(node.name))
			{
				UINT boneIndex = reader->boneMap[node.name];

				Matrix transform = bones[boneIndex].offset;

				transform *= nodeTransform[index].transform[f][nodeIndex];

				clipTransform[index].transform[f][boneIndex] = transform;
			}

			nodeIndex++;
		}
	}
}
