#pragma once

class Cube : public Transform
{
    typedef VertexColorNormal VertexType;
public:
	Cube(Vector4 color);
	~Cube();

    void Update();
    void Render();

    void CreateMesh(Vector4 color);
    void CreateNormal();

private:
    Material* material;
    Mesh*     mesh;

    vector<VertexType> vertices;
    vector<UINT>        indices;

    static int count;
};
