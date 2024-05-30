#pragma once

struct Vertex
{
    Vertex()
    {
    }

    Vertex(Vector3 vector)
    {
        pos = vector;
    }
    

    Vertex(float x, float y, float z)
    {
        pos = XMFLOAT3(x, y, z);
    }

    XMFLOAT3 pos = {};
};

struct VertexColor
{
    VertexColor()
    {
    }

    VertexColor(XMFLOAT3 pos, XMFLOAT4 color)
        :pos(pos), color(color)
    {
    }

    VertexColor(float x, float y, float z, float r, float g, float b, float a)
    {
        pos = { x, y, z };
        color = { r, g, b, a };
    }

    XMFLOAT3 pos   = {};
    XMFLOAT4 color = {};
};

struct VertexTexture
{
    VertexTexture()
    {
    }

    VertexTexture(Vector3 pos, Vector2 uv)
        :pos(pos), uv(uv)
    {
    }

    Vector3 pos = {};
    Vector2 uv  = {};
};

struct VertexTextureNormal
{
    VertexTextureNormal()
    {
    }

    VertexTextureNormal(Vector3 pos, Vector2 uv, Vector3 normal)
        :pos(pos), uv(uv), normal(normal)
    {
    }

    Vector3 pos    = {};
    Vector2 uv     = {};
    Vector3 normal = {};
};

struct VertexColorNormal
{
    VertexColorNormal()
    {
    }

    VertexColorNormal(Vector3 pos, Vector4 color, Vector3 normal)
        :pos(pos), color(color), normal(normal)
    {
    }

    Vector3 pos    = {};
    Vector4 color  = {};
    Vector3 normal = {};
};

struct VertexTextureNormalTangent
{
    VertexTextureNormalTangent()
    {
    }

    VertexTextureNormalTangent(Vector3 pos, Vector2 uv, Vector3 normal, Vector3 tangent)
        :pos(pos), uv(uv), normal(normal), tangent(tangent)
    {
    }

    Vector3 pos     = {};
    Vector2 uv      = {};
    Vector3 normal  = {};
    Vector3 tangent = {};
};

struct VertexTextureNormalTangentAlpha
{
    VertexTextureNormalTangentAlpha()
    {
    }

    Vector3 pos      = {};
    Vector2 uv       = {};
    Vector3 normal   = {};
    Vector3 tangent  = {};
    float   alpha[4] = {};
};

struct VertexTextureNormalTangentBlend
{
    VertexTextureNormalTangentBlend()
    {
    }

    Vector3 pos     = {};
    Vector2 uv      = {};
    Vector3 normal  = {};
    Vector3 tangent = {};
    Vector4 indices = {};
    Vector4 weights = {};
};

struct InstanceData
{
    Matrix world = XMMatrixIdentity();
    UINT   index = 0;
};