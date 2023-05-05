#include <utility>
#include <string>
#include <iostream>
#include "Convert.h"
#include "RowCommon.h"

namespace convert {
	int YUY2ToARGB(const uint8_t* src_yuy2,
		int src_stride_yuy2,
		uint8_t* dst_argb,
		int dst_stride_argb,
		int width,
		int height) {
		int y;
		void (*YUY2ToARGBRow)(const uint8_t * src_yuy2, uint8_t * dst_argb, int width) =
			YUY2ToARGBRow_C;
		if (!src_yuy2 || !dst_argb || width <= 0 || height == 0) {
			return -1;
		}
		// Negative height means invert the image.
		if (height < 0) {
			height = -height;
			src_yuy2 = src_yuy2 + (height - 1) * src_stride_yuy2;
			src_stride_yuy2 = -src_stride_yuy2;
		}
		// Coalesce rows.
		if (src_stride_yuy2 == width * 2 && dst_stride_argb == width * 4) {
			width *= height;
			height = 1;
			src_stride_yuy2 = dst_stride_argb = 0;
		}

		for (y = 0; y < height; ++y) {
			YUY2ToARGBRow(src_yuy2, dst_argb, width);
			src_yuy2 += src_stride_yuy2;
			dst_argb += dst_stride_argb;
		}
		return 0;
	}

	int UYVYToARGB(const uint8_t* src_uyvy,
		int src_stride_uyvy,
		uint8_t* dst_argb,
		int dst_stride_argb,
		int width,
		int height) {
		int y;
		void (*UYVYToARGBRow)(const uint8_t * src_uyvy, uint8_t * dst_argb, int width) =
			UYVYToARGBRow_C;
		if (!src_uyvy || !dst_argb || width <= 0 || height == 0) {
			return -1;
		}
		// Negative height means invert the image.
		if (height < 0) {
			height = -height;
			src_uyvy = src_uyvy + (height - 1) * src_stride_uyvy;
			src_stride_uyvy = -src_stride_uyvy;
		}
		// Coalesce rows.
		if (src_stride_uyvy == width * 2 && dst_stride_argb == width * 4) {
			width *= height;
			height = 1;
			src_stride_uyvy = dst_stride_argb = 0;
		}
		for (y = 0; y < height; ++y) {
			UYVYToARGBRow(src_uyvy, dst_argb, width);
			src_uyvy += src_stride_uyvy;
			dst_argb += dst_stride_argb;
		}
		return 0;
	}

	int I420ToARGB(const uint8_t* src_y,
		int src_stride_y,
		const uint8_t* src_u,
		int src_stride_u,
		const uint8_t* src_v,
		int src_stride_v,
		uint8_t* dst_argb,
		int dst_stride_argb,
		int width,
		int height) {
		int y;
		void (*I422ToARGBRow)(const uint8_t * y_buf, const uint8_t * u_buf,
			const uint8_t * v_buf, uint8_t * rgb_buf, int width) =
			I422ToARGBRow_C;
		if (!src_y || !src_u || !src_v || !dst_argb || width <= 0 || height == 0) {
			return -1;
		}
		// Negative height means invert the image.
		if (height < 0) {
			height = -height;
			dst_argb = dst_argb + (height - 1) * dst_stride_argb;
			dst_stride_argb = -dst_stride_argb;
		}

		for (y = 0; y < height; ++y) {
			I422ToARGBRow(src_y, src_u, src_v, dst_argb, width);
			dst_argb += dst_stride_argb;
			src_y += src_stride_y;
			if (y & 1) {
				src_u += src_stride_u;
				src_v += src_stride_v;
			}
		}


		return 0;
	}

