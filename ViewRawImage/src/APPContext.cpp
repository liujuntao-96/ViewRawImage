#include <iostream>
#include <assert.h>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <SDKDDKVer.h>
#include <afxdlgs.h>
#include <d3d11.h>
#include <chrono>

#include "APPContext.h"
#include "util/PixDesc.h"
#include "util/ImgUtils.h"
#include "util/Common.h"
#include "convert/Convert.h"
#include "convert/ConvertToARGB.h"
#include "imgui/imgui.h"

#define CHECK_CONTEXT() \
do {\
	if (!gAPPContext)\
		assert(0 && "gAPPContext is null.");\
}while(0)

extern ID3D11Device* g_pd3dDevice;
extern ID3D11DeviceContext* g_pd3dDeviceContext;
extern IDXGISwapChain* g_pSwapChain;
extern ID3D11RenderTargetView* g_mainRenderTargetView;
extern ID3D11Texture2D* g_pTexture;
extern ID3D11ShaderResourceView* g_pSRView;

namespace ctx {

	struct APPContext_t
	{
		int scaling = 100;
		int width = 0;
		int height = 0;
		int fps = 30;
		std::string filePath;
		ImTextureID textureID;
		bool upDateTexFlag = false;
		enum PixelFormat format;
		enum Tiled tiled;
		bool autoResize;
		unsigned char* buffer = nullptr;
		unsigned char* memBuffer = nullptr;
		unsigned char* tiledBuffer = nullptr;
		unsigned char* convertBuffer = nullptr;
		unsigned char* texBuffer = nullptr;
		int bufferSize = 0;
		int tiledBufferSize = 0;
		int convertBufferSize = 0;
		std::ifstream file;
		int64_t fileSize = 0;
		int64_t fileOffset = 0;
		int64_t frameTotal = 0;
		bool playFlag = false;
		bool playMode = false;
		std::chrono::system_clock::time_point lastTime;
		int progressBar = 0;
		bool progressBarFlag = 0;
		DXGI_FORMAT dxgiFormat;
		int step = 0;
	};

	static APPContext gAPPContext = nullptr;

	bool loadTexture(const uint8_t* pData, DXGI_FORMAT format, int step)
	{
		CHECK_CONTEXT();

		if (g_pSRView) {
			g_pSRView->Release();
			g_pSRView = nullptr;
		}
		if (g_pTexture) {
			g_pTexture->Release();
			g_pTexture = nullptr;
		}

		int width = gAPPContext->width;
		int height = gAPPContext->height;

		CD3D11_TEXTURE2D_DESC texDesc(format, width, height, 1, 0, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET,
			D3D11_USAGE_DEFAULT, 0, 1, 0, D3D11_RESOURCE_MISC_GENERATE_MIPS);
		g_pd3dDevice->CreateTexture2D(&texDesc, nullptr, &g_pTexture);
		g_pd3dDeviceContext->UpdateSubresource(g_pTexture, 0, nullptr, pData, width * step, 0);

		CD3D11_SHADER_RESOURCE_VIEW_DESC srvDesc(D3D11_SRV_DIMENSION_TEXTURE2D, format);
		g_pd3dDevice->CreateShaderResourceView(g_pTexture, &srvDesc, &g_pSRView);
		g_pd3dDeviceContext->GenerateMips(g_pSRView);

		gAPPContext->textureID = (ImTextureID)g_pSRView;

		return true;
	}

	void* GetBuffer()
	{
		CHECK_CONTEXT();
		return (void*)gAPPContext->buffer;
	}

	void* GetMemBuffer()
	{
		CHECK_CONTEXT();
		return (void*)gAPPContext->memBuffer;
	}

	int GetBufferSize()
	{
		CHECK_CONTEXT();
		return gAPPContext->bufferSize;
	}

	void SetFormat(enum PixelFormat format)
	{
		CHECK_CONTEXT();
		gAPPContext->format = format;
	}

	bool GetUpDateTexFlag()
	{
		CHECK_CONTEXT();
		return gAPPContext->upDateTexFlag;
	}

