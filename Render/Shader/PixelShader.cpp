#include "Framework.h"
#include "PixelShader.h"

PixelShader::PixelShader(wstring file)
{
    DWORD flags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;

    D3DCompileFromFile
    (
        file.c_str(),
        nullptr,
        D3D_COMPILE_STANDARD_FILE_INCLUDE,
        "main",
        "ps_5_0",
        flags,
        0,
        &blob,
        nullptr
    );

    DEVICE->CreatePixelShader
    (
        blob->GetBufferPointer(),
        blob->GetBufferSize(),
        nullptr,
        &pixelShader
    );
}

PixelShader::~PixelShader()
{
    pixelShader->Release();
}

void PixelShader::SetShader()
{
    DC->PSSetShader(pixelShader, nullptr, 0);
}