	int NV12ToARGB(const uint8_t* src_y,
		int src_stride_y,
		const uint8_t* src_uv,
		int src_stride_uv,
		uint8_t* dst_argb,
		int dst_stride_argb,
		int width,
		int height) {
		int y;
		void (*NV12ToARGBRow)(
			const uint8_t * y_buf, const uint8_t * uv_buf, uint8_t * rgb_buf, int width) = NV12ToARGBRow_C;
		if (!src_y || !src_uv || !dst_argb || width <= 0 || height == 0) {
			return -1;
		}
		// Negative height means invert the image.
		if (height < 0) {
			height = -height;
			dst_argb = dst_argb + (height - 1) * dst_stride_argb;
			dst_stride_argb = -dst_stride_argb;
		}

		for (y = 0; y < height; ++y) {
			NV12ToARGBRow(src_y, src_uv, dst_argb, width);
			dst_argb += dst_stride_argb;
			src_y += src_stride_y;
			if (y & 1) {
				src_uv += src_stride_uv;
			}
		}
		return 0;
	}

	int NV21ToARGB(const uint8_t* src_y,
		int src_stride_y,
		const uint8_t* src_vu,
		int src_stride_vu,
		uint8_t* dst_argb,
		int dst_stride_argb,
		int width,
		int height) {
		int y;
		void (*NV21ToARGBRow)(
			const uint8_t * y_buf, const uint8_t * uv_buf, uint8_t * rgb_buf, int width) = NV21ToARGBRow_C;
		if (!src_y || !src_vu || !dst_argb || width <= 0 || height == 0) {
			return -1;
		}
		// Negative height means invert the image.
		if (height < 0) {
			height = -height;
			dst_argb = dst_argb + (height - 1) * dst_stride_argb;
			dst_stride_argb = -dst_stride_argb;
		}

		for (y = 0; y < height; ++y) {
			NV21ToARGBRow(src_y, src_vu, dst_argb, width);
			dst_argb += dst_stride_argb;
			src_y += src_stride_y;
			if (y & 1) {
				src_vu += src_stride_vu;
			}
		}
		return 0;
	}

	int NV16ToARGB(const uint8_t* src_y,
		int src_stride_y,
		const uint8_t* src_uv,
		int src_stride_uv,
		uint8_t* dst_argb,
		int dst_stride_argb,
		int width,
		int height) {
		int y;
		if (!src_y || !src_uv || !dst_argb || width <= 0 || height == 0) {
			return -1;
		}
		// Negative height means invert the image.
		if (height < 0) {
			height = -height;
			dst_argb = dst_argb + (height - 1) * dst_stride_argb;
			dst_stride_argb = -dst_stride_argb;
		}

		for (y = 0; y < height; ++y) {
			NV16ToARGBRow_C(src_y, src_uv, dst_argb, width);
			dst_argb += dst_stride_argb;
			src_y += src_stride_y;
			src_uv += src_stride_uv;
		}

		return 0;
	}

	int NV61ToARGB(const uint8_t* src_y,
		int src_stride_y,
		const uint8_t* src_vu,
		int src_stride_vu,
		uint8_t* dst_argb,
		int dst_stride_argb,
		int width,
		int height) {
		int y;
		if (!src_y || !src_vu || !dst_argb || width <= 0 || height == 0) {
			return -1;
		}
		// Negative height means invert the image.
		if (height < 0) {
			height = -height;
			dst_argb = dst_argb + (height - 1) * dst_stride_argb;
			dst_stride_argb = -dst_stride_argb;
		}

		for (y = 0; y < height; ++y) {
			NV61ToARGBRow_C(src_y, src_vu, dst_argb, width);
			dst_argb += dst_stride_argb;
			src_y += src_stride_y;
			src_vu += src_stride_vu;
		}

		return 0;
	}

	int P010MSBToARGB(const uint16_t* src_y,
		int src_stride_y,
		const uint16_t* src_uv,
		int src_stride_uv,
		uint8_t* dst_argb,
		int dst_stride_argb,
		int width,
		int height) {
		int y;
		void (*P210ToARGBRow)(
			const uint16_t * y_buf, const uint16_t * uv_buf, uint8_t * rgb_buf,
			int width, int shift) = P210ToARGBRow_C;
		if (!src_y || !src_uv || !dst_argb || width <= 0 || height == 0) {
			return -1;
		}
		// Negative height means invert the image.
		if (height < 0) {
			height = -height;
			dst_argb = dst_argb + (height - 1) * dst_stride_argb;
			dst_stride_argb = -dst_stride_argb;
		}

		for (y = 0; y < height; ++y) {
			P210ToARGBRow(src_y, src_uv, dst_argb, width, 8);
			dst_argb += dst_stride_argb;
			src_y += (src_stride_y / 2);
			if (y & 1) {
				src_uv += (src_stride_uv / 2);
			}
		}
		return 0;
	}

