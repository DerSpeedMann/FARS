#pragma once

#include "stb_image.h"
#include "imgui_impl_dx11.h"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/core/directx.hpp"
#include "opencv2/imgproc.hpp"
#include <d3d11.h>

class ImageVisualizer
{
public:
    static void RenderPoI(ImVec2 position, ImU32 color, float radius = 0, float thickness = 0);
    static ImVec2 CalculateResolution(int originalWidth, int originalHeight, int maxWidth, int maxHeight);
    static cv::Mat LoadImageMatrixFromFile(const char* filename, int flags, int* out_width, int* out_height);
    static void DX11_Init(ID3D11Device* device);
    static bool LoadTextureFromFile(const char* filename, ID3D11ShaderResourceView** out_srv, int* out_width, int* out_height);
    static bool SaveImageToFile(const char* filename, cv::Mat img);
};

