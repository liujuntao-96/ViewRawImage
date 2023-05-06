#pragma once

#include "util/PixDesc.h"

namespace ctx {
	struct APPContext_t;
	typedef struct APPContext_t* APPContext;

	APPContext GetAPPContext();

	void SetFormat(enum PixelFormat format);

	void SetUpDateTexFlag(bool status);

	bool* GetAutoResizeFlagPtr();

	int64_t GetFrameTotal();

	int* GetScalingPtr();

	void* GetBuffer();

	int GetBufferSize();

	unsigned int GetTextureID();

	bool GetUpDateTexFlag();

	void UpDateTexture();
	void* GetMemBuffer();
	bool InitAPPContext();

	void DeInitAPPContext();

	void SetFilePath(const std::string& filePath);
	std::string& GetFilePath(void);

	int* GetWidthPtr();

	int* GetHeightPtr();
	int* GetFormatPtr();
	int* GetTiledPtr();
	int GetWidth();
	int GetHeight();
	void SetWidth(int w);
	void SetHeight(int h);
	void OpenFile(const std::string& path);
	void CloseFile();
	int* GetFPSPtr();
	bool* GetPlayModePtr();
	void SetPlayFlag(bool status);
	bool GetPlayFlag();
	int* GetProgressBarPtr();
	void SetProgressBarFlag(bool status);
}