	int P010LSBToARGB(const uint16_t* src_y,
		int src_stride_y,
		const uint16_t* src_uv,
		int src_stride_uv,
		uint8_t* dst_argb,
		int dst_stride_argb,
		int width,
		int height) {
		int y;
		void (*P210ToARGBRow)(
			const uint16_t * y_buf, const uint16_t * uv_buf, uint8_t * rgb_buf,
			int width, int shift) = P210ToARGBRow_C;
		if (!src_y || !src_uv || !dst_argb || width <= 0 || height == 0) {
			return -1;
		}
		// Negative height means invert the image.
		if (height < 0) {
			height = -height;
			dst_argb = dst_argb + (height - 1) * dst_stride_argb;
			dst_stride_argb = -dst_stride_argb;
		}

		for (y = 0; y < height; ++y) {
			P210ToARGBRow(src_y, src_uv, dst_argb, width, 2);
			dst_argb += dst_stride_argb;
			src_y += (src_stride_y / 2);
			if (y & 1) {
				src_uv += (src_stride_uv / 2);
			}
		}
		return 0;
	}

	int I010ToARGB(const uint16_t* src_y,
		int src_stride_y,
		const uint16_t* src_u,
		int src_stride_u,
		const uint16_t* src_v,
		int src_stride_v,
		uint8_t* dst_argb,
		int dst_stride_argb,
		int width,
		int height) {
		int y;
		void (*I210ToARGBRow)(const uint16_t * y_buf, const uint16_t * u_buf,
			const uint16_t * v_buf, uint8_t * rgb_buf,
			int width, int shift) =
			I210ToARGBRow_C;
		if (!src_y || !src_u || !src_v || !dst_argb || width <= 0 || height == 0) {
			return -1;
		}
		// Negative height means invert the image.
		if (height < 0) {
			height = -height;
			dst_argb = dst_argb + (height - 1) * dst_stride_argb;
			dst_stride_argb = -dst_stride_argb;
		}
		for (y = 0; y < height; ++y) {
			I210ToARGBRow(src_y, src_u, src_v, dst_argb, width, 2);
			dst_argb += dst_stride_argb;
			src_y += (src_stride_y / 2);
			if (y & 1) {
				src_u += (src_stride_u / 2);
				src_v += (src_stride_v / 2);
			}
		}
		return 0;
	}

	void BGRPToBGRA(uint8_t* bgrp, uint8_t* argb, int w, int h)
	{
		uint8_t* b, * g, * r;
		b = bgrp;
		g = bgrp + w * h;
		r = bgrp + w * h * 2;
		for (int y = 0; y < h; y++)
		{
			for (int x = 0; x < w; x++)
			{
				argb[3] = 255;
				argb[2] = r[0];
				argb[1] = g[0];
				argb[0] = b[0];
				r++;
				g++;
				b++;
				argb += 4;
			}
		}
	}


	static void converseTiled2Linear(void* tiledBuf, void* linearBuf, uint32_t step, uint32_t planeWidth, uint32_t planeHeight, uint32_t tiledWH)
	{
		uint8_t* pTiled, * pLinear;
		uint32_t offset = 0, i, x = 0, y = 0, lineSize, totalSize, copy_size;

		lineSize = planeWidth * step;
		totalSize = lineSize * planeHeight;
		copy_size = step * tiledWH;

		while (offset < totalSize)
		{
			pTiled = (uint8_t*)tiledBuf + offset;
			pLinear = (uint8_t*)linearBuf + y * lineSize + x * step;

			for (i = 0; i < tiledWH; i++)
			{
				memcpy(pLinear + i * lineSize, pTiled, copy_size);
				pTiled += copy_size;
				offset += copy_size;
			}

			x += tiledWH;
			if (x >= planeWidth) {
				y += tiledWH;
				x = 0;
			}
		}
	}

