#pragma once
#include <cstdint>
#include "../util/ImgUtils.h"

namespace convert {
	int YUY2ToARGB(const uint8_t* src_yuy2,
		int src_stride_yuy2,
		uint8_t* dst_argb,
		int dst_stride_argb,
		int width,
		int height);

	int UYVYToARGB(const uint8_t* src_uyvy,
		int src_stride_uyvy,
		uint8_t* dst_argb,
		int dst_stride_argb,
		int width,
		int height);

	int I420ToARGB(const uint8_t* src_y,
		int src_stride_y,
		const uint8_t* src_u,
		int src_stride_u,
		const uint8_t* src_v,
		int src_stride_v,
		uint8_t* dst_argb,
		int dst_stride_argb,
		int width,
		int height);

	int NV12ToARGB(const uint8_t* src_y,
		int src_stride_y,
		const uint8_t* src_uv,
		int src_stride_uv,
		uint8_t* dst_argb,
		int dst_stride_argb,
		int width,
		int height);

	int NV21ToARGB(const uint8_t* src_y,
		int src_stride_y,
		const uint8_t* src_vu,
		int src_stride_vu,
		uint8_t* dst_argb,
		int dst_stride_argb,
		int width,
		int height);

	int NV16ToARGB(const uint8_t* src_y,
		int src_stride_y,
		const uint8_t* src_uv,
		int src_stride_uv,
		uint8_t* dst_argb,
		int dst_stride_argb,
		int width,
		int height);

	int NV61ToARGB(const uint8_t* src_y,
		int src_stride_y,
		const uint8_t* src_vu,
		int src_stride_vu,
		uint8_t* dst_argb,
		int dst_stride_argb,
		int width,
		int height);

	int P010MSBToARGB(const uint16_t* src_y,
		int src_stride_y,
		const uint16_t* src_uv,
		int src_stride_uv,
		uint8_t* dst_argb,
		int dst_stride_argb,
		int width,
		int height);

	int P010LSBToARGB(const uint16_t* src_y,
		int src_stride_y,
		const uint16_t* src_uv,
		int src_stride_uv,
		uint8_t* dst_argb,
		int dst_stride_argb,
		int width,
		int height);

	int I010ToARGB(const uint16_t* src_y,
		int src_stride_y,
		const uint16_t* src_u,
		int src_stride_u,
		const uint16_t* src_v,
		int src_stride_v,
		uint8_t* dst_argb,
		int dst_stride_argb,
		int width,
		int height);

	void BGRPToBGRA(uint8_t* bgrp, uint8_t* argb, int w, int h);

	void ConvertTiled(int w, int h, enum Tiled tiled, enum PixelFormat pix_fmt, uint8_t* buffer, uint8_t* tiledBuffer);
	void ConvertFromTiled(int w, int h, enum Tiled tiled, enum PixelFormat pix_fmt, uint8_t* buffer, uint8_t* tiledBuffer);

	void R5G5B5A1ToB5G5R5A1(int w, int h, uint8_t* buffer);
	int ABGR1555ToARGB(const uint8_t* src_argb1555,
		int src_stride_argb1555,
		uint8_t* dst_argb,
		int dst_stride_argb,
		int width,
		int height);
	void B10G10R10A2ToR10G10B10A2(int w, int h, uint8_t* buffer);
	void BGR565ToBGR565(int w, int h, uint8_t* buffer);
	void ABGR8888ToB8G8R8A8(int w, int h, uint8_t* buffer);
	void A1B5G5R5ToB5G5R5A1(int w, int h, uint8_t* buffer);
	void ARGB8888ToB8G8R8A8(int w, int h, uint8_t* buffer);
	int ABGR4444ToARGB(const uint8_t* src_argb4444,
		int src_stride_argb4444,
		uint8_t* dst_argb,
		int dst_stride_argb,
		int width,
		int height);
	int ARGB4444ToARGB(const uint8_t* src_argb4444,
		int src_stride_argb4444,
		uint8_t* dst_argb,
		int dst_stride_argb,
		int width,
		int height);

	int BGRA4444ToARGB(const uint8_t* src_argb4444,
		int src_stride_argb4444,
		uint8_t* dst_argb,
		int dst_stride_argb,
		int width,
		int height);

	int RGBA4444ToARGB(const uint8_t* src_argb4444,
		int src_stride_argb4444,
		uint8_t* dst_argb,
		int dst_stride_argb,
		int width,
		int height);

