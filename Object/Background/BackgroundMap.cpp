#include "Framework.h"
#include "BackgroundMap.h"


BackgroundMap::BackgroundMap()
	:Model("megaforce")
{	
	GAMEMANAGER->SetBackground(this);

	Initialize();

	rotation.y = XM_PI;
}

BackgroundMap::~BackgroundMap()
{
	for (Collider* collider : colliders)
		delete collider;
	colliders.clear();
}

void BackgroundMap::Update()
{
	Model::Update();

	for (Collider* collider : colliders)
		collider->Update();
}

void BackgroundMap::Render()
{
	Model::Render();

	for (Collider* collider : colliders)
		collider->Render();
}

void BackgroundMap::Debug()
{
	if (ImGui::TreeNode("Background Collider"))
	{
		if (ImGui::Button("Add"))
		{
			collider_count++;
			colliders.push_back(new ColliderBox());
		}
		if (ImGui::Button("Save"))
			save_Collider();
		if (ImGui::Button("Load"))
			load_Collider();
	
		for (int i = 0; i < collider_count; i++)
		{
			string label = "Collider_" + to_string(i);
	
			Collider* collider = colliders[i];
			if (ImGui::TreeNode(label.c_str()))
			{
				const char* list[] = { "FLOOR", "WALL", "CEILING" };
				ImGui::Combo("Type", (int*)&collider->WallTypes(), list, 3);

				ImGui::DragFloat3("TransForm", (float*)&collider->translation);
				ImGui::DragFloat3("Rotation", (float*)&collider->rotation);
				ImGui::DragFloat3("Scale", (float*)&collider->scale);
	
				if (ImGui::Button("Copy"))
				{
					collider_count++;
					Collider* colliderCopy = new ColliderBox();
					colliderCopy->WallTypes() = collider->WallTypes();
					if (colliderCopy->WallTypes() == Collider::WallType::FLOOR)	colliderCopy->SetColor(1, 0, 0);
					else if (colliderCopy->WallTypes() == Collider::WallType::WALL)	colliderCopy->SetColor(0, 1, 0);
					else if (colliderCopy->WallTypes() == Collider::WallType::CEILING)	colliderCopy->SetColor(0, 0, 1);
					colliderCopy->translation = collider->translation;
					colliderCopy->rotation = collider->rotation;
					colliderCopy->scale = collider->scale;
					colliderCopy->SetParent(this);


					colliders.push_back(colliderCopy);
				}
				if (ImGui::Button("Delete"))
				{
					delete collider;
					colliders.erase(remove(colliders.begin(), colliders.end(), colliders[i]), colliders.end());
					collider_count--;
				}
				ImGui::TreePop();
			}
		}
	
		ImGui::TreePop();
	}
}

void BackgroundMap::Initialize()
{
	reader->GetMaterial()[0]->Load(L"background01.mat");
	reader->GetMaterial()[1]->Load(L"background02.mat");
	reader->GetMaterial()[2]->Load(L"background03.mat");
	reader->GetMaterial()[3]->Load(L"background04.mat");
	reader->GetMaterial()[4]->Load(L"background05.mat");
	reader->GetMaterial()[5]->Load(L"background06.mat");
	reader->GetMaterial()[6]->Load(L"background07.mat");
	reader->GetMaterial()[7]->Load(L"background08.mat");
	reader->GetMaterial()[8]->Load(L"background09.mat");

	load_Collider();
}

void BackgroundMap::save_Collider()
{
	string name = "BackgroundMap";
	string path = "_ModelData/Collider/" + name + ".collider";
	CreateFolder(path);
	BinaryWriter data(ToWString(path));

	data.WriteData(collider_count);
	for (Collider* collider : colliders)
	{
		data.WriteData(collider->GetLabel());
		data.WriteData((int)collider->WallTypes());
		data.WriteData(collider->translation);
		data.WriteData(collider->rotation);
		data.WriteData(collider->scale);
		
		delete collider;
	}
	colliders.clear();
	collider_count = 0;
}

void BackgroundMap::load_Collider()
{
	for (Collider* collider : colliders)
		delete collider;
	colliders.clear();

	string name = "BackgroundMap";
	string path = "_ModelData/Collider/" + name + ".collider";
	BinaryReader data(ToWString(path));

	collider_count = data.ReadUINT();
	for (UINT i = 0; i < collider_count; i++)
	{
		Collider* collider = new ColliderBox();
		collider->SetLabel(data.ReadString());
		collider->WallTypes() = (Collider::WallType)data.ReadInt();
		collider->translation = data.ReadVector3();
		collider->rotation = data.ReadVector3();
		collider->scale = data.ReadVector3();
		collider->SetParent(this);

		if (collider->WallTypes() == Collider::WallType::FLOOR)
			collider->SetColor(1, 0, 0);
		else if (collider->WallTypes() == Collider::WallType::WALL)
			collider->SetColor(0, 1, 0);
		else if (collider->WallTypes() == Collider::WallType::CEILING)
			collider->SetColor(0, 0, 1);

		colliders.push_back(collider);
	}
}