	void ConvertTiled(int w, int h, enum Tiled tiled, enum PixelFormat pix_fmt, uint8_t* buffer, uint8_t* tiledBuffer)
	{
		uint8_t* data[4];
		int linesize[4];
		uint8_t* dataTiled[4];
		const PixFmtDescriptor* pixDesc = pix_fmt_desc_get(pix_fmt);
		const TiledDescriptor* tiledDesc = tiled_desc_get(tiled);
		const int align = tiledDesc->align;

		image_fill_arrays(data, linesize, (const uint8_t*)buffer, pix_fmt, w, h, 1);
		image_fill_arrays(dataTiled, linesize, (const uint8_t*)tiledBuffer, pix_fmt, w, h, 1);

		for (int i = 0; i < pix_fmt_count_planes(pix_fmt); i++)
		{
			int height = h;
			if (i == 1 || i == 2)
			{
				height = h >> pixDesc->log2_chroma_h;
			}
			converseTiled2Linear(data[i], dataTiled[i], pixDesc->comp[i].step, linesize[i] / pixDesc->comp[i].step, height, align);
		}
	}

	void R5G5B5A1ToB5G5R5A1(int w, int h, uint8_t* buffer)
	{
		uint16_t* p = (uint16_t*)buffer;

		for (int i = 0; i < w * h; i++) {
			uint16_t color = *p;
			uint16_t r = (color >> 0) & 0x1f;
			uint16_t g = (color >> 5) & 0x1f;
			uint16_t b = (color >> 10) & 0x1f;
			uint16_t a = (color >> 15) & 0x1;
			uint16_t b5g5r5a1 = (b << 0) | (g << 5) | (r << 10) | (a << 15);
			*p = b5g5r5a1;
			p++;
		}
	}

	void B10G10R10A2ToR10G10B10A2(int w, int h, uint8_t* buffer)
	{
		uint32_t* p = (uint32_t*)buffer;

		for (int i = 0; i < w * h; i++) {
			uint32_t color = *p;
			uint32_t b = (color >> 0) & 0x3ff;
			uint32_t g = (color >> 10) & 0x3ff;
			uint32_t r = (color >> 20) & 0x3ff;
			uint32_t a = (color >> 30) & 0x3;
			uint32_t r10g10b10a2 = (r << 0) | (g << 10) | (b << 20) | (a << 30);
			*p = r10g10b10a2;
			p++;
		}
	}

	void BGR565ToBGR565(int w, int h, uint8_t* buffer)
	{
		uint8_t* src_bgr565 = buffer;

		for (int i = 0; i < w * h; i++) {
			uint8_t b = src_bgr565[0] & 0x1f;
			uint8_t g = (src_bgr565[0] >> 5) | ((src_bgr565[1] & 0x07) << 3);
			uint8_t r = src_bgr565[1] >> 3;
			uint16_t b5g5r5 = (r << 0) | (g << 5) | (b << 11);
			*(uint16_t*)src_bgr565 = b5g5r5;
			src_bgr565 += 2;
		}
	}

	void ABGR8888ToB8G8R8A8(int w, int h, uint8_t* buffer)
	{
		uint32_t* p = (uint32_t*)buffer;

		for (int i = 0; i < w * h; i++) {
			uint32_t color = *p;
			uint32_t a = (color >> 0) & 0xff;
			uint32_t b = (color >> 8) & 0xff;
			uint32_t g = (color >> 16) & 0xff;
			uint32_t r = (color >> 24) & 0xff;
			uint32_t bgra8888 = (b << 0) | (g << 8) | (r << 16) | (a << 24);
			*p = bgra8888;
			p++;
		}
	}


	void A1B5G5R5ToB5G5R5A1(int w, int h, uint8_t* buffer)
	{
		uint16_t* p = (uint16_t*)buffer;
		for (int i = 0; i < w * h; i++) {
			uint16_t color = *p;
			uint16_t a = color & 1;
			uint16_t b = (color >> 1) & 0x1f;
			uint16_t g = (color >> 6) & 0x1f;
			uint16_t r = (color >> 11) & 0x1f;
			uint16_t b5g5r5a1 = (b << 0) | (g << 5) | (r << 10) | (a << 15);
			*p = b5g5r5a1;
			p++;
		}
	}