	int* GetProgressBarPtr()
	{
		CHECK_CONTEXT();
		return &gAPPContext->progressBar;
	}

	void SetProgressBarFlag(bool status)
	{
		CHECK_CONTEXT();
		gAPPContext->progressBarFlag = status;
	}

	void SetUpDateTexFlag(bool status)
	{
		CHECK_CONTEXT();
		gAPPContext->upDateTexFlag = status;
	}


	void SetPlayFlag(bool status)
	{
		CHECK_CONTEXT();
		gAPPContext->playFlag = status;
	}

	bool GetPlayFlag()
	{
		CHECK_CONTEXT();
		return gAPPContext->playFlag;
	}

	bool* GetPlayModePtr()
	{
		CHECK_CONTEXT();
		return &gAPPContext->playMode;
	}

	int64_t GetFrameTotal()
	{
		CHECK_CONTEXT();
		return gAPPContext->frameTotal;
	}

	void OpenFile(const std::string& path)
	{
		CHECK_CONTEXT();
		CloseFile();
		gAPPContext->filePath = path;

		gAPPContext->file.open(gAPPContext->filePath, std::ios::in | std::ios::binary | std::ios::ate);
		if (!gAPPContext->file.is_open()) {
			std::cerr << "Failed to open file: " << path << std::endl;
			assert(0 && "Failed to open file");
		}
		gAPPContext->fileSize = gAPPContext->file.tellg();
		gAPPContext->file.seekg(0, std::ios::beg);
		gAPPContext->fileOffset = 0;
		gAPPContext->progressBar = 0;
	}

	void CloseFile()
	{
		CHECK_CONTEXT();
		if (gAPPContext->file.is_open())
			gAPPContext->file.close();

		gAPPContext->fileOffset = 0;
		gAPPContext->fileSize = 0;
	}

