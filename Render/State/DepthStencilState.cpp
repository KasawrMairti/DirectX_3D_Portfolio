#include "Framework.h"
#include "DepthStencilState.h"


DepthStencilState::DepthStencilState()
{
	desc.DepthEnable		= true;	
	desc.DepthWriteMask		= D3D11_DEPTH_WRITE_MASK_ALL;	
	desc.DepthFunc			= D3D11_COMPARISON_LESS;

	desc.StencilEnable		= true;	
	desc.StencilReadMask	= 0xFF;
	desc.StencilWriteMask	= 0xFF;

	desc.FrontFace.StencilFailOp      = D3D11_STENCIL_OP_KEEP;
	desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	desc.FrontFace.StencilPassOp	  = D3D11_STENCIL_OP_KEEP;
	desc.FrontFace.StencilFunc		  = D3D11_COMPARISON_ALWAYS;

	desc.BackFace.StencilFailOp		  = D3D11_STENCIL_OP_KEEP;
	desc.BackFace.StencilDepthFailOp  = D3D11_STENCIL_OP_DECR;
	desc.BackFace.StencilPassOp		  = D3D11_STENCIL_OP_KEEP;
	desc.BackFace.StencilFunc		  = D3D11_COMPARISON_ALWAYS;

	ChangeState();
}

DepthStencilState::~DepthStencilState()
{
	state->Release();
}

void DepthStencilState::SetState(UINT stencilRef)
{
	DC->OMSetDepthStencilState(state, stencilRef);
}

void DepthStencilState::ChangeState()
{
	if (state != nullptr)
		state->Release();

	DEVICE->CreateDepthStencilState(&desc, &state);
}

void DepthStencilState::DepthEnable(bool value)
{
	desc.DepthEnable = value;

	ChangeState();
}

void DepthStencilState::DepthWriteMask(D3D11_DEPTH_WRITE_MASK value)
{
	desc.DepthWriteMask = value;

	ChangeState();
}