	void A1R5G5B5ToB5G5R5A1(int w, int h, uint8_t* buffer)
	{
		uint16_t* p = (uint16_t*)buffer;
		for (int i = 0; i < w * h; i++) {
			uint16_t color = *p;
			uint16_t a = color & 1;
			uint16_t r = (color >> 1) & 0x1f;
			uint16_t g = (color >> 6) & 0x1f;
			uint16_t b = (color >> 11) & 0x1f;
			uint16_t b5g5r5a1 = (b << 0) | (g << 5) | (r << 10) | (a << 15);
			*p = b5g5r5a1;
			p++;
		}
	}

	int ARGB1555ToARGB(const uint8_t* src_argb1555,
		int src_stride_argb1555,
		uint8_t* dst_argb,
		int dst_stride_argb,
		int width,
		int height) {
		int y;
		void (*ARGB1555ToARGBRow)(const uint8_t * src_argb1555, uint8_t * dst_argb,
			int width) = ARGB1555ToARGBRow_C;
		if (!src_argb1555 || !dst_argb || width <= 0 || height == 0) {
			return -1;
		}
		// Negative height means invert the image.
		if (height < 0) {
			height = -height;
			src_argb1555 = src_argb1555 + (height - 1) * src_stride_argb1555;
			src_stride_argb1555 = -src_stride_argb1555;
		}
		// Coalesce rows.
		if (src_stride_argb1555 == width * 2 && dst_stride_argb == width * 4) {
			width *= height;
			height = 1;
			src_stride_argb1555 = dst_stride_argb = 0;
		}

		for (y = 0; y < height; ++y) {
			ARGB1555ToARGBRow(src_argb1555, dst_argb, width);
			src_argb1555 += src_stride_argb1555;
			dst_argb += dst_stride_argb;
		}
		return 0;
	}

	void ARGB8888ToB8G8R8A8(int w, int h, uint8_t* buffer)
	{
		uint32_t* p = (uint32_t*)buffer;

		for (int i = 0; i < w * h; i++) {
			uint32_t color = *p;
			uint32_t a = (color >> 0) & 0xff;
			uint32_t r = (color >> 8) & 0xff;
			uint32_t g = (color >> 16) & 0xff;
			uint32_t b = (color >> 24) & 0xff;
			uint32_t bgra8888 = (b << 0) | (g << 8) | (r << 16) | (a << 24);
			*p = bgra8888;
			p++;
		}
	}

	void ABGR4444ToBGRA(uint8_t* buffer, int w, int h, uint8_t* argb_data)
	{

		//memcpy(argb_data, buffer, w * h * 2);
		//return;
		uint32_t bgra8888 = 0;
		uint16_t* p4 = (uint16_t*)buffer;
		uint32_t* p8 = (uint32_t*)argb_data;
		for (int i = 0; i < w * h; i++) {
			uint16_t color = *p4;

			uint16_t a = (color >> 0) & 0xf;
			uint16_t b = (color >> 4) & 0xf;
			uint16_t g = (color >> 8) & 0xf;
			uint16_t r = (color >> 12) & 0xf;
			bgra8888 = (b << 0) | (g << 8) | (r << 16) | (a << 24);
			*p8 = bgra8888;
			bgra8888 = 0;
			p8++;
			p4++;
		}
	}

	int ABGR4444ToARGB(const uint8_t* src_argb4444,
		int src_stride_argb4444,
		uint8_t* dst_argb,
		int dst_stride_argb,
		int width,
		int height) {
		int y;
		void (*ABGR4444ToARGBRow)(const uint8_t * src_argb4444, uint8_t * dst_argb,
			int width) = ABGR4444ToARGBRow_C;
		if (!src_argb4444 || !dst_argb || width <= 0 || height == 0) {
			return -1;
		}
		// Negative height means invert the image.
		if (height < 0) {
			height = -height;
			src_argb4444 = src_argb4444 + (height - 1) * src_stride_argb4444;
			src_stride_argb4444 = -src_stride_argb4444;
		}
		// Coalesce rows.
		if (src_stride_argb4444 == width * 2 && dst_stride_argb == width * 4) {
			width *= height;
			height = 1;
			src_stride_argb4444 = dst_stride_argb = 0;
		}

		for (y = 0; y < height; ++y) {
			ABGR4444ToARGBRow(src_argb4444, dst_argb, width);
			src_argb4444 += src_stride_argb4444;
			dst_argb += dst_stride_argb;
		}
		return 0;
	}

