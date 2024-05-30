#pragma once

class ConstBuffer
{
public:
	ConstBuffer(void* data, UINT dataSize);
	virtual ~ConstBuffer();

	void SetVSBuffer(UINT slot);
	void SetPSBuffer(UINT slot);
	void SetCSBuffer(UINT slot);

	void UpdateSubResource();

private:
	void* data;
	UINT  dataSize;

	ID3D11Buffer* constBuffer;

	D3D11_MAPPED_SUBRESOURCE subResource;
};
