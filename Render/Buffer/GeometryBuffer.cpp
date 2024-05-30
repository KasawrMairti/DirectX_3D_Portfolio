#include "Framework.h"
#include "GeometryBuffer.h"


GeometryBuffer::GeometryBuffer()
{
	for (UINT i = 0; i < SIZE; i++)
	{
		rtvs[i] = new RenderTarget();
		srvs[i] = rtvs[i]->GetSRV();
	}

	depthStencil = new DepthStencil(WIN_WIDTH, WIN_HEIGHT, true);

	srvs[SIZE] = depthStencil->GetSRV();

	for (UINT i = 0; i < SIZE + 1; i++)
	{
		quads[i] = new Quad(Vector2(200, 200));
		quads[i]->translation = Vector3(100 + i * 200, 100, 0);

		Texture* texture = Texture::Get(L"G" + to_wstring(i), srvs[i]);
		quads[i]->GetMaterial()->SetDiffuseMap(texture);
		quads[i]->UpdateWorld();
	}
}

GeometryBuffer::~GeometryBuffer()
{
	delete depthStencil;
	
	for (UINT i = 0; i < SIZE; i++)
	{
		delete rtvs[i];
	}

	for (Quad* quad : quads)
		delete quad;
}

void GeometryBuffer::SetMultiRenderTarget()
{
	RenderTarget::SetMulti(rtvs, SIZE, depthStencil, Vector4(1, 1, 0, 1));
}

void GeometryBuffer::PostRender()
{
	for (Quad* quad : quads)
		quad->Render();
}

void GeometryBuffer::SetSRVs()
{
	for (UINT i = 0; i < SIZE + 1; i++)
	{
		DC->PSSetShaderResources(10 + i, 1, &srvs[i]);
	}
}
