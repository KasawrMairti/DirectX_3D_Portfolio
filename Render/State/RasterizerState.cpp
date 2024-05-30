#include "Framework.h"
#include "RasterizerState.h"

RasterizerState::RasterizerState()
{
	desc.CullMode = D3D11_CULL_BACK;
	desc.FillMode = D3D11_FILL_SOLID;

	DEVICE->CreateRasterizerState(&desc, &rasterizerState);
}

RasterizerState::~RasterizerState()
{
	rasterizerState->Release();
}

void RasterizerState::SetState()
{
	DC->RSSetState(rasterizerState);
}

void RasterizerState::ChangeState(D3D11_FILL_MODE mode)
{
	if (desc.FillMode == mode)
		return;

	desc.FillMode = mode;

	if (rasterizerState != nullptr)
		rasterizerState->Release();

	DEVICE->CreateRasterizerState(&desc, &rasterizerState);
	DC->RSSetState(rasterizerState);
}

void RasterizerState::FrontCounterClockWise(bool value)
{
	desc.FrontCounterClockwise = value;

	if (rasterizerState != nullptr)
		rasterizerState->Release();

	DEVICE->CreateRasterizerState(&desc, &rasterizerState);
}
