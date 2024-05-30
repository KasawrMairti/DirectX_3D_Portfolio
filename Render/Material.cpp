#include "Framework.h"
#include "Material.h"

int Material::ID = 0;

Material::Material()
{
	buffer = new MaterialBuffer();

	char path[128];
	GetCurrentDirectoryA(128, path);

	projectDir = path;

	projectDir += "/_Texture";

	ID += 1;

	label.resize(20);
	label = "Material" + to_string(ID);
}

Material::Material(wstring file)
{
	buffer = new MaterialBuffer();

	SetShader(file);

	char path[128];
	GetCurrentDirectoryA(128, path);

	projectDir = path;

	projectDir += "/_Texture";

	ID += 1;

	label.resize(20);
	label = "Material" + to_string(ID);
}

Material::~Material()
{
	delete buffer;
}

void Material::SetVertexShader(wstring file)
{
	vertexShader = Shader::GetVS(file);
}

void Material::SetPixelShader(wstring file)
{
	pixelShader = Shader::GetPS(file);
}

void Material::SetShader(wstring file)
{
	SetVertexShader(file);
	SetPixelShader(file);
}

void Material::SetMaterial()
{
	vertexShader->SetShader();
	 pixelShader->SetShader();

	 if (diffuseMap)
		 diffuseMap->PSSetShaderResources(0);

	 if (specularMap)
		 specularMap->PSSetShaderResources(1);
	 
	 if (normalMap)
		 normalMap->PSSetShaderResources(2);

	 buffer->SetPSBuffer(1);
}

void Material::SetDiffuseMap(Texture* texture)
{
	diffuseMap = texture;

	buffer->data.hasDiffuseMap = true;
}

void Material::SetDiffuseMap(wstring file)
{
	diffuseMap = Texture::Get(file);

	buffer->data.hasDiffuseMap = true;
}

void Material::SetSpecularMap(wstring file)
{
	specularMap = Texture::Get(file);

	buffer->data.hasSpecularMap = true;
}

void Material::SetNormalMap(wstring file)
{
	normalMap = Texture::Get(file);

	buffer->data.hasNormalMap = true;
}

void Material::Debug()
{
	//label.resize(100);
	char* str = (char*)label.data();

	ImGui::InputText("Label", str, 128);

	label = str;
	
	if (label[0] == '\0')
		label = "NULL";

	if (ImGui::BeginMenu(label.c_str()))
	{
		ImGui::ColorEdit4((label + " Diffuse").c_str(), (float*)&buffer->data.diffuse);
		ImGui::ColorEdit4((label + " Specular").c_str(), (float*)&buffer->data.specular);
		ImGui::ColorEdit4((label + " Ambient").c_str(), (float*)&buffer->data.ambient);
		ImGui::ColorEdit4((label + " Emissive").c_str(), (float*)&buffer->data.emissive);
		
		ImGui::Checkbox((label + " HasDiffuseMap").c_str(), (bool*)&buffer->data.hasDiffuseMap);
		ImGui::Checkbox((label + " HasSpecularMap").c_str(), (bool*)&buffer->data.hasSpecularMap);
		ImGui::Checkbox((label + " HasNormalMap").c_str(), (bool*)&buffer->data.hasNormalMap);

		ImGui::SliderFloat((label + " Shininess").c_str(), &buffer->data.shininess, 1.0f, 50.0f);

		SaveDialog();
		LoadDialog();

		ImGui::EndMenu();
	}

	SelectMap();
}

void Material::SelectMap()
{
	if (ImGui::BeginMenu(label.c_str()))
	{
		if (ImGui::Button("DiffuseMap"))
			Dialog->OpenDialog("Diffuse", "Select Diffuse", ".png,.jpg,.dds,.tga,.DDS,.TGA", "_Texture/");

		if (ImGui::Button("NormalMap"))
			Dialog->OpenDialog("Normal", "Select Normal", ".png,.jpg,.dds,.tga,.DDS,.TGA", "_Texture/");

		if (ImGui::Button("SpecularMap"))
			Dialog->OpenDialog("Specular", "Select Specular", ".png,.jpg,.dds,.tga,.DDS,.TGA", "_Texture/");

		if (Dialog->Display("Diffuse") || Dialog->Display("Specular") || Dialog->Display("Normal"))
		{
			if (Dialog->IsOk())
			{
				string path = Dialog->GetFilePathName();

				path = path.substr(projectDir.size() + 1, path.size());

				wstring file = ToWString(path);

				if (Dialog->GetOpenedKey() == "Diffuse")
					SetDiffuseMap(file);
				else if (Dialog->GetOpenedKey() == "Specular")
					SetSpecularMap(file);
				else if (Dialog->GetOpenedKey() == "Normal")
					SetNormalMap(file);
			}

			Dialog->Close();
		}

		ImGui::EndMenu();
	}
}

void Material::Save(wstring file)
{
	BinaryWriter data(file);

	data.WriteData(label);

	if (vertexShader)
		data.WriteData(vertexShader->GetPath());
	else
		data.WriteData(L"");

	if (pixelShader)
		data.WriteData(pixelShader->GetPath());
	else
		data.WriteData(L"");

	if (diffuseMap)
		data.WriteData(diffuseMap->GetPath());
	else
		data.WriteData(L"");

	if (specularMap)
		data.WriteData(specularMap->GetPath());
	else
		data.WriteData(L"");

	if (normalMap)
		data.WriteData(normalMap->GetPath());
	else
		data.WriteData(L"");

	data.WriteData(buffer->data.diffuse  );
	data.WriteData(buffer->data.specular );
	data.WriteData(buffer->data.ambient  );
	data.WriteData(buffer->data.emissive );
	data.WriteData(buffer->data.shininess);
}

void Material::Load(wstring file)
{
	BinaryReader data(file);

	if (!data.Succeeded())
		return;

	label = data.ReadString();

	wstring str;

	str = data.ReadWString();
	if (str != L"")
		vertexShader = Shader::GetVS(str);

	str = data.ReadWString();
	if (str != L"")
		pixelShader = Shader::GetPS(str);

	str = data.ReadWString();
	if (str != L"")
		SetDiffuseMap(str);

	str = data.ReadWString();
	if (str != L"")
		SetSpecularMap(str);

	str = data.ReadWString();
	if (str != L"")
		SetNormalMap(str);

	buffer->data.diffuse   = data.ReadVector4();
	buffer->data.specular  = data.ReadVector4();
	buffer->data.ambient   = data.ReadVector4();
	buffer->data.emissive  = data.ReadVector4();

	buffer->data.shininess = data.ReadFloat();
}

void Material::SaveDialog()
{
	if (ImGui::Button(("Save " + label).c_str()))
	{
		Dialog->OpenDialog("Save Material", "Save", ".mat", "_TextData/");
	}

	if (Dialog->Display("Save Material", 32, { 500, 400 }))
	{
		if (Dialog->IsOk())
		{
			string path = Dialog->GetFilePathName();

			path = path.substr(GetProjectDir().size(), path.length());

			Save(ToWString(path));
		}

		Dialog->Close();
	}
}

void Material::LoadDialog()
{
	if (ImGui::Button(("Load " + label).c_str()))
	{
		Dialog->OpenDialog("Load Material", "Load", ".mat", "_TextData/");
	}

	if (Dialog->Display("Load Material", 32, { 500, 400 }))
	{
		if (Dialog->IsOk())
		{
			string path = Dialog->GetFilePathName();

			path = path.substr(GetProjectDir().size(), path.length());

			Load(ToWString(path));
		}

		Dialog->Close();
	}
}
