#define STB_IMAGE_IMPLEMENTATION
#include "ImageVisualizer.h"
#include <iostream>
#include <fstream>

// Other examples: https://github.com/ocornut/imgui/wiki/Image-Loading-and-Displaying-Examples

static ID3D11Device* g_pd3dDevice = NULL;
static cv::ocl::Context m_oclCtx;

void ImageVisualizer::DX11_Init(ID3D11Device* device)
{
    g_pd3dDevice = device;
    // initialize OpenCL context of OpenCV lib from DirectX
    if (cv::ocl::haveOpenCL())
    {
        m_oclCtx = cv::directx::ocl::initializeContextFromD3D11Device(g_pd3dDevice);
    }

}

ImVec2 ImageVisualizer::CalculateResolution(int originalWidth, int originalHeight, int maxWidth, int maxHeight)
{
    int aspectRatio = originalHeight / originalWidth;
    ImVec2 newSize = ImVec2(originalWidth, originalHeight);

    if (maxWidth < originalWidth)
    {
        newSize.x = maxWidth;
        newSize.y = aspectRatio * maxWidth;
    }
    if (maxHeight < originalHeight)
    {
        newSize.y = maxHeight;
        newSize.x = aspectRatio * maxHeight;
    }

    return newSize;
}

bool ImageVisualizer::SaveImageToFile(const char* filename, cv::Mat img) {
    return cv::imwrite(filename, img);
}

cv::Mat ImageVisualizer::LoadImageMatrixFromFile(const char* filename, int flags, int* out_width, int* out_height)
{
    cv::Mat src = cv::imread(filename, flags);
    *out_width = src.size().width;
    *out_height = src.size().height;
    return src;
}

bool ImageVisualizer::LoadTextureFromFile(const char* filename, ID3D11ShaderResourceView** out_srv, int* out_width, int* out_height)
{
    if (g_pd3dDevice == NULL)
    {
        std::cout << "DX11 device is not initialized!\n";
        return false;
    }

    cv::Mat src = LoadImageMatrixFromFile(filename, cv::IMREAD_COLOR | cv::IMREAD_ANYDEPTH, out_width, out_height);

    if (src.empty())
    {
        std::cout << "Image not found or invalid Image!\n";
        return false;
    }
    cv::Mat RGBAMat;
    cv::cvtColor(src, RGBAMat, cv::COLOR_BGR2BGRA);

    // Create texture
    D3D11_TEXTURE2D_DESC desc;
    ZeroMemory(&desc, sizeof(desc));
    desc.Width = *out_width;
    desc.Height = *out_height;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.SampleDesc.Count = 1;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    desc.CPUAccessFlags = 0;

    ID3D11Texture2D* pTexture = NULL;

    HRESULT r = g_pd3dDevice->CreateTexture2D(&desc, 0, &pTexture);
    if (FAILED(r))
    {
        throw std::runtime_error("Can't create DX11 texture\n");
    }

    cv::directx::convertToD3D11Texture2D(RGBAMat, pTexture);
    if (pTexture == NULL)
    {
        std::cout << "Converting texture to DX11 failed!\n";
        return false;
    }

    // Create texture view
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
    ZeroMemory(&srvDesc, sizeof(srvDesc));
    srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = 1;
    srvDesc.Texture2D.MostDetailedMip = 0;
    g_pd3dDevice->CreateShaderResourceView(pTexture, &srvDesc, out_srv);
    pTexture->Release();

    std::cout << printf("Did load image w: %d h: %d\n", *out_width, *out_height);

    return true;
}
// Simple helper function to load an image into a DX11 texture with common settings
bool LoadTextureFromFileST(const char* filename, ID3D11ShaderResourceView** out_srv, int* out_width, int* out_height)
{
    if (g_pd3dDevice == NULL)
    {
        std::cout << "DX11 device is not initialized!\n";
        return false;
    }

    // Load from disk into a raw RGBA buffer
    int image_width = 0;
    int image_height = 0;
    unsigned char* image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
    if (image_data == NULL)
    {
        std::cout << "Image not found or invalid Image!\n";
        return false;
    }

    // Create texture
    D3D11_TEXTURE2D_DESC desc;
    ZeroMemory(&desc, sizeof(desc));
    desc.Width = image_width;
    desc.Height = image_height;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.SampleDesc.Count = 1;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    desc.CPUAccessFlags = 0;

    ID3D11Texture2D* pTexture = NULL;
    D3D11_SUBRESOURCE_DATA subResource;
    subResource.pSysMem = image_data;
    subResource.SysMemPitch = desc.Width * 4;
    subResource.SysMemSlicePitch = 0;
    g_pd3dDevice->CreateTexture2D(&desc, &subResource, &pTexture);

    // Create texture view
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
    ZeroMemory(&srvDesc, sizeof(srvDesc));
    srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = desc.MipLevels;
    srvDesc.Texture2D.MostDetailedMip = 0;
    g_pd3dDevice->CreateShaderResourceView(pTexture, &srvDesc, out_srv);
    pTexture->Release();

    *out_width = image_width;
    *out_height = image_height;
    stbi_image_free(image_data);

    return true;
}
