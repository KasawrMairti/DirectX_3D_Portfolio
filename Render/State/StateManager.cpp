#include "Framework.h"
#include "StateManager.h"

StateManager::StateManager()
{
	CreateSamplerState();
	CreateRasterizerState();
	CreateBlendState();
	CreateDepthStencilState();
}

StateManager::~StateManager()
{
	delete    samplerState;
	delete rasterizerState;

	for (BlendState* blendState : blendStates)
		delete blendState;

	blendStates.clear();

	for (DepthStencilState* depthStencilState : depthStencilStates)
		delete depthStencilState;

	depthStencilStates.clear();
}

void StateManager::CreateSamplerState()
{
	samplerState = new SamplerState();
}

void StateManager::CreateRasterizerState()
{
	rasterizerState = new RasterizerState();
	rasterizerState->SetState();
}

void StateManager::CreateBlendState()
{
	blendStates.push_back(new BlendState());
	blendStates.push_back(new BlendState());
	blendStates.push_back(new BlendState());
	blendStates.push_back(new BlendState());

	blendStates[1]->Alpha(true);
	blendStates[2]->Additive();
	//blendStates[2]->AlphaToCoverage(true);
	blendStates[3]->AlphaToCoverage(true);
}

void StateManager::CreateDepthStencilState()
{
	depthStencilStates.push_back(new DepthStencilState());
	depthStencilStates.push_back(new DepthStencilState());
	depthStencilStates.push_back(new DepthStencilState());

	depthStencilStates[1]->DepthEnable(false);
	depthStencilStates[2]->DepthWriteMask(D3D11_DEPTH_WRITE_MASK_ZERO);
}

void StateManager::AlphaBegin()
{
	blendStates[1]->SetState();
}

void StateManager::AlphaEnd()
{
	blendStates[0]->SetState();
}

void StateManager::AlphaToCoverageEnable()
{
	blendStates[3]->SetState();
}

void StateManager::AdditiveBegin()
{
	blendStates[2]->SetState();
}

void StateManager::DepthEnable()
{
	depthStencilStates[0]->SetState();
}

void StateManager::DepthDisable()
{
	depthStencilStates[1]->SetState();
}

void StateManager::DepthWriteMaskZero()
{
	depthStencilStates[2]->SetState();
}

void StateManager::Set()
{
	AlphaEnd();
	DepthEnable();
}

void StateManager::PostSet()
{
	AlphaBegin();
	DepthDisable();
}
