#include "Framework.h"
#include "RawBuffer.h"

RawBuffer::RawBuffer(void* inputData, UINT byteWidth)
	:inputData(inputData), byteWidth(byteWidth)
{
	CreateOutput();
	CreateUAV();
	CreateResult();
}

RawBuffer::~RawBuffer()
{
	result->Release();
	   uav->Release();
	output->Release();
}

void RawBuffer::Copy(void* data, UINT size)
{
	DC->CopyResource(result, output);

	D3D11_MAPPED_SUBRESOURCE subResource;

	DC->Map(result, 0, D3D11_MAP_READ, 0, &subResource);

	memcpy(data, subResource.pData, size);

	DC->Unmap(result, 0);
}

void RawBuffer::CreateOutput()
{
	ID3D11Buffer* buffer;

	D3D11_BUFFER_DESC desc = {};

	desc.Usage     = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = byteWidth;
	desc.BindFlags = D3D11_BIND_UNORDERED_ACCESS;
	desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS;

	DEVICE->CreateBuffer(&desc, nullptr, &buffer);

	output = (ID3D11Resource*)buffer;
}

void RawBuffer::CreateUAV()
{
	ID3D11Buffer* buffer = (ID3D11Buffer*)output;

	D3D11_BUFFER_DESC desc = {};
	buffer->GetDesc(&desc);

	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
	uavDesc.Format              = DXGI_FORMAT_R32_TYPELESS;
	uavDesc.ViewDimension       = D3D11_UAV_DIMENSION_BUFFER;
	uavDesc.Buffer.FirstElement = 0;
	uavDesc.Buffer.Flags        = D3D11_BUFFER_UAV_FLAG_RAW;
	uavDesc.Buffer.NumElements  = desc.ByteWidth / 4;

	DEVICE->CreateUnorderedAccessView(buffer, &uavDesc, &uav);
}

void RawBuffer::CreateResult()
{
	ID3D11Buffer* buffer;

	D3D11_BUFFER_DESC desc = {};
	((ID3D11Buffer*)output)->GetDesc(&desc);

	desc.Usage          = D3D11_USAGE_STAGING;
	desc.BindFlags      = 0;
	desc.MiscFlags      = 0;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;

	DEVICE->CreateBuffer(&desc, nullptr, &buffer);

	result = (ID3D11Resource*)buffer;
}
