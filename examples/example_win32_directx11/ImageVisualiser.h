#pragma once

#include "stb_image.h"
#include "imgui_impl_dx11.h"
#include <d3d11.h>

class ImageVisualiser
{
public:
    static ImVec2 CalculateResolution(int originalWidth, int originalHeight, int maxWidth, int maxHeight);
    static void DX11_Init(ID3D11Device* device);
    static bool LoadTextureFromFile(const char* filename, ID3D11ShaderResourceView** out_srv, int* out_width, int* out_height);
};

