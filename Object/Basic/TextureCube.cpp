#include "Framework.h"
#include "TextureCube.h"

TextureCube::TextureCube()
{
	CreateQuads();
}

TextureCube::~TextureCube()
{
	for (Quad* quad : quads)
		delete quad;

	quads.clear();
}

void TextureCube::Update()
{
	for (Quad* quad : quads)
		quad->Update();

	Transform::Update();
}

void TextureCube::Render()
{
	for (Quad* quad : quads)
		quad->Render();
}

void TextureCube::CreateQuads()
{
	quads.resize(6);

	quads[LEFT] = new Quad();
	quads[LEFT]->translation.x = -0.5f;
	quads[LEFT]->rotation.y = +XM_PIDIV2;
	quads[LEFT]->SetParent(this);

	quads[RIGHT] = new Quad();
	quads[RIGHT]->translation.x = +0.5f;
	quads[RIGHT]->rotation.y = -XM_PIDIV2;
	quads[RIGHT]->SetParent(this);

	quads[BOTTOM] = new Quad();
	quads[BOTTOM]->translation.y = -0.5f;
	quads[BOTTOM]->rotation.x = -XM_PIDIV2;
	quads[BOTTOM]->SetParent(this);

	quads[TOP] = new Quad();
	quads[TOP]->translation.y = +0.5f;
	quads[TOP]->rotation.x = +XM_PIDIV2;
	quads[TOP]->SetParent(this);


	quads[FRONT] = new Quad();
	quads[FRONT]->translation.z = -0.5f;
	//quads[FRONT]->rotation.y = +XM_PIDIV2;
	quads[FRONT]->SetParent(this);

	quads[BACK] = new Quad();
	quads[BACK]->translation.z = +0.5f;
	quads[BACK]->rotation.y = -XM_PI;
	quads[BACK]->SetParent(this);

}

void TextureCube::SetDiffuse(wstring file)
{
	for (Quad* quad : quads)
	{
		quad->GetMaterial()->SetDiffuseMap(file);
	}
}
