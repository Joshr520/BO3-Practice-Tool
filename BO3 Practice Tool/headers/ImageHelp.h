#pragma once

#include <d3d11.h>

#include <vector>
#include <unordered_map>
#include <string>

namespace ImageHelp
{
	struct Image
	{
		int imgWidth = 0;
		int imgHeight = 0;
		int index = 0;
		std::string imgDirectPath = "";
		std::string imgRelativePath = "";
		ID3D11ShaderResourceView* imgTexture = NULL;
	};

	bool LoadTextureFromFile(ID3D11Device* g_pd3dDevice, const char* filename, ID3D11ShaderResourceView** out_srv, int* out_width, int* out_height);
	void InitImgList();
	void InitImgTextures();

	inline std::vector<Image> bgbImgList;
	inline std::vector<Image> codeImgList;
	inline std::vector<Image> valveSolverImgList;
	inline std::unordered_map<std::string, Image> iceCodeImgList;
}