	void swapChannels(uint8_t* buffer, enum PixelFormat format, DXGI_FORMAT* dxgiFormat, int* step)
	{
		CHECK_CONTEXT();
		int w = gAPPContext->width;
		int h = gAPPContext->height;

		switch (format)
		{
		case PIX_FMT_NONE:
			assert(0 && "swapChannels:format err.");
			break;
		case PIX_FMT_RGBA8888:
			*dxgiFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
			*step = 4;
			break;
		case PIX_FMT_R5G5B5A1:
			*dxgiFormat = DXGI_FORMAT_B5G5R5A1_UNORM;
			*step = 2;
			convert::R5G5B5A1ToB5G5R5A1(w, h, buffer);
			break;
		case PIX_FMT_RGBA4444:
			*dxgiFormat = DXGI_FORMAT_B8G8R8A8_UNORM;
			*step = 4;
			break;
		case PIX_FMT_BGR565:
			*dxgiFormat = DXGI_FORMAT_B5G6R5_UNORM;
			*step = 2;
			break;
		case PIX_FMT_BGRA8888:
			*dxgiFormat = DXGI_FORMAT_B8G8R8A8_UNORM;
			*step = 4;
			break;
		case PIX_FMT_BGRX8888:
			*dxgiFormat = DXGI_FORMAT_B8G8R8X8_UNORM;
			*step = 4;
			break;
		case PIX_FMT_B10G10R10A2:
			*dxgiFormat = DXGI_FORMAT_R10G10B10A2_UNORM;
			*step = 4;
			convert::B10G10R10A2ToR10G10B10A2(w, h, buffer);
			break;
		case PIX_FMT_B5G5R5A1:
			*dxgiFormat = DXGI_FORMAT_B5G5R5A1_UNORM;
			*step = 2;
			break;
		case PIX_FMT_B5G5R5X1:
			*dxgiFormat = DXGI_FORMAT_B5G5R5A1_UNORM;
			*step = 2;
			break;
		case PIX_FMT_BGRA4444:
			*dxgiFormat = DXGI_FORMAT_B8G8R8A8_UNORM;
			*step = 4;
			break;
		case PIX_FMT_BGRX4444:
			*dxgiFormat = DXGI_FORMAT_B8G8R8X8_UNORM;
			*step = 4;
			break;
		case PIX_FMT_RGB565:
			*dxgiFormat = DXGI_FORMAT_B5G6R5_UNORM;
			*step = 2;
			convert::BGR565ToBGR565(w, h, buffer);
			break;
		case PIX_FMT_ABGR8888:
			*dxgiFormat = DXGI_FORMAT_B8G8R8A8_UNORM;
			*step = 4;
			convert::ABGR8888ToB8G8R8A8(w, h, buffer);
			break;
		case PIX_FMT_A1B5G5R5:
			*dxgiFormat = DXGI_FORMAT_B5G5R5A1_UNORM;
			*step = 2;
			convert::A1B5G5R5ToB5G5R5A1(w, h, buffer);
			break;
		case PIX_FMT_ABGR4444:
			*dxgiFormat = DXGI_FORMAT_B8G8R8A8_UNORM;
			*step = 4;
			break;
		case PIX_FMT_ARGB8888:
			*dxgiFormat = DXGI_FORMAT_B8G8R8A8_UNORM;
			*step = 4;
			convert::ARGB8888ToB8G8R8A8(w, h, buffer);
			break;
		case PIX_FMT_A1R5G5B5:
			*dxgiFormat = DXGI_FORMAT_B5G5R5A1_UNORM;
			*step = 2;
			convert::A1R5G5B5ToB5G5R5A1(w, h, buffer);
			break;
		case PIX_FMT_ARGB4444:
			*dxgiFormat = DXGI_FORMAT_B8G8R8A8_UNORM;
			*step = 4;
			break;
		case PIX_FMT_YUYV:
			*dxgiFormat = DXGI_FORMAT_B8G8R8A8_UNORM;
			*step = 4;
			break;
		case PIX_FMT_UYVY:
			*dxgiFormat = DXGI_FORMAT_B8G8R8A8_UNORM;
			*step = 4;
			break;
		case PIX_FMT_YV12:
			*dxgiFormat = DXGI_FORMAT_B8G8R8A8_UNORM;
			*step = 4;
			break;
		case PIX_FMT_I420:
			*dxgiFormat = DXGI_FORMAT_B8G8R8A8_UNORM;
			*step = 4;
			break;
		case PIX_FMT_NV12:
			*dxgiFormat = DXGI_FORMAT_B8G8R8A8_UNORM;
			*step = 4;
			break;
		case PIX_FMT_NV21:
			*dxgiFormat = DXGI_FORMAT_B8G8R8A8_UNORM;
			*step = 4;
			break;
		case PIX_FMT_NV16:
			*dxgiFormat = DXGI_FORMAT_B8G8R8A8_UNORM;
			*step = 4;
			break;
		case PIX_FMT_NV61:
			*dxgiFormat = DXGI_FORMAT_B8G8R8A8_UNORM;
			*step = 4;
			break;
		case PIX_FMT_P010MSB:
			*dxgiFormat = DXGI_FORMAT_B8G8R8A8_UNORM;
			*step = 4;
			break;
		case PIX_FMT_P010LSB:
			*dxgiFormat = DXGI_FORMAT_B8G8R8A8_UNORM;
			*step = 4;
			break;
		case PIX_FMT_I010:
			*dxgiFormat = DXGI_FORMAT_B8G8R8A8_UNORM;
			*step = 4;
			break;
		case PIX_FMT_BGR888Planar:
			*dxgiFormat = DXGI_FORMAT_B8G8R8A8_UNORM;
			*step = 4;
			break;
		case PIX_FMT_R10G10B10A2:
			*dxgiFormat = DXGI_FORMAT_R10G10B10A2_UNORM;
			*step = 4;
			break;
		case PIX_FMT_NB:
			assert(0 && "swapChannels:format err.");
			break;
		default:
			assert(0 && "swapChannels:format err.");
			break;
		}
	}


