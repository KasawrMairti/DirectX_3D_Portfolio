#include "Framework.h"
#include "BlendState.h"


BlendState::BlendState()
{
	desc.AlphaToCoverageEnable = false;
	desc.IndependentBlendEnable = false;

	desc.RenderTarget[0].BlendEnable            = false;
    desc.RenderTarget[0].SrcBlend               = D3D11_BLEND_SRC_ALPHA;
    desc.RenderTarget[0].DestBlend              = D3D11_BLEND_INV_SRC_ALPHA;
    desc.RenderTarget[0].BlendOp                = D3D11_BLEND_OP_ADD;
    desc.RenderTarget[0].SrcBlendAlpha          = D3D11_BLEND_ONE;
    desc.RenderTarget[0].DestBlendAlpha         = D3D11_BLEND_INV_SRC_ALPHA;
    desc.RenderTarget[0].BlendOpAlpha           = D3D11_BLEND_OP_ADD;
    desc.RenderTarget[0].RenderTargetWriteMask  = D3D11_COLOR_WRITE_ENABLE_ALL;

    ChangeState();
}

BlendState::~BlendState()
{
    state->Release();
}

void BlendState::SetState()
{
    float blendFactor[] = { 0, 0, 0, 0 };
    DC->OMSetBlendState(state, blendFactor, 0XFFFFFFFF);
}

void BlendState::ChangeState()
{
    if (state != nullptr)
        state->Release();

    DEVICE->CreateBlendState(&desc, &state);
}

void BlendState::Alpha(bool value)
{
    desc.RenderTarget[0].BlendEnable = value;

    ChangeState();
}

void BlendState::AlphaToCoverage(bool value)
{
    desc.AlphaToCoverageEnable = value;

    ChangeState();
}

void BlendState::Additive()
{
    desc.RenderTarget[0].BlendEnable = true;
    desc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;

    ChangeState();
}
