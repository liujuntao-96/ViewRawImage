#include <assert.h>
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
		case PIX_FMT_A1R5G5B5:
			convert::ARGB1555ToARGB(data[0], linesize[0], argb_data, argbStride, w, h);
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
		default:
			assert(0 && "format is unsupported.");
			break;
		}
	}
};