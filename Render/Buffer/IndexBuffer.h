#pragma once

class IndexBuffer
{
public:
	IndexBuffer(vector<UINT>& indices);
	~IndexBuffer();

	void IASetBuffer();

private:
	ID3D11Buffer* indexBuffer;
};
