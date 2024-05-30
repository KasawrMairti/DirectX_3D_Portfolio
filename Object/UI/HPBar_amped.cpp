#include "Framework.h"
#include "HPBar_amped.h"


HPBar_amped::HPBar_amped(wstring file)
	:Quad(file)
{
	material->SetShader(L"Texture");

	hpBar = new ProgressBar(L"UI/hp_bar.png", L"UI/hp_bar_BG.png");
	hpBar->SetLabel("HP Bar");
	hpBar->scale.x *= 0.47f;
	hpBar->scale.y *= 0.8f;
	hpBar->translation.x += 25.0f;
	hpBar->translation.y += 1.5f;
	hpBar->SetParent(this);
}

HPBar_amped::~HPBar_amped()
{
	delete hpBar;
}

void HPBar_amped::Update()
{
	Quad::Update();
	hpBar->Update();
}

void HPBar_amped::Render()
{
	Quad::Render();
	hpBar->Render();
}

void HPBar_amped::Debug()
{
	static float value = 1.0f;
	hpBar->SetValue(value);
	ImGui::SliderFloat("HP", &value, 0.0f, 1.0f);
}

void HPBar_amped::SetValue(float value)
{
	hpBar->SetValue(value);
}