	int ARGB4444ToARGB(const uint8_t* src_argb4444,
		int src_stride_argb4444,
		uint8_t* dst_argb,
		int dst_stride_argb,
		int width,
		int height) {
		int y;
		void (*ARGB4444ToARGBRow)(const uint8_t * src_argb4444, uint8_t * dst_argb,
			int width) = ARGB4444ToARGBRow_C;
		if (!src_argb4444 || !dst_argb || width <= 0 || height == 0) {
			return -1;
		}
		// Negative height means invert the image.
		if (height < 0) {
			height = -height;
			src_argb4444 = src_argb4444 + (height - 1) * src_stride_argb4444;
			src_stride_argb4444 = -src_stride_argb4444;
		}
		// Coalesce rows.
		if (src_stride_argb4444 == width * 2 && dst_stride_argb == width * 4) {
			width *= height;
			height = 1;
			src_stride_argb4444 = dst_stride_argb = 0;
		}

		for (y = 0; y < height; ++y) {
			ARGB4444ToARGBRow(src_argb4444, dst_argb, width);
			src_argb4444 += src_stride_argb4444;
			dst_argb += dst_stride_argb;
		}
		return 0;
	}

	int BGRA4444ToARGB(const uint8_t* src_argb4444,
		int src_stride_argb4444,
		uint8_t* dst_argb,
		int dst_stride_argb,
		int width,
		int height) {
		int y;
		void (*BGRA4444ToBGRARow)(const uint8_t * src_argb4444, uint8_t * dst_argb,
			int width) = BGRA4444ToARGBRow_C;
		if (!src_argb4444 || !dst_argb || width <= 0 || height == 0) {
			return -1;
		}
		// Negative height means invert the image.
		if (height < 0) {
			height = -height;
			src_argb4444 = src_argb4444 + (height - 1) * src_stride_argb4444;
			src_stride_argb4444 = -src_stride_argb4444;
		}
		// Coalesce rows.
		if (src_stride_argb4444 == width * 2 && dst_stride_argb == width * 4) {
			width *= height;
			height = 1;
			src_stride_argb4444 = dst_stride_argb = 0;
		}

		for (y = 0; y < height; ++y) {
			BGRA4444ToBGRARow(src_argb4444, dst_argb, width);
			src_argb4444 += src_stride_argb4444;
			dst_argb += dst_stride_argb;
		}
		return 0;
	}

	int RGBA4444ToARGB(const uint8_t* src_argb4444,
		int src_stride_argb4444,
		uint8_t* dst_argb,
		int dst_stride_argb,
		int width,
		int height) {
		int y;
		void (*RGBA4444ToBGRARow)(const uint8_t * src_argb4444, uint8_t * dst_argb,
			int width) = RGBA4444ToARGBRow_C;
		if (!src_argb4444 || !dst_argb || width <= 0 || height == 0) {
			return -1;
		}
		// Negative height means invert the image.
		if (height < 0) {
			height = -height;
			src_argb4444 = src_argb4444 + (height - 1) * src_stride_argb4444;
			src_stride_argb4444 = -src_stride_argb4444;
		}
		// Coalesce rows.
		if (src_stride_argb4444 == width * 2 && dst_stride_argb == width * 4) {
			width *= height;
			height = 1;
			src_stride_argb4444 = dst_stride_argb = 0;
		}

		for (y = 0; y < height; ++y) {
			RGBA4444ToBGRARow(src_argb4444, dst_argb, width);
			src_argb4444 += src_stride_argb4444;
			dst_argb += dst_stride_argb;
		}
		return 0;
	}
};

