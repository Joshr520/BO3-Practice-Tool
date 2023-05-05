#include "ImageHelp.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "imgui.h"
#include "GUIWindow.h"

#include "PlayerOptions.h"
#include "Resources.h"

#include <filesystem>

using namespace GUIWindow;
using namespace SOECodeGuide;

// DX11 results in less memory of scuffed DX12 implementation. Downside is slightly more GPU usage (probably negligible).

namespace ImageHelp
{
    bool LoadTextureFromFile(ID3D11Device* g_pd3dDevice, const char* filename, ID3D11ShaderResourceView** out_srv, int* out_width, int* out_height)
    {
        // Load from disk into a raw RGBA buffer
        int image_width = 0;
        int image_height = 0;
        unsigned char* image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
        if (image_data == NULL)
            return false;

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

    void InitImgList()
    {
        for (const auto& entry : std::filesystem::directory_iterator("Resource Images/Gum Images/Classics"))
        {
            const std::string& name = entry.path().filename().string();
            if (name.find(".png") == std::string::npos)
                continue;
            Image gum;
            gum.imgRelativePath = name.substr(0, name.size() - 4);
            gum.imgDirectPath = entry.path().string();
            gum.index = (int)bgbImgList.size() + 1;
            bgbImgList.push_back(gum);
        }
        for (const auto& entry : std::filesystem::directory_iterator("Resource Images/Gum Images/Megas"))
        {
            const std::string& name = entry.path().filename().string();
            if (name.find(".png") == std::string::npos)
                continue;
            Image gum;
            gum.imgRelativePath = name.substr(0, name.size() - 4);
            gum.imgDirectPath = entry.path().string();
            gum.index = (int)bgbImgList.size() + 1;
            bgbImgList.push_back(gum);
    }
        for (const auto& entry : std::filesystem::directory_iterator("Resource Images/Soe Code"))
        {
            const std::string& name = entry.path().filename().string();
            if (name.find(".png") == std::string::npos)
                continue;
            Image code;
            code.imgRelativePath = name.substr(0, name.size() - 4);
            code.imgDirectPath = entry.path().string();
            code.index = (int)codeImgList.size() + 1;
            codeImgList.push_back(code);
            soeCodeCombo.push_back(name.substr(0, name.size() - 4));
        }
        for (const auto& entry : std::filesystem::directory_iterator("Resource Images/GK Valve Solver"))
        {
            const std::string& name = entry.path().filename().string();
            if (name.find(".png") == std::string::npos)
                continue;
            Image valve;
            valve.imgRelativePath = name.substr(0, name.size() - 4);
            valve.imgDirectPath = entry.path().string();
            valve.index = (int)valveSolverImgList.size() + 1;
            valveSolverImgList.push_back(valve);
        }
        for (const auto& entry : std::filesystem::directory_iterator("Resource Images/Ice Code"))
        {
            const std::string& name = entry.path().filename().string();
            if (name.find(".png") == std::string::npos)
                continue;
            Image iceCode;
            iceCode.imgRelativePath = name.substr(0, name.size() - 4);
            iceCode.imgDirectPath = entry.path().string();
            iceCode.index = (int)iceCodeImgList.size() + 1;
            iceCodeImgList.insert({ iceCode.imgRelativePath, iceCode });
        }
    }

    void InitImgTextures()
    {
        for (Image& img : bgbImgList)
        {
            bool ret = LoadTextureFromFile(g_pd3dDevice, img.imgDirectPath.c_str(), &img.imgTexture, &img.imgWidth, &img.imgHeight);
            IM_ASSERT(ret);
        }

        for (Image& img : codeImgList)
        {
            bool ret = LoadTextureFromFile(g_pd3dDevice, img.imgDirectPath.c_str(), &img.imgTexture, &img.imgWidth, &img.imgHeight);
            IM_ASSERT(ret);
        }

        for (Image& img : valveSolverImgList)
        {
            bool ret = LoadTextureFromFile(g_pd3dDevice, img.imgDirectPath.c_str(), &img.imgTexture, &img.imgWidth, &img.imgHeight);
            IM_ASSERT(ret);
        }

        for (std::pair<std::string, Image> img : iceCodeImgList)
        {
            bool ret = LoadTextureFromFile(g_pd3dDevice, img.second.imgDirectPath.c_str(), &img.second.imgTexture, &img.second.imgWidth, &img.second.imgHeight);
            iceCodeImgList[img.first].imgTexture = img.second.imgTexture;
            iceCodeImgList[img.first].imgWidth = img.second.imgWidth;
            iceCodeImgList[img.first].imgHeight = img.second.imgHeight;
            IM_ASSERT(ret);
        }
    }
}