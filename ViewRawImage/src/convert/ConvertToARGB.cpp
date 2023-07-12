#include <assert.h>
#include <vcruntime_string.h>

#include "ConvertToARGB.h"
#include "../util/ImgUtils.h"
#include "Convert.h"


namespace convert {
	void ConvertToARGB(uint8_t* yuv_data, uint8_t* argb_data, int w, int h, enum PixelFormat yuv_fmt)
	{
		uint8_t* data[4];
		int linesize[4];
		int argbStride = w * 4;

		if (image_fill_arrays(data, linesize, (const uint8_t*)yuv_data, yuv_fmt, w, h, 1) < 0)
			assert(0 && "image fill arrays fail.");

		switch (yuv_fmt)
		{
		case PIX_FMT_RGBA8888:
			convert::RGBA8888ToARGB(w, h, yuv_data);
			memcpy(argb_data, yuv_data, w * h * 4);
			break;
		case PIX_FMT_ABGR8888:
			convert::ABGR8888ToARGB(w, h, yuv_data);
			memcpy(argb_data, yuv_data, w * h * 4);
			break;
		case PIX_FMT_ARGB8888:
			convert::ARGB8888ToARGB(w, h, yuv_data);
			memcpy(argb_data, yuv_data, w * h * 4);
			break;
		case PIX_FMT_R5G5B5A1:
			convert::RGBA5551ToARGB(data[0], linesize[0], argb_data, argbStride, w, h);
			break;
		case PIX_FMT_B5G5R5A1:
		case PIX_FMT_B5G5R5X1:
			convert::BGRA5551ToARGB(data[0], linesize[0], argb_data, argbStride, w, h);
			break;
		case PIX_FMT_BGR565:
			convert::BGR565ToARGB(data[0], linesize[0], argb_data, argbStride, w, h);
			break;
		case PIX_FMT_RGB565:
			convert::RGB565ToARGB(data[0], linesize[0], argb_data, argbStride, w, h);
			break;
		case PIX_FMT_BGRA8888:
		case PIX_FMT_BGRX8888:
			memcpy(argb_data, yuv_data, w * h * 4);
			break;
		case PIX_FMT_B10G10R10A2:
			convert::B10G10R10A2ToARGB(w, h, yuv_data, argb_data);
			break;
		case PIX_FMT_R10G10B10A2:
			convert::R10G10B10A2ToARGB(w, h, yuv_data, argb_data);
			break;
		case PIX_FMT_A1R5G5B5:
			convert::ARGB1555ToARGB(data[0], linesize[0], argb_data, argbStride, w, h);
			break;
		case PIX_FMT_A1B5G5R5:
			convert::ABGR1555ToARGB(data[0], linesize[0], argb_data, argbStride, w, h);
			break;
		case PIX_FMT_ABGR4444:
			convert::ABGR4444ToARGB(data[0], linesize[0], argb_data, argbStride, w, h);
			break;
		case PIX_FMT_ARGB4444:
			convert::ARGB4444ToARGB(data[0], linesize[0], argb_data, argbStride, w, h);
			break;
		case PIX_FMT_BGRA4444:
			convert::BGRA4444ToARGB(data[0], linesize[0], argb_data, argbStride, w, h);
			break;
		case PIX_FMT_BGRX4444:
			convert::BGRA4444ToARGB(data[0], linesize[0], argb_data, argbStride, w, h);
			break;
		case PIX_FMT_RGBA4444:
			convert::RGBA4444ToARGB(data[0], linesize[0], argb_data, argbStride, w, h);
			break;
		case PIX_FMT_YUYV:
			convert::YUY2ToARGB(data[0], linesize[0], argb_data, argbStride, w, h);
			break;
		case PIX_FMT_UYVY:
			convert::UYVYToARGB(data[0], linesize[0], argb_data, argbStride, w, h);
			break;
		case PIX_FMT_YV12:
			convert::I420ToARGB(data[0], linesize[0], data[2], linesize[2], data[1], linesize[1], argb_data, argbStride, w, h);
			break;
		case PIX_FMT_I420:
			convert::I420ToARGB(data[0], linesize[0], data[1], linesize[1], data[2], linesize[2], argb_data, argbStride, w, h);
			break;
		case PIX_FMT_NV12:
			convert::NV12ToARGB(data[0], linesize[0], data[1], linesize[1], argb_data, argbStride, w, h);
			break;
		case PIX_FMT_NV21:
			convert::NV21ToARGB(data[0], linesize[0], data[1], linesize[1], argb_data, argbStride, w, h);
			break;
		case PIX_FMT_NV16:
			convert::NV16ToARGB(data[0], linesize[0], data[1], linesize[1], argb_data, argbStride, w, h);
			break;
		case PIX_FMT_NV61:
			convert::NV61ToARGB(data[0], linesize[0], data[1], linesize[1], argb_data, argbStride, w, h);
			break;
		case PIX_FMT_P010MSB:
			convert::P010MSBToARGB((uint16_t*)data[0], linesize[0], (uint16_t*)data[1], linesize[1], argb_data, argbStride, w, h);
			break;
		case PIX_FMT_P010LSB:
			convert::P010LSBToARGB((uint16_t*)data[0], linesize[0], (uint16_t*)data[1], linesize[1], argb_data, argbStride, w, h);
			break;
		case PIX_FMT_I010:
			convert::I010ToARGB((uint16_t*)data[0], linesize[0], (uint16_t*)data[1], linesize[1], (uint16_t*)data[2], linesize[2], argb_data, argbStride, w, h);
			break;
		case PIX_FMT_BGR888Planar:
			convert::BGRPToBGRA(data[0], argb_data, w, h);
			break;
		case PIX_FMT_RGB888Planar:
			convert::RGBPToBGRA(data[0], argb_data, w, h);
			break;
		case PIX_FMT_RGB24:
			convert::RGB24ToBGRA(data[0], argb_data, w, h);
			break;
		case PIX_FMT_BGR24:
			convert::BGR24ToBGRA(data[0], argb_data, w, h);
			break;
		default:
			assert(0 && "format is unsupported.");
			break;
		}
	}

