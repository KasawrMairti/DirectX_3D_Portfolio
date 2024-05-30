#pragma once

class SamplerState
{
public:
	SamplerState();
	~SamplerState();

private:
	ID3D11SamplerState* samplerState = nullptr;
};
