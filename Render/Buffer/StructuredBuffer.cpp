#include "Framework.h"
#include "StructuredBuffer.h"

StructuredBuffer::StructuredBuffer(void* inputData, UINT inputStride, UINT inputCount, UINT outputStride, UINT outputCount)
	: inputData(inputData),	inputStride(inputStride), inputCount(inputCount), outputStride(outputStride), outputCount(outputCount)
{
	CreateInput();
	CreateSRV();
	CreateOutput();
	CreateUAV();
	CreateResult();
}

StructuredBuffer::~StructuredBuffer()
{
	result->Release();
	   uav->Release();
	output->Release();

	   srv->Release();
	 input->Release();
}

void StructuredBuffer::Copy(void* data, UINT size)
{
	DC->CopyResource(result, output);

	D3D11_MAPPED_SUBRESOURCE subResource;

	DC->Map(result, 0, D3D11_MAP_READ, 0, &subResource);

	memcpy(data, subResource.pData, size);

	DC->Unmap(result, 0);
}

void StructuredBuffer::UpdateInput(void* data)
{
	DC->UpdateSubresource(input, 0, nullptr, data, inputStride, inputCount);

	srv->Release();

	CreateSRV();
}

void StructuredBuffer::SetSRV()
{
	DC->CSSetShaderResources(0, 1, &srv);
}

void StructuredBuffer::SetUAV()
{
	DC->CSSetUnorderedAccessViews(0, 1, &uav, nullptr);
}

void StructuredBuffer::CreateInput()
{
	ID3D11Buffer* buffer;

	D3D11_BUFFER_DESC desc = {};

	desc.Usage     = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = inputStride * inputCount;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;

	desc.StructureByteStride = inputStride;

	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = inputData;

	DEVICE->CreateBuffer(&desc, &initData, &buffer);

	input = (ID3D11Resource*)buffer;
}

void StructuredBuffer::CreateSRV()
{
	ID3D11Buffer* buffer = (ID3D11Buffer*)input;

	D3D11_BUFFER_DESC desc = {};
	buffer->GetDesc(&desc);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format              = DXGI_FORMAT_UNKNOWN;
	srvDesc.ViewDimension       = D3D11_SRV_DIMENSION_BUFFER;
	srvDesc.Buffer.FirstElement = 0;
	srvDesc.Buffer.NumElements  = inputCount;

	DEVICE->CreateShaderResourceView(buffer, &srvDesc, &srv);
}

void StructuredBuffer::CreateOutput()
{
	ID3D11Buffer* buffer;

	D3D11_BUFFER_DESC desc = {};

	desc.Usage     = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = outputStride * outputCount;
	desc.BindFlags = D3D11_BIND_UNORDERED_ACCESS;
	desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	desc.StructureByteStride = outputStride;

	DEVICE->CreateBuffer(&desc, nullptr, &buffer);

	output = (ID3D11Resource*)buffer;
}

void StructuredBuffer::CreateUAV()
{
	ID3D11Buffer* buffer = (ID3D11Buffer*)output;

	D3D11_BUFFER_DESC desc = {};
	buffer->GetDesc(&desc);

	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
	uavDesc.Format              = DXGI_FORMAT_UNKNOWN;
	uavDesc.ViewDimension       = D3D11_UAV_DIMENSION_BUFFER;
	uavDesc.Buffer.FirstElement = 0;
	uavDesc.Buffer.Flags        = 0;
	uavDesc.Buffer.NumElements  = outputCount;

	DEVICE->CreateUnorderedAccessView(buffer, &uavDesc, &uav);
}

void StructuredBuffer::CreateResult()
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