	void ConvertFromARGB(uint8_t* argb_data, uint8_t* yuv_data, int w, int h, enum PixelFormat yuv_fmt)
	{
		uint8_t* data[4];
		int linesize[4];
		int argbStride = w * 4;

		if (image_fill_arrays(data, linesize, (const uint8_t*)yuv_data, yuv_fmt, w, h, 1) < 0)
			assert(0 && "image fill arrays fail.");

		switch (yuv_fmt)
		{
		case PIX_FMT_RGBA8888:
			convert::BGRA8888ToRGBA(w, h, argb_data);
			memcpy(yuv_data, argb_data, w * h * 4);
			break;
		case PIX_FMT_R5G5B5A1:
			convert::ARGBToABGR1555(argb_data, argbStride, yuv_data, linesize[0], w, h);
			break;
		case PIX_FMT_RGBA4444:
			convert::ARGBToABGR4444(argb_data, argbStride, yuv_data, linesize[0], w, h);
			break;
		case PIX_FMT_BGR565:
			convert::ARGBToRGB565(argb_data, argbStride, yuv_data, linesize[0], w, h);
			break;
		case PIX_FMT_BGRA8888:
			memcpy(yuv_data, argb_data, argbStride * h);
			break;
		case PIX_FMT_BGRX8888:
			memcpy(yuv_data, argb_data, argbStride * h);
			break;
		case PIX_FMT_B10G10R10A2:
			convert::BGRAToB10G10R10A2(w, h, argb_data, yuv_data);
			break;
		case PIX_FMT_B5G5R5A1:
			convert::ARGBToARGB1555(argb_data, argbStride, yuv_data, linesize[0], w, h);
			break;
		case PIX_FMT_B5G5R5X1:
			convert::ARGBToARGB1555(argb_data, argbStride, yuv_data, linesize[0], w, h);
			break;
		case PIX_FMT_BGRA4444:
			convert::ARGBToARGB4444(argb_data, argbStride, yuv_data, linesize[0], w, h);
			break;
		case PIX_FMT_BGRX4444:
			convert::ARGBToARGB4444(argb_data, argbStride, yuv_data, linesize[0], w, h);
			break;
		case PIX_FMT_RGB565:
			convert::ARGBToBGR565(argb_data, argbStride, yuv_data, linesize[0], w, h);
			break;
		case PIX_FMT_ABGR8888:
			convert::BGRAToABGR8888(w, h, argb_data, yuv_data);
			break;
		case PIX_FMT_A1B5G5R5:
			convert::ARGBToRGBA1555(argb_data, argbStride, yuv_data, linesize[0], w, h);
			break;
		case PIX_FMT_ABGR4444:
			convert::ARGBToRGBA4444(argb_data, argbStride, yuv_data, linesize[0], w, h);
			break;
		case PIX_FMT_ARGB8888:
			convert::BGRAToARGB8888(w, h, argb_data, yuv_data);
			break;
		case PIX_FMT_A1R5G5B5:
			convert::ARGBToBGRA1555(argb_data, argbStride, yuv_data, linesize[0], w, h);
			break;
		case PIX_FMT_ARGB4444:
			convert::ARGBToBGRA4444(argb_data, argbStride, yuv_data, linesize[0], w, h);
			break;
		case PIX_FMT_YUYV:
			convert::BGRAToYUYV(w, h, argb_data, yuv_data);
			break;
		case PIX_FMT_UYVY:
			convert::BGRAToUYVY(w, h, argb_data, yuv_data);
			break;
		case PIX_FMT_YV12:
			convert::BGRAToYV12(argb_data, w, h, yuv_data);
			break;
		case PIX_FMT_I420:
			convert::BGRAToYUV420(argb_data, w, h, yuv_data);
			break;
		case PIX_FMT_NV12:
			convert::BGRA8888ToNV12(argb_data, w, h, yuv_data);
			break;
		case PIX_FMT_NV21:
			convert::BGRA8888ToNV21(argb_data, w, h, yuv_data);
			break;
		case PIX_FMT_NV16:
			convert::BGRA8888ToNV16(argb_data, w, h, yuv_data);
			break;
		case PIX_FMT_NV61:
			convert::BGRA8888ToNV61(argb_data, w, h, yuv_data);
			break;
		case PIX_FMT_P010MSB:
			convert::BGRA8888ToP010MSB(argb_data, w, h, yuv_data);
			break;
		case PIX_FMT_P010LSB:
			convert::BGRA8888ToP010LSB(argb_data, w, h, yuv_data);
			break;
		case PIX_FMT_I010:
			convert::BGRAToI010(argb_data, w, h, yuv_data);
			break;
		case PIX_FMT_BGR888Planar:
			convert::BGRAToBGR888P(argb_data, w, h, yuv_data);
			break;
		case PIX_FMT_R10G10B10A2:
			convert::BGRAToR10G10B10A2(w, h, argb_data, yuv_data);
			break;
		case PIX_FMT_RGB888Planar:
			convert::BGRAToRGB888P(argb_data, w, h, yuv_data);
			break;
		case PIX_FMT_RGB24:
			convert::BGRAToRGB24(argb_data, w, h, yuv_data);
			break;
		case PIX_FMT_BGR24:
			convert::BGRAToBGR24(argb_data, w, h, yuv_data);
			break;
		default:
			assert(0 && "format is unsupported.");
			break;
		}
	}
};