	int ARGB1555ToARGB(const uint8_t* src_argb1555,
		int src_stride_argb1555,
		uint8_t* dst_argb,
		int dst_stride_argb,
		int width,
		int height);
	int RGBA5551ToARGB(const uint8_t* src_argb1555,
		int src_stride_argb1555,
		uint8_t* dst_argb,
		int dst_stride_argb,
		int width,
		int height);
	int BGRA5551ToARGB(const uint8_t* src_argb1555,
		int src_stride_argb1555,
		uint8_t* dst_argb,
		int dst_stride_argb,
		int width,
		int height);
	void A1R5G5B5ToB5G5R5A1(int w, int h, uint8_t* buffer);
	void RGBA8888ToARGB(int w, int h, uint8_t* buffer);
	void BGRA8888ToRGBA(int w, int h, uint8_t* buffer);
	void ABGR8888ToARGB(int w, int h, uint8_t* buffer);
	void B10G10R10A2ToARGB(int w, int h, uint8_t* buffer, uint8_t* argb);
	void R10G10B10A2ToARGB(int w, int h, uint8_t* buffer, uint8_t* argb);

	int BGR565ToARGB(const uint8_t* src_rgb565,
		int src_stride_rgb565,
		uint8_t* dst_argb,
		int dst_stride_argb,
		int width,
		int height);
	int RGB565ToARGB(const uint8_t* src_rgb565,
		int src_stride_rgb565,
		uint8_t* dst_argb,
		int dst_stride_argb,
		int width,
		int height);

	void ARGB8888ToARGB(int w, int h, uint8_t* buffer);

	int ARGBToABGR1555(const uint8_t* src_argb,
		int src_stride_argb,
		uint8_t* dst_argb1555,
		int dst_stride_argb1555,
		int width,
		int height);
	int ARGBToARGB1555(const uint8_t* src_argb,
		int src_stride_argb,
		uint8_t* dst_argb1555,
		int dst_stride_argb1555,
		int width,
		int height);
	int ARGBToBGRA1555(const uint8_t* src_argb,
		int src_stride_argb,
		uint8_t* dst_argb1555,
		int dst_stride_argb1555,
		int width,
		int height);
	int ARGBToRGBA1555(const uint8_t* src_argb,
		int src_stride_argb,
		uint8_t* dst_argb1555,
		int dst_stride_argb1555,
		int width,
		int height);

	int ARGBToBGRA4444(const uint8_t* src_argb,
		int src_stride_argb,
		uint8_t* dst_argb4444,
		int dst_stride_argb4444,
		int width,
		int height);
	int ARGBToRGBA4444(const uint8_t* src_argb,
		int src_stride_argb,
		uint8_t* dst_argb4444,
		int dst_stride_argb4444,
		int width,
		int height);
	int ARGBToABGR4444(const uint8_t* src_argb,
		int src_stride_argb,
		uint8_t* dst_argb4444,
		int dst_stride_argb4444,
		int width,
		int height);
	int ARGBToARGB4444(const uint8_t* src_argb,
		int src_stride_argb,
		uint8_t* dst_argb4444,
		int dst_stride_argb4444,
		int width,
		int height);

	int ARGBToBGR565(const uint8_t* src_argb,
		int src_stride_argb,
		uint8_t* dst_rgb565,
		int dst_stride_rgb565,
		int width,
		int height);
	int ARGBToRGB565(const uint8_t* src_argb,
		int src_stride_argb,
		uint8_t* dst_rgb565,
		int dst_stride_rgb565,
		int width,
		int height);
	void BGRAToR10G10B10A2(int w, int h, uint8_t* argb_data, uint8_t* yuv_data);
	void BGRAToB10G10R10A2(int w, int h, uint8_t* argb_data, uint8_t* yuv_data);
	void BGRAToABGR8888(int w, int h, uint8_t* argb_data, uint8_t* yuv_data);
	void BGRAToARGB8888(int w, int h, uint8_t* argb_data, uint8_t* yuv_data);
	void BGRAToYUYV(int width, int height, uint8_t* pBgra, uint8_t* pYuv);
	void BGRAToUYVY(int width, int height, uint8_t* pBgra, uint8_t* pYuv);
	void BGRAToRGBA1555(uint8_t* argb_data, uint8_t* yuv_data, int w, int h);
	void BGRAToYUV420(unsigned char* RgbBuf, int w, int h, unsigned char* yuvBuf);
	void BGRAToYV12(unsigned char* RgbBuf, int w, int h, unsigned char* yuvBuf);
	void BGRA8888ToNV12(const uint8_t* src_data, int src_width, int src_height, uint8_t* dst_data);
	void BGRA8888ToNV21(const uint8_t* src_data, int src_width, int src_height, uint8_t* dst_data);
	void BGRA8888ToNV16(const uint8_t* src_data, int src_width, int src_height, uint8_t* dst_data);
	void BGRA8888ToNV61(const uint8_t* src_data, int src_width, int src_height, uint8_t* dst_data);
	void BGRA8888ToP010LSB(const uint8_t* src_data, int src_width, int src_height, uint8_t* dst_data);
	void BGRA8888ToP010MSB(const uint8_t* src_data, int src_width, int src_height, uint8_t* dst_data);
	void BGRAToI010(unsigned char* RgbBuf, int w, int h, unsigned char* yuvBuf);
	void BGRAToBGR888P(uint8_t* argb, int w, int h, uint8_t* bgrp);
};