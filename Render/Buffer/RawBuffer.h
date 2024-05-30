#pragma once

class RawBuffer
{
public:
	RawBuffer(void* inputData, UINT byteWidth);
	~RawBuffer();

	void Copy(void* data, UINT size);

	ID3D11UnorderedAccessView* GetUAV() { return uav; }
private:
	void CreateOutput();
	void CreateUAV();
	void CreateResult();

private:
	ID3D11UnorderedAccessView* uav;

	ID3D11Resource* output;
	ID3D11Resource* result;

	void* inputData;
	UINT  byteWidth;
};
