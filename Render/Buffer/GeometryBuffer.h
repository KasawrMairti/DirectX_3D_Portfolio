#pragma once

class GeometryBuffer
{
	enum RenderTargets	{ DIFFUSE, NORMAL, SPECULAR, SIZE };

public:
	GeometryBuffer();
	~GeometryBuffer();

	void SetMultiRenderTarget();
	void PostRender();
	void SetSRVs();

private:
	DepthStencil* depthStencil;

	RenderTarget* rtvs[SIZE];

	class Quad* quads[SIZE + 1];
	ID3D11ShaderResourceView* srvs[SIZE + 1];
};