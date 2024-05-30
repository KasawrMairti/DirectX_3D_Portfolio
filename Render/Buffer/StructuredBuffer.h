#pragma once

class StructuredBuffer
{
public:
	StructuredBuffer(void* inputData, UINT inputStride, UINT inputCount, UINT outputStride, UINT outputCount);
	~StructuredBuffer();

	void Copy(void* data, UINT size);
	void UpdateInput(void* data);

	ID3D11UnorderedAccessView* GetUAV() { return uav; }
	ID3D11ShaderResourceView*  GetSRV() { return srv;}

	void SetSRV();
	void SetUAV();

private:
	void CreateInput();
	void CreateSRV();
	void CreateOutput();
	void CreateUAV();
	void CreateResult();

private:
	ID3D11UnorderedAccessView* uav;
	ID3D11ShaderResourceView*  srv;

	ID3D11Resource*  input;
	ID3D11Resource* output;
	ID3D11Resource* result;

	void* inputData;
	UINT  inputStride;
	UINT  inputCount;

	UINT  outputStride;
	UINT  outputCount;
};