	void UpDateTexture()
	{
		CHECK_CONTEXT();
		uint8_t* data[4];
		int linesize[4];
		bool convertTiled = false;
		if (gAPPContext->width <= 0 || gAPPContext->height <= 0)
			return;

		const PixFmtDescriptor* desc = pix_fmt_desc_get(gAPPContext->format);
		if (!desc) {
			assert(0 && "pixdesc is null.");
		}

		int size = image_get_buffer_size(gAPPContext->format, gAPPContext->width, gAPPContext->height, 1);
		if (size < 0)
			assert(0 && "get buffer size fail.");

		if (size != gAPPContext->bufferSize) {
			if (gAPPContext->buffer) {
				delete[] gAPPContext->buffer;
				delete[] gAPPContext->memBuffer;
			}
			gAPPContext->buffer = new unsigned char[size];
			gAPPContext->memBuffer = new unsigned char[size];
			gAPPContext->bufferSize = size;
		}

		if (gAPPContext->tiled != TILED_LINEAR) {
			if (size != gAPPContext->tiledBufferSize)
			{
				if (gAPPContext->tiledBufferSize)
					delete[] gAPPContext->tiledBuffer;
				gAPPContext->tiledBuffer = new unsigned char[size];
				gAPPContext->tiledBufferSize = size;
			}
			convertTiled = true;
		}

		if (image_fill_arrays(data, linesize, (const uint8_t*)gAPPContext->buffer, gAPPContext->format,
			gAPPContext->width, gAPPContext->height, 1) < 0)
			assert(0 && "image fill arrays fail.");

		if (!gAPPContext->file.is_open()) {
			return;
		}
		if (gAPPContext->playMode && gAPPContext->playFlag) {
			auto currentTime = std::chrono::system_clock::now();
			auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - gAPPContext->lastTime);
			if (elapsedTime.count() > (int64_t)((1.0 / gAPPContext->fps) * 1000)) {
				uint64_t frameTotal = gAPPContext->fileSize / size;
				gAPPContext->frameTotal = frameTotal;
				uint64_t frameOffset = frameTotal * gAPPContext->progressBar / 100;
				uint64_t temp = 0;
				if (gAPPContext->progressBarFlag) {
					gAPPContext->fileOffset = frameOffset * size;
				}

				if (gAPPContext->fileOffset > gAPPContext->fileSize - size) {
					gAPPContext->fileOffset = gAPPContext->fileSize - size;
					gAPPContext->playFlag = false;
				}
				if (gAPPContext->fileOffset < 0)
					gAPPContext->fileOffset = 0;

				gAPPContext->file.seekg(gAPPContext->fileOffset, std::ios::beg);
				gAPPContext->file.read((char*)gAPPContext->buffer, size);
				memcpy(gAPPContext->memBuffer, gAPPContext->buffer, size);
				int* progressBarPtr = ctx::GetProgressBarPtr();
				gAPPContext->fileOffset += size;
				*progressBarPtr = (uint32_t)((float)(gAPPContext->fileOffset / size * 100) / (float)frameTotal);
				gAPPContext->lastTime = currentTime;
				gAPPContext->progressBarFlag = false;
			}
			else
			{
				goto display;
			}
		}
		else {
			gAPPContext->file.seekg(0, std::ios::beg);
			gAPPContext->file.read((char*)gAPPContext->buffer, size > gAPPContext->fileSize ? gAPPContext->fileSize : size);
			memcpy(gAPPContext->memBuffer, gAPPContext->buffer, (size_t)(size > gAPPContext->fileSize ? gAPPContext->fileSize : size));
		}

		if (convertTiled)
		{
			convert::ConvertTiled(gAPPContext->width, gAPPContext->height, gAPPContext->tiled, gAPPContext->format, gAPPContext->buffer, gAPPContext->tiledBuffer);
			gAPPContext->texBuffer = gAPPContext->tiledBuffer;
		}
		else
			gAPPContext->texBuffer = gAPPContext->buffer;

		swapChannels(gAPPContext->texBuffer, gAPPContext->format, &gAPPContext->dxgiFormat, &gAPPContext->step);

