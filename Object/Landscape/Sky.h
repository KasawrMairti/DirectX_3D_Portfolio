#pragma once

class Sky
{
public:
	Sky();
	~Sky();

	void Render();
	void PostRender();

private:
	Sphere* sphere;

	RasterizerState* rs;

	SkyBuffer* buffer;
};