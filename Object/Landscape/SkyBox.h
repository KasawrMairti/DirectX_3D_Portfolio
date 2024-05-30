#pragma once

class SkyBox
{
public:
	SkyBox(wstring file);
	~SkyBox();

	void Render();

private:
	Sphere* sphere;

	RasterizerState* rs;

	Texture* cubeMap;
};