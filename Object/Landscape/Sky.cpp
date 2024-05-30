#include "Framework.h"
#include "Sky.h"


Sky::Sky()
{
	sphere = new Sphere();
	sphere->GetMaterial()->SetShader(L"05Sky");

	buffer = new SkyBuffer();

	rs = new RasterizerState();
	rs->FrontCounterClockWise(true);
}

Sky::~Sky()
{
	delete sphere;
	delete buffer;
	delete rs;
}

void Sky::Render()
{
	buffer->SetPSBuffer(10);

	rs->SetState();
	StateManager::GetInstance()->DepthDisable();

	sphere->Render();

	RS->SetState();
	StateManager::GetInstance()->DepthEnable();
}

void Sky::PostRender()
{
	ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Sky Option");

	ImGui::ColorEdit3("CenterColor", (float*)&buffer->data.centerColor, ImGuiColorEditFlags_PickerHueBar);
	ImGui::ColorEdit3("Apex  Color", (float*)&buffer->data.apexColor  , ImGuiColorEditFlags_PickerHueWheel);
	ImGui::DragFloat("Height", &buffer->data.height);
}
