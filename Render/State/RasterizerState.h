#pragma once

class RasterizerState
{
public:
	RasterizerState();
	~RasterizerState();

	void SetState();
	
	void ChangeState(D3D11_FILL_MODE mode);

	void FrontCounterClockWise(bool value);

private:
	ID3D11RasterizerState* rasterizerState = nullptr;

	D3D11_RASTERIZER_DESC desc = {};
};