		if (!(desc->flags & PIX_FMT_FLAG_RGB) || gAPPContext->format == PIX_FMT_BGR888Planar || desc->flags & PIX_FMT_FLAG_4444) {
			uint8_t* buffer = convertTiled ? gAPPContext->tiledBuffer : gAPPContext->buffer;
			if (gAPPContext->convertBufferSize != gAPPContext->width * gAPPContext->height * 4) {

				if (gAPPContext->convertBuffer)
					delete[] gAPPContext->convertBuffer;

				gAPPContext->convertBuffer = new unsigned char[gAPPContext->width * gAPPContext->height * 4];
				gAPPContext->convertBufferSize = gAPPContext->width * gAPPContext->height * 4;
			}
			convert::ConvertToARGB(buffer, gAPPContext->convertBuffer, gAPPContext->width, gAPPContext->height, gAPPContext->format);
			gAPPContext->texBuffer = gAPPContext->convertBuffer;
		}

	display:
		loadTexture(gAPPContext->texBuffer, gAPPContext->dxgiFormat, gAPPContext->step);

		SetUpDateTexFlag(false);
	}

	unsigned int GetTextureID()
	{
		CHECK_CONTEXT();
		return (unsigned int)gAPPContext->textureID;
	}

	int GetWidth()
	{
		CHECK_CONTEXT();
		return gAPPContext->width;
	}

	int GetHeight()
	{
		CHECK_CONTEXT();
		return gAPPContext->height;
	}

	int* GetWidthPtr()
	{
		CHECK_CONTEXT();
		return &gAPPContext->width;
	}

	int* GetHeightPtr()
	{
		CHECK_CONTEXT();
		return &gAPPContext->height;
	}

	int* GetFormatPtr()
	{
		CHECK_CONTEXT();
		return (int*)&gAPPContext->format;
	}

	int* GetTiledPtr()
	{
		CHECK_CONTEXT();
		return (int*)&gAPPContext->tiled;
	}

	bool* GetAutoResizeFlagPtr()
	{
		CHECK_CONTEXT();
		return &gAPPContext->autoResize;
	}

	int* GetScalingPtr()
	{
		CHECK_CONTEXT();
		return &gAPPContext->scaling;
	}

	int* GetFPSPtr()
	{
		CHECK_CONTEXT();
		return &gAPPContext->fps;
	}

	void SetWidth(int w)
	{
		CHECK_CONTEXT();
		gAPPContext->width = w;
	}

	void SetHeight(int h)
	{
		CHECK_CONTEXT();
		gAPPContext->height = h;
	}

	void SetFilePath(const std::string& filePath)
	{
		CHECK_CONTEXT();
		gAPPContext->filePath = filePath;
	}

	std::string& GetFilePath(void)
	{
		CHECK_CONTEXT();
		return gAPPContext->filePath;
	}

	APPContext GetAPPContext()
	{
		CHECK_CONTEXT();
		return gAPPContext;
	}



	bool InitAPPContext()
	{
		gAPPContext = new APPContext_t();
		if (!gAPPContext)
			return false;

		if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
			assert(0 && "AfxWinInit fail.");

		return true;
	}

	void DeInitAPPContext()
	{
		CHECK_CONTEXT();

		AfxWinTerm();

		if (gAPPContext->file.is_open())
			gAPPContext->file.close();

		if (gAPPContext->buffer) {
			delete[] gAPPContext->buffer;
			delete[] gAPPContext->memBuffer;
			gAPPContext->buffer = nullptr;
			gAPPContext->memBuffer = nullptr;
		}
		if (gAPPContext->tiledBuffer)
		{
			delete[] gAPPContext->tiledBuffer;
			gAPPContext->tiledBuffer = nullptr;
		}
		if (gAPPContext->convertBuffer) {
			delete[] gAPPContext->convertBuffer;
			gAPPContext->convertBuffer = nullptr;
		}
		if (gAPPContext)
			delete gAPPContext;
		gAPPContext = nullptr;
	}
}