#pragma once

#include "PixDesc.h"

#ifdef __cplusplus
extern "C" {
#endif

	int image_fill_arrays(uint8_t* dst_data[4], int dst_linesize[4],
		const uint8_t* src,
		enum PixelFormat pix_fmt, int width, int height, int align);

	int image_get_buffer_size(enum PixelFormat pix_fmt,
		int width, int height, int align);

	int image_fill_linesizes(int linesizes[4], enum PixelFormat pix_fmt, int width);

	int image_fill_pointers(uint8_t* data[4], enum PixelFormat pix_fmt, int height,
		uint8_t* ptr, const int linesizes[4]);

#ifdef __cplusplus
}
#endif