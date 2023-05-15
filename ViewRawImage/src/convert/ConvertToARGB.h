#pragma once
#include <cstdint>
#include "../util/PixFmt.h"

namespace convert {
	void ConvertToARGB(uint8_t* yuv_data, uint8_t* argb_data, int w, int h, enum PixelFormat yuv_fmt);
	void ConvertFromARGB(uint8_t* argb_data, uint8_t* yuv_data, int w, int h, enum PixelFormat yuv_fmt);
};