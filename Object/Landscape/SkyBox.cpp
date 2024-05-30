#include "Framework.h"
#include "SkyBox.h"


SkyBox::SkyBox(wstring file)
{
	sphere = new Sphere();
	sphere->GetMaterial()->SetShader(L"05SkyBox");

	rs = new RasterizerState();
	rs->FrontCounterClockWise(true);

	cubeMap = Texture::Get(file);
}

SkyBox::~SkyBox()
{
	delete sphere;
	delete rs;
}

void SkyBox::Render()
{
	cubeMap->PSSetShaderResources(10);

	rs->SetState();
	StateManager::GetInstance()->DepthDisable();

	sphere->Render();

	RS->SetState();
	StateManager::GetInstance()->DepthEnable();
}
