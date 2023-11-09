#include <utility>
#include <string>
#include <iostream>
#include "Convert.h"
#include "RowCommon.h"

#define align_buffer_64(var, size)                                           \
  uint8_t* var##_mem = (uint8_t*)(malloc((size) + 63));         /* NOLINT */ \
  uint8_t* var = (uint8_t*)(((intptr_t)(var##_mem) + 63) & ~63) /* NOLINT */

#define free_aligned_buffer_64(var) \
  free(var##_mem);                  \
  var = 0

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

	void RGB24ToBGRA(uint8_t* bgrp, uint8_t* argb, int w, int h)
	{
		for (int y = 0; y < h; y++)
		{
			for (int x = 0; x < w; x++)
			{
				argb[3] = 255;
				argb[2] = bgrp[0];
				argb[1] = bgrp[1];
				argb[0] = bgrp[2];
				bgrp += 3;
				argb += 4;
			}
		}
	}

	void RGBPToBGRA(uint8_t* bgrp, uint8_t* argb, int w, int h)
	{
		uint8_t* b, * g, * r;
		r = bgrp;
		g = bgrp + w * h;
		b = bgrp + w * h * 2;
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

	void BGR24ToBGRA(uint8_t* bgrp, uint8_t* argb, int w, int h)
	{
		for (int y = 0; y < h; y++)
		{
			for (int x = 0; x < w; x++)
			{
				argb[3] = 255;
				argb[2] = bgrp[2];
				argb[1] = bgrp[1];
				argb[0] = bgrp[0];
				bgrp += 3;
				argb += 4;
			}
		}
	}


	static void converseLinear2Tiled(void* tiledBuf, void* linearBuf, uint32_t step, uint32_t planeWidth, uint32_t planeHeight, uint32_t tiledWH, int copy_size)
	{
		uint8_t* plinear = (uint8_t*)linearBuf;
		uint8_t* ptiled = (uint8_t*)tiledBuf;
		int h = planeHeight;
		int w = planeWidth * step;
		int a = tiledWH;

		for (int y = 0; y < h; y += a)
		{
			for (int x = 0; x < w; x += copy_size)
			{
				for (int m = 0; m < a; m++)
				{
					plinear = (uint8_t*)linearBuf + (((w * (y + m)) + x));
					memcpy(ptiled, plinear, copy_size);
					ptiled += copy_size;
				}
			}
		}

	}

	void ConvertFromTiled(int w, int h, enum Tiled tiled, enum PixelFormat pix_fmt, uint8_t* buffer, uint8_t* tiledBuffer)
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
			converseLinear2Tiled(dataTiled[i], data[i], pixDesc->comp[i].step, linesize[i] / pixDesc->comp[i].step, height, align, pixDesc->comp[0].step * align);
		}
	}

	static void converseTiled2Linear(void* tiledBuf, void* linearBuf, uint32_t step, uint32_t planeWidth, uint32_t planeHeight, uint32_t tiledWH, int copy_size)
	{
		uint8_t* pTiled, * pLinear;
		uint32_t offset = 0, i, x = 0, y = 0, lineSize, totalSize;

		lineSize = planeWidth * step;
		totalSize = lineSize * planeHeight;

		while (offset < totalSize)
		{
			pTiled = (uint8_t*)tiledBuf + offset;
			pLinear = (uint8_t*)linearBuf + y * lineSize + x;

			for (i = 0; i < tiledWH; i++)
			{
				memcpy(pLinear + i * lineSize, pTiled, copy_size);
				pTiled += copy_size;
				offset += copy_size;
			}

			x += copy_size;
			if (x >= lineSize) {
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
			converseTiled2Linear(data[i], dataTiled[i], pixDesc->comp[i].step, linesize[i] / pixDesc->comp[i].step, height, align, pixDesc->comp[0].step * align);
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

	int ABGR1555ToARGB(const uint8_t* src_argb1555,
		int src_stride_argb1555,
		uint8_t* dst_argb,
		int dst_stride_argb,
		int width,
		int height) {
		int y;
		void (*ABGR1555ToARGBRow)(const uint8_t * src_argb1555, uint8_t * dst_argb,
			int width) = ABGR1555ToARGBRow_C;
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
			ABGR1555ToARGBRow(src_argb1555, dst_argb, width);
			src_argb1555 += src_stride_argb1555;
			dst_argb += dst_stride_argb;
		}
		return 0;
	}
	int RGBA5551ToARGB(const uint8_t* src_argb1555,
		int src_stride_argb1555,
		uint8_t* dst_argb,
		int dst_stride_argb,
		int width,
		int height) {
		int y;
		void (*RGBA5551ToARGBRow)(const uint8_t * src_argb1555, uint8_t * dst_argb,
			int width) = RGBA5551ToARGBRow_C;
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
			RGBA5551ToARGBRow(src_argb1555, dst_argb, width);
			src_argb1555 += src_stride_argb1555;
			dst_argb += dst_stride_argb;
		}
		return 0;
	}

	int BGRA5551ToARGB(const uint8_t* src_argb1555,
		int src_stride_argb1555,
		uint8_t* dst_argb,
		int dst_stride_argb,
		int width,
		int height) {
		int y;
		void (*BGRA5551ToARGBRow)(const uint8_t * src_argb1555, uint8_t * dst_argb,
			int width) = BGRA5551ToARGBRow_C;
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
			BGRA5551ToARGBRow(src_argb1555, dst_argb, width);
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

	void RGBA8888ToARGB(int w, int h, uint8_t* buffer)
	{
		uint32_t* p = (uint32_t*)buffer;

		for (int i = 0; i < w * h; i++) {
			uint32_t color = *p;
			uint32_t r = (color >> 0) & 0xff;
			uint32_t g = (color >> 8) & 0xff;
			uint32_t b = (color >> 16) & 0xff;
			uint32_t a = (color >> 24) & 0xff;
			uint32_t bgra8888 = (b << 0) | (g << 8) | (r << 16) | (a << 24);
			*p = bgra8888;
			p++;
		}
	}

	void ABGR8888ToARGB(int w, int h, uint8_t* buffer)
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

	void ARGB8888ToARGB(int w, int h, uint8_t* buffer)
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

	void BGRA8888ToRGBA(int w, int h, uint8_t* buffer)
	{
		uint32_t* p = (uint32_t*)buffer;

		for (int i = 0; i < w * h; i++) {
			uint32_t color = *p;
			uint32_t b = (color >> 0) & 0xff;
			uint32_t g = (color >> 8) & 0xff;
			uint32_t r = (color >> 16) & 0xff;
			uint32_t a = (color >> 24) & 0xff;
			uint32_t bgra8888 = (r << 0) | (g << 8) | (b << 16) | (a << 24);
			*p = bgra8888;
			p++;
		}
	}

	void B10G10R10A2ToARGB(int w, int h, uint8_t* buffer, uint8_t* argb)
	{
		uint32_t* p = (uint32_t*)buffer;
		uint32_t* dst = (uint32_t*)argb;
		for (int i = 0; i < w * h; i++) {
			uint32_t color = *p;
			uint8_t b = ((color >> 0) & 0x3ff) >> 2;
			uint8_t g = ((color >> 10) & 0x3ff) >> 2;
			uint8_t r = ((color >> 20) & 0x3ff) >> 2;
			uint8_t a = (color >> 30) & 0x3;
			uint32_t bgra8888 = (b << 0) | (g << 8) | (r << 16) | ((a * 85) << 24);
			*dst = bgra8888;
			p++;
			dst++;
		}
	}

	void R10G10B10A2ToARGB(int w, int h, uint8_t* buffer, uint8_t* argb)
	{
		uint32_t* p = (uint32_t*)buffer;
		uint32_t* dst = (uint32_t*)argb;
		for (int i = 0; i < w * h; i++) {
			uint32_t color = *p;
			uint8_t r = ((color >> 0) & 0x3ff) >> 2;
			uint8_t g = ((color >> 10) & 0x3ff) >> 2;
			uint8_t b = ((color >> 20) & 0x3ff) >> 2;
			uint8_t a = (color >> 30) & 0x3;
			uint32_t bgra8888 = (b << 0) | (g << 8) | (r << 16) | ((a * 85) << 24);
			*dst = bgra8888;
			p++;
			dst++;
		}
	}


	void ABGR4444ToBGRA(uint8_t* buffer, int w, int h, uint8_t* argb_data)
	{
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

	int RGB565ToARGB(const uint8_t* src_rgb565,
		int src_stride_rgb565,
		uint8_t* dst_argb,
		int dst_stride_argb,
		int width,
		int height) {
		int y;
		void (*RGB565ToARGBRow)(const uint8_t * src_rgb565, uint8_t * dst_argb,
			int width) = RGB565ToARGBRow_C;
		if (!src_rgb565 || !dst_argb || width <= 0 || height == 0) {
			return -1;
		}
		// Negative height means invert the image.
		if (height < 0) {
			height = -height;
			src_rgb565 = src_rgb565 + (height - 1) * src_stride_rgb565;
			src_stride_rgb565 = -src_stride_rgb565;
		}
		// Coalesce rows.
		if (src_stride_rgb565 == width * 2 && dst_stride_argb == width * 4) {
			width *= height;
			height = 1;
			src_stride_rgb565 = dst_stride_argb = 0;
		}

		for (y = 0; y < height; ++y) {
			RGB565ToARGBRow(src_rgb565, dst_argb, width);
			src_rgb565 += src_stride_rgb565;
			dst_argb += dst_stride_argb;
		}
		return 0;
	}

	int BGR565ToARGB(const uint8_t* src_rgb565,
		int src_stride_rgb565,
		uint8_t* dst_argb,
		int dst_stride_argb,
		int width,
		int height) {
		int y;
		void (*BGR565ToARGBRow)(const uint8_t * src_rgb565, uint8_t * dst_argb,
			int width) = BGR565ToARGBRow_C;
		if (!src_rgb565 || !dst_argb || width <= 0 || height == 0) {
			return -1;
		}
		// Negative height means invert the image.
		if (height < 0) {
			height = -height;
			src_rgb565 = src_rgb565 + (height - 1) * src_stride_rgb565;
			src_stride_rgb565 = -src_stride_rgb565;
		}
		// Coalesce rows.
		if (src_stride_rgb565 == width * 2 && dst_stride_argb == width * 4) {
			width *= height;
			height = 1;
			src_stride_rgb565 = dst_stride_argb = 0;
		}

		for (y = 0; y < height; ++y) {
			BGR565ToARGBRow(src_rgb565, dst_argb, width);
			src_rgb565 += src_stride_rgb565;
			dst_argb += dst_stride_argb;
		}
		return 0;
	}


	int ARGBToARGB1555(const uint8_t* src_argb,
		int src_stride_argb,
		uint8_t* dst_argb1555,
		int dst_stride_argb1555,
		int width,
		int height) {
		int y;
		void (*ARGBToARGB1555Row)(const uint8_t * src_argb, uint8_t * dst_rgb,
			int width) = ARGBToARGB1555Row_C;
		if (!src_argb || !dst_argb1555 || width <= 0 || height == 0) {
			return -1;
		}
		if (height < 0) {
			height = -height;
			src_argb = src_argb + (height - 1) * src_stride_argb;
			src_stride_argb = -src_stride_argb;
		}
		// Coalesce rows.
		if (src_stride_argb == width * 4 && dst_stride_argb1555 == width * 2) {
			width *= height;
			height = 1;
			src_stride_argb = dst_stride_argb1555 = 0;
		}

		for (y = 0; y < height; ++y) {
			ARGBToARGB1555Row(src_argb, dst_argb1555, width);
			src_argb += src_stride_argb;
			dst_argb1555 += dst_stride_argb1555;
		}
		return 0;
	}

	int ARGBToABGR1555(const uint8_t* src_argb,
		int src_stride_argb,
		uint8_t* dst_argb1555,
		int dst_stride_argb1555,
		int width,
		int height) {
		int y;
		void (*ARGBToABGR1555Row)(const uint8_t * src_argb, uint8_t * dst_rgb,
			int width) = ARGBToABGR1555Row_C;
		if (!src_argb || !dst_argb1555 || width <= 0 || height == 0) {
			return -1;
		}
		if (height < 0) {
			height = -height;
			src_argb = src_argb + (height - 1) * src_stride_argb;
			src_stride_argb = -src_stride_argb;
		}
		//// Coalesce rows.
		//if (src_stride_argb == width * 4 && dst_stride_argb1555 == width * 2) {
		//	width *= height;
		//	height = 1;
		//	src_stride_argb = dst_stride_argb1555 = 0;
		//}

		for (y = 0; y < height; ++y) {
			ARGBToABGR1555Row(src_argb, dst_argb1555, width);
			src_argb += src_stride_argb;
			dst_argb1555 += dst_stride_argb1555;
		}
		return 0;
	}

	void BGRAToRGBA1555(uint8_t* argb_data, uint8_t* yuv_data, int w, int h)
	{
		uint16_t rgba8888 = 0;
		uint16_t* p = (uint16_t*)yuv_data;
		uint32_t* p8 = (uint32_t*)argb_data;
		for (int i = 0; i < w * h; i++) {
			uint32_t color = *p8;

			uint8_t b = (color >> 3) & 0x1f;
			uint8_t g = (color >> 11) & 0x1f;
			uint8_t r = (color >> 19) & 0x1f;
			uint8_t a = (color >> 31) & 0x1;
			rgba8888 = r | (g << 5) | (b << 10) | (a << 15);
			*p = rgba8888;
			p8++;
			p++;
		}
	}

	int ARGBToBGRA1555(const uint8_t* src_argb,
		int src_stride_argb,
		uint8_t* dst_argb1555,
		int dst_stride_argb1555,
		int width,
		int height) {
		int y;
		void (*ARGBToBGRA1555Row)(const uint8_t * src_argb, uint8_t * dst_rgb,
			int width) = ARGBToBGRA1555Row_C;
		if (!src_argb || !dst_argb1555 || width <= 0 || height == 0) {
			return -1;
		}
		if (height < 0) {
			height = -height;
			src_argb = src_argb + (height - 1) * src_stride_argb;
			src_stride_argb = -src_stride_argb;
		}
		// Coalesce rows.
		if (src_stride_argb == width * 4 && dst_stride_argb1555 == width * 2) {
			width *= height;
			height = 1;
			src_stride_argb = dst_stride_argb1555 = 0;
		}

		for (y = 0; y < height; ++y) {
			ARGBToBGRA1555Row(src_argb, dst_argb1555, width);
			src_argb += src_stride_argb;
			dst_argb1555 += dst_stride_argb1555;
		}
		return 0;
	}

	int ARGBToRGBA1555(const uint8_t* src_argb,
		int src_stride_argb,
		uint8_t* dst_argb1555,
		int dst_stride_argb1555,
		int width,
		int height) {
		int y;
		void (*ARGBToRGBA1555Row)(const uint8_t * src_argb, uint8_t * dst_rgb,
			int width) = ARGBToRGBA1555Row_C;
		if (!src_argb || !dst_argb1555 || width <= 0 || height == 0) {
			return -1;
		}
		if (height < 0) {
			height = -height;
			src_argb = src_argb + (height - 1) * src_stride_argb;
			src_stride_argb = -src_stride_argb;
		}
		// Coalesce rows.
		if (src_stride_argb == width * 4 && dst_stride_argb1555 == width * 2) {
			width *= height;
			height = 1;
			src_stride_argb = dst_stride_argb1555 = 0;
		}

		for (y = 0; y < height; ++y) {
			ARGBToRGBA1555Row(src_argb, dst_argb1555, width);
			src_argb += src_stride_argb;
			dst_argb1555 += dst_stride_argb1555;
		}
		return 0;
	}

	int ARGBToARGB4444(const uint8_t* src_argb,
		int src_stride_argb,
		uint8_t* dst_argb4444,
		int dst_stride_argb4444,
		int width,
		int height) {
		int y;
		void (*ARGBToARGB4444Row)(const uint8_t * src_argb, uint8_t * dst_rgb,
			int width) = ARGBToARGB4444Row_C;
		if (!src_argb || !dst_argb4444 || width <= 0 || height == 0) {
			return -1;
		}
		if (height < 0) {
			height = -height;
			src_argb = src_argb + (height - 1) * src_stride_argb;
			src_stride_argb = -src_stride_argb;
		}
		// Coalesce rows.
		if (src_stride_argb == width * 4 && dst_stride_argb4444 == width * 2) {
			width *= height;
			height = 1;
			src_stride_argb = dst_stride_argb4444 = 0;
		}

		for (y = 0; y < height; ++y) {
			ARGBToARGB4444Row(src_argb, dst_argb4444, width);
			src_argb += src_stride_argb;
			dst_argb4444 += dst_stride_argb4444;
		}
		return 0;
	}

	int ARGBToABGR4444(const uint8_t* src_argb,
		int src_stride_argb,
		uint8_t* dst_argb4444,
		int dst_stride_argb4444,
		int width,
		int height) {
		int y;
		void (*ARGBToABGR4444Row)(const uint8_t * src_argb, uint8_t * dst_rgb,
			int width) = ARGBToABGR4444Row_C;
		if (!src_argb || !dst_argb4444 || width <= 0 || height == 0) {
			return -1;
		}
		if (height < 0) {
			height = -height;
			src_argb = src_argb + (height - 1) * src_stride_argb;
			src_stride_argb = -src_stride_argb;
		}
		// Coalesce rows.
		if (src_stride_argb == width * 4 && dst_stride_argb4444 == width * 2) {
			width *= height;
			height = 1;
			src_stride_argb = dst_stride_argb4444 = 0;
		}

		for (y = 0; y < height; ++y) {
			ARGBToABGR4444Row(src_argb, dst_argb4444, width);
			src_argb += src_stride_argb;
			dst_argb4444 += dst_stride_argb4444;
		}
		return 0;
	}

	int ARGBToRGBA4444(const uint8_t* src_argb,
		int src_stride_argb,
		uint8_t* dst_argb4444,
		int dst_stride_argb4444,
		int width,
		int height) {
		int y;
		void (*ARGBToRGBA4444Row)(const uint8_t * src_argb, uint8_t * dst_rgb,
			int width) = ARGBToRGBA4444Row_C;
		if (!src_argb || !dst_argb4444 || width <= 0 || height == 0) {
			return -1;
		}
		if (height < 0) {
			height = -height;
			src_argb = src_argb + (height - 1) * src_stride_argb;
			src_stride_argb = -src_stride_argb;
		}
		// Coalesce rows.
		if (src_stride_argb == width * 4 && dst_stride_argb4444 == width * 2) {
			width *= height;
			height = 1;
			src_stride_argb = dst_stride_argb4444 = 0;
		}

		for (y = 0; y < height; ++y) {
			ARGBToRGBA4444Row(src_argb, dst_argb4444, width);
			src_argb += src_stride_argb;
			dst_argb4444 += dst_stride_argb4444;
		}
		return 0;
	}

	int ARGBToBGRA4444(const uint8_t* src_argb,
		int src_stride_argb,
		uint8_t* dst_argb4444,
		int dst_stride_argb4444,
		int width,
		int height) {
		int y;
		void (*ARGBToBGRA4444Row)(const uint8_t * src_argb, uint8_t * dst_rgb,
			int width) = ARGBToBGRA4444Row_C;
		if (!src_argb || !dst_argb4444 || width <= 0 || height == 0) {
			return -1;
		}
		if (height < 0) {
			height = -height;
			src_argb = src_argb + (height - 1) * src_stride_argb;
			src_stride_argb = -src_stride_argb;
		}
		// Coalesce rows.
		if (src_stride_argb == width * 4 && dst_stride_argb4444 == width * 2) {
			width *= height;
			height = 1;
			src_stride_argb = dst_stride_argb4444 = 0;
		}

		for (y = 0; y < height; ++y) {
			ARGBToBGRA4444Row(src_argb, dst_argb4444, width);
			src_argb += src_stride_argb;
			dst_argb4444 += dst_stride_argb4444;
		}
		return 0;
	}

	int ARGBToRGB565(const uint8_t* src_argb,
		int src_stride_argb,
		uint8_t* dst_rgb565,
		int dst_stride_rgb565,
		int width,
		int height) {
		int y;
		void (*ARGBToRGB565Row)(const uint8_t * src_argb, uint8_t * dst_rgb,
			int width) = ARGBToRGB565Row_C;
		if (!src_argb || !dst_rgb565 || width <= 0 || height == 0) {
			return -1;
		}
		if (height < 0) {
			height = -height;
			src_argb = src_argb + (height - 1) * src_stride_argb;
			src_stride_argb = -src_stride_argb;
		}
		// Coalesce rows.
		if (src_stride_argb == width * 4 && dst_stride_rgb565 == width * 2) {
			width *= height;
			height = 1;
			src_stride_argb = dst_stride_rgb565 = 0;
		}

		for (y = 0; y < height; ++y) {
			ARGBToRGB565Row(src_argb, dst_rgb565, width);
			src_argb += src_stride_argb;
			dst_rgb565 += dst_stride_rgb565;
		}
		return 0;
	}

	int ARGBToBGR565(const uint8_t* src_argb,
		int src_stride_argb,
		uint8_t* dst_rgb565,
		int dst_stride_rgb565,
		int width,
		int height) {
		int y;
		void (*ARGBToBGR565Row)(const uint8_t * src_argb, uint8_t * dst_rgb,
			int width) = ARGBToBGR565Row_C;
		if (!src_argb || !dst_rgb565 || width <= 0 || height == 0) {
			return -1;
		}
		if (height < 0) {
			height = -height;
			src_argb = src_argb + (height - 1) * src_stride_argb;
			src_stride_argb = -src_stride_argb;
		}
		// Coalesce rows.
		if (src_stride_argb == width * 4 && dst_stride_rgb565 == width * 2) {
			width *= height;
			height = 1;
			src_stride_argb = dst_stride_rgb565 = 0;
		}

		for (y = 0; y < height; ++y) {
			ARGBToBGR565Row(src_argb, dst_rgb565, width);
			src_argb += src_stride_argb;
			dst_rgb565 += dst_stride_rgb565;
		}
		return 0;
	}

	void BGRAToB10G10R10A2(int w, int h, uint8_t* argb_data, uint8_t* yuv_data)
	{
		uint32_t bgra8888 = 0;
		uint32_t* p10 = (uint32_t*)yuv_data;
		uint32_t* p8 = (uint32_t*)argb_data;
		for (int i = 0; i < w * h; i++) {
			uint32_t color = *p8;

			uint32_t b = (color >> 0) & 0xff;
			uint32_t g = (color >> 8) & 0xff;
			uint32_t r = (color >> 16) & 0xff;
			uint32_t a = (color >> 24) & 0xff;
			bgra8888 = (b << 2) | (g << 12) | (r << 22) | a << 30;

			*p10 = bgra8888;
			p8++;
			p10++;
		}
	}

	void BGRAToR10G10B10A2(int w, int h, uint8_t* argb_data, uint8_t* yuv_data)
	{
		uint32_t bgra8888 = 0;
		uint32_t* p10 = (uint32_t*)yuv_data;
		uint32_t* p8 = (uint32_t*)argb_data;
		for (int i = 0; i < w * h; i++) {
			uint32_t color = *p8;

			uint32_t b = (color >> 0) & 0xff;
			uint32_t g = (color >> 8) & 0xff;
			uint32_t r = (color >> 16) & 0xff;
			uint32_t a = (color >> 24) & 0xff;
			bgra8888 = (r << 2) | (g << 12) | (b << 22) | a << 30;
			*p10 = bgra8888;
			p8++;
			p10++;
		}
	}

	void BGRAToABGR8888(int w, int h, uint8_t* argb_data, uint8_t* yuv_data)
	{
		uint32_t bgra8888 = 0;
		uint32_t* p = (uint32_t*)yuv_data;
		uint32_t* p8 = (uint32_t*)argb_data;
		for (int i = 0; i < w * h; i++) {
			uint32_t color = *p8;

			uint32_t b = (color >> 0) & 0xff;
			uint32_t g = (color >> 8) & 0xff;
			uint32_t r = (color >> 16) & 0xff;
			uint32_t a = (color >> 24) & 0xff;
			bgra8888 = a | (b << 8) | (g << 16) | (r << 24);
			*p = bgra8888;
			p8++;
			p++;
		}
	}

	void BGRAToARGB8888(int w, int h, uint8_t* argb_data, uint8_t* yuv_data)
	{
		uint32_t bgra8888 = 0;
		uint32_t* p = (uint32_t*)yuv_data;
		uint32_t* p8 = (uint32_t*)argb_data;
		for (int i = 0; i < w * h; i++) {
			uint32_t color = *p8;

			uint32_t b = (color >> 0) & 0xff;
			uint32_t g = (color >> 8) & 0xff;
			uint32_t r = (color >> 16) & 0xff;
			uint32_t a = (color >> 24) & 0xff;
			bgra8888 = a | (r << 8) | (g << 16) | (b << 24);
			*p = bgra8888;
			p8++;
			p++;
		}
	}

	static __inline int32_t clamp0(int32_t v) {
		return -(v >= 0) & v;
	}
	// TODO(fbarchard): make clamp255 preserve negative values.
	static __inline int32_t clamp255(int32_t v) {
		return (-(v >= 255) | v) & 255;
	}

	static __inline int32_t clamp1023(int32_t v) {
		return (-(v >= 1023) | v) & 1023;
	}

	static __inline uint32_t Clamp(int32_t val) {
		int v = clamp0(val);
		return (uint32_t)(clamp255(v));
	}

	void yuv2rgb_pixel(uint8_t b, uint8_t g, uint8_t r, uint8_t& y, uint8_t& u, uint8_t& v)
	{

		y = Clamp(((66 * r + 129 * g + 25 * b + 128) >> 8) + 16);
		u = Clamp((((-38 * r - 74 * g + 112 * b + 128) >> 8) + 128));
		v = Clamp((((112 * r - 94 * g - 18 * b + 128) >> 8) + 128));
	}

	void BGRAToYUYV(int width, int height, uint8_t* pBgra, uint8_t* pYuv)
	{
		int width1 = width = width / 2 * 2;

		for (int h = 0; h < height; h++)
		{
			uint8_t* ptr1 = pBgra + h * width * 4;
			uint8_t* ptr2 = pYuv + h * width * 2;

			for (int w = 0; w < width1; w += 2)
			{
				uint8_t y1, u1, v1, y2, u2, v2;
				yuv2rgb_pixel(ptr1[0], ptr1[1], ptr1[2], y1, u1, v1);
				ptr1 += 4;
				yuv2rgb_pixel(ptr1[0], ptr1[1], ptr1[2], y2, u2, v2);
				ptr1 += 4;
				uint8_t u = Clamp((u1 + u2) / 2);
				uint8_t v = Clamp((v1 + v2) / 2);
				*ptr2++ = y1;
				*ptr2++ = u;
				*ptr2++ = y2;
				*ptr2++ = v;
			}
		}
	}

	void BGRAToUYVY(int width, int height, uint8_t* pBgra, uint8_t* pYuv)
	{
		int width1 = width = width / 2 * 2;

		for (int h = 0; h < height; h++)
		{
			uint8_t* ptr1 = pBgra + h * width * 4;
			uint8_t* ptr2 = pYuv + h * width * 2;

			for (int w = 0; w < width1; w += 2)
			{
				uint8_t y1, u1, v1, y2, u2, v2;
				yuv2rgb_pixel(ptr1[0], ptr1[1], ptr1[2], y1, u1, v1);
				ptr1 += 4;
				yuv2rgb_pixel(ptr1[0], ptr1[1], ptr1[2], y2, u2, v2);
				ptr1 += 4;
				uint8_t u = Clamp((u1 + u2) / 2);
				uint8_t v = Clamp((v1 + v2) / 2);
				*ptr2++ = u;
				*ptr2++ = y1;
				*ptr2++ = v;
				*ptr2++ = y2;
			}
		}
	}

	void BGRAToYUV420(unsigned char* RgbBuf, int w, int h, unsigned char* yuvBuf)
	{
		unsigned char* ptrY, * ptrU, * ptrV, * ptrRGB;
		ptrY = yuvBuf;
		ptrU = yuvBuf + w * h;
		ptrV = ptrU + (w * h * 1 / 4);
		unsigned char y, u, v, r, g, b;
		for (int j = 0; j < h; j++) {
			ptrRGB = RgbBuf + w * j * 4;
			for (int i = 0; i < w; i++) {

				b = *(ptrRGB++);
				g = *(ptrRGB++);
				r = *(ptrRGB++);
				ptrRGB++;
				y = (unsigned char)((66 * r + 129 * g + 25 * b + 128) >> 8) + 16;
				u = (unsigned char)((-38 * r - 74 * g + 112 * b + 128) >> 8) + 128;
				v = (unsigned char)((112 * r - 94 * g - 18 * b + 128) >> 8) + 128;
				*(ptrY++) = Clamp(y);
				if (j % 2 == 0 && i % 2 == 0) {
					*(ptrU++) = Clamp(u);
				}
				else {
					if (i % 2 == 0) {
						*(ptrV++) = Clamp(v);
					}
				}
			}
		}
	}

	void BGRAToYV12(unsigned char* RgbBuf, int w, int h, unsigned char* yuvBuf)
	{
		unsigned char* ptrY, * ptrU, * ptrV, * ptrRGB;
		ptrY = yuvBuf;
		ptrV = yuvBuf + w * h;
		ptrU = ptrV + (w * h * 1 / 4);
		unsigned char y, u, v, r, g, b;
		for (int j = 0; j < h; j++) {
			ptrRGB = RgbBuf + w * j * 4;
			for (int i = 0; i < w; i++) {

				b = *(ptrRGB++);
				g = *(ptrRGB++);
				r = *(ptrRGB++);
				ptrRGB++;
				y = (unsigned char)((66 * r + 129 * g + 25 * b + 128) >> 8) + 16;
				u = (unsigned char)((-38 * r - 74 * g + 112 * b + 128) >> 8) + 128;
				v = (unsigned char)((112 * r - 94 * g - 18 * b + 128) >> 8) + 128;
				*(ptrY++) = Clamp(y);
				if (j % 2 == 0 && i % 2 == 0) {
					*(ptrU++) = Clamp(u);
				}
				else {
					if (i % 2 == 0) {
						*(ptrV++) = Clamp(v);
					}
				}
			}
		}
	}

	void BGRA8888ToNV12(const uint8_t* src_data, int src_width, int src_height, uint8_t* dst_data)
	{
		const int src_stride = src_width * 4;
		const int dst_y_stride = src_width;
		const int dst_uv_stride = src_width / 2;

		for (int y = 0; y < src_height; ++y) {
			const uint8_t* src = src_data + y * src_stride;
			uint8_t* dst_y = dst_data + y * dst_y_stride;
			uint8_t* dst_uv = dst_data + src_height * dst_y_stride + (y / 2) * dst_uv_stride * 2;

			for (int x = 0; x < src_width; x += 2) {
				const uint8_t b1 = *(src++);
				const uint8_t g1 = *(src++);
				const uint8_t r1 = *(src++);
				const uint8_t a1 = *(src++);
				const uint8_t b2 = *(src++);
				const uint8_t g2 = *(src++);
				const uint8_t r2 = *(src++);
				const uint8_t a2 = *(src++);

				const uint8_t y1 = Clamp(((66 * r1 + 129 * g1 + 25 * b1 + 128) >> 8) + 16);
				const uint8_t y2 = Clamp(((66 * r2 + 129 * g2 + 25 * b2 + 128) >> 8) + 16);
				const uint8_t u = Clamp(((-38 * r1 - 74 * g1 + 112 * b1 + 128) >> 8) + 128);
				const uint8_t v = Clamp(((112 * r1 - 94 * g1 - 18 * b1 + 128) >> 8) + 128);

				*(dst_y++) = y1;
				*(dst_y++) = y2;
				*(dst_uv++) = u;
				*(dst_uv++) = v;
			}
		}
	}

	void BGRA8888ToNV21(const uint8_t* src_data, int src_width, int src_height, uint8_t* dst_data)
	{
		const int src_stride = src_width * 4;
		const int dst_y_stride = src_width;
		const int dst_uv_stride = src_width / 2;

		for (int y = 0; y < src_height; ++y) {
			const uint8_t* src = src_data + y * src_stride;
			uint8_t* dst_y = dst_data + y * dst_y_stride;
			uint8_t* dst_uv = dst_data + src_height * dst_y_stride + (y / 2) * dst_uv_stride * 2;

			for (int x = 0; x < src_width; x += 2) {
				const uint8_t b1 = *(src++);
				const uint8_t g1 = *(src++);
				const uint8_t r1 = *(src++);
				const uint8_t a1 = *(src++);
				const uint8_t b2 = *(src++);
				const uint8_t g2 = *(src++);
				const uint8_t r2 = *(src++);
				const uint8_t a2 = *(src++);

				const uint8_t y1 = Clamp(((66 * r1 + 129 * g1 + 25 * b1 + 128) >> 8) + 16);
				const uint8_t y2 = Clamp(((66 * r2 + 129 * g2 + 25 * b2 + 128) >> 8) + 16);
				const uint8_t u = Clamp(((-38 * r1 - 74 * g1 + 112 * b1 + 128) >> 8) + 128);
				const uint8_t v = Clamp(((112 * r1 - 94 * g1 - 18 * b1 + 128) >> 8) + 128);

				*(dst_y++) = y1;
				*(dst_y++) = y2;
				*(dst_uv++) = v;
				*(dst_uv++) = u;
			}
		}
	}

	void BGRA8888ToNV16(const uint8_t* src_data, int src_width, int src_height, uint8_t* dst_data)
	{
		const int src_stride = src_width * 4;
		const int dst_y_stride = src_width;
		const int dst_uv_stride = src_width;

		// Convert BGRA8888 to YUV422
		for (int y = 0; y < src_height; ++y) {
			const uint8_t* src = src_data + y * src_stride;
			uint8_t* dst_y = dst_data + y * dst_y_stride;
			uint8_t* dst_uv = dst_data + src_height * dst_y_stride + y * dst_uv_stride;

			for (int x = 0; x < src_width; x += 2) {
				const uint8_t b1 = *(src++);
				const uint8_t g1 = *(src++);
				const uint8_t r1 = *(src++);
				const uint8_t a1 = *(src++);
				const uint8_t b2 = *(src++);
				const uint8_t g2 = *(src++);
				const uint8_t r2 = *(src++);
				const uint8_t a2 = *(src++);

				const uint8_t y1 = Clamp(((66 * r1 + 129 * g1 + 25 * b1 + 128) >> 8) + 16);
				const uint8_t y2 = Clamp(((66 * r2 + 129 * g2 + 25 * b2 + 128) >> 8) + 16);
				const uint8_t u = Clamp(((-38 * r1 - 74 * g1 + 112 * b1 + 128) >> 8) + 128);
				const uint8_t v = Clamp(((112 * r1 - 94 * g1 - 18 * b1 + 128) >> 8) + 128);

				*(dst_y++) = y1;
				*(dst_y++) = y2;
				*(dst_uv++) = u;
				*(dst_uv++) = v;
			}
		}
	}

	void BGRA8888ToNV61(const uint8_t* src_data, int src_width, int src_height, uint8_t* dst_data)
	{
		const int src_stride = src_width * 4;
		const int dst_y_stride = src_width;
		const int dst_uv_stride = src_width;

		// Convert BGRA8888 to YUV422
		for (int y = 0; y < src_height; ++y) {
			const uint8_t* src = src_data + y * src_stride;
			uint8_t* dst_y = dst_data + y * dst_y_stride;
			uint8_t* dst_uv = dst_data + src_height * dst_y_stride + y * dst_uv_stride;

			for (int x = 0; x < src_width; x += 2) {
				const uint8_t b1 = *(src++);
				const uint8_t g1 = *(src++);
				const uint8_t r1 = *(src++);
				const uint8_t a1 = *(src++);
				const uint8_t b2 = *(src++);
				const uint8_t g2 = *(src++);
				const uint8_t r2 = *(src++);
				const uint8_t a2 = *(src++);

				const uint8_t y1 = Clamp(((66 * r1 + 129 * g1 + 25 * b1 + 128) >> 8) + 16);
				const uint8_t y2 = Clamp(((66 * r2 + 129 * g2 + 25 * b2 + 128) >> 8) + 16);
				const uint8_t u = Clamp(((-38 * r1 - 74 * g1 + 112 * b1 + 128) >> 8) + 128);
				const uint8_t v = Clamp(((112 * r1 - 94 * g1 - 18 * b1 + 128) >> 8) + 128);

				*(dst_y++) = y1;
				*(dst_y++) = y2;
				*(dst_uv++) = v;
				*(dst_uv++) = u;
			}
		}
	}


	void BGRA8888ToP010LSB(const uint8_t* src_data, int src_width, int src_height, uint8_t* dst_data)
	{
		const int src_stride = src_width * 4;
		const int dst_y_stride = src_width * 2;
		const int dst_uv_stride = src_width;

		for (int y = 0; y < src_height; ++y) {
			const uint8_t* src = src_data + y * src_stride;
			uint16_t* dst_y = (uint16_t*)(dst_data + y * dst_y_stride);
			uint16_t* dst_uv = (uint16_t*)(dst_data + src_height * dst_y_stride + (y / 2) * dst_uv_stride * 2);

			for (int x = 0; x < src_width; x += 2) {
				const uint8_t b1 = *(src++);
				const uint8_t g1 = *(src++);
				const uint8_t r1 = *(src++);
				const uint8_t a1 = *(src++);
				const uint8_t b2 = *(src++);
				const uint8_t g2 = *(src++);
				const uint8_t r2 = *(src++);
				const uint8_t a2 = *(src++);

				const uint16_t y1 = clamp1023(((66 * r1 + 129 * g1 + 25 * b1 + 128) >> 8) + 16);
				const uint16_t y2 = clamp1023(((66 * r2 + 129 * g2 + 25 * b2 + 128) >> 8) + 16);
				const uint16_t u = clamp1023(((-38 * r1 - 74 * g1 + 112 * b1 + 128) >> 8) + 128);
				const uint16_t v = clamp1023(((112 * r1 - 94 * g1 - 18 * b1 + 128) >> 8) + 128);

				*(dst_y++) = y1 << 2;
				*(dst_y++) = y2 << 2;
				*(dst_uv++) = u << 2;
				*(dst_uv++) = v << 2;
			}
		}
	}

	void BGRA8888ToP010MSB(const uint8_t* src_data, int src_width, int src_height, uint8_t* dst_data)
	{
		const int src_stride = src_width * 4;
		const int dst_y_stride = src_width * 2;
		const int dst_uv_stride = src_width;

		for (int y = 0; y < src_height; ++y) {
			const uint8_t* src = src_data + y * src_stride;
			uint16_t* dst_y = (uint16_t*)(dst_data + y * dst_y_stride);
			uint16_t* dst_uv = (uint16_t*)(dst_data + src_height * dst_y_stride + (y / 2) * dst_uv_stride * 2);

			for (int x = 0; x < src_width; x += 2) {
				const uint8_t b1 = *(src++);
				const uint8_t g1 = *(src++);
				const uint8_t r1 = *(src++);
				const uint8_t a1 = *(src++);
				const uint8_t b2 = *(src++);
				const uint8_t g2 = *(src++);
				const uint8_t r2 = *(src++);
				const uint8_t a2 = *(src++);

				const uint16_t y1 = clamp1023(((66 * r1 + 129 * g1 + 25 * b1 + 128) >> 8) + 16);
				const uint16_t y2 = clamp1023(((66 * r2 + 129 * g2 + 25 * b2 + 128) >> 8) + 16);
				const uint16_t u = clamp1023(((-38 * r1 - 74 * g1 + 112 * b1 + 128) >> 8) + 128);
				const uint16_t v = clamp1023(((112 * r1 - 94 * g1 - 18 * b1 + 128) >> 8) + 128);

				*(dst_y++) = y1 << 8;
				*(dst_y++) = y2 << 8;
				*(dst_uv++) = u << 8;
				*(dst_uv++) = v << 8;
			}
		}
	}

	void BGRAToI010(unsigned char* RgbBuf, int w, int h, unsigned char* yuvBuf)
	{
		uint8_t* ptrRGB;
		uint16_t* ptrY, * ptrU, * ptrV;
		ptrY = (uint16_t*)yuvBuf;
		ptrU = (uint16_t*)(yuvBuf + (w * h * 2));
		ptrV = (uint16_t*)((uint8_t*)ptrU + ((w * h * 1 / 4 * 2)));
		uint16_t y, u, v, r, g, b;
		for (int j = 0; j < h; j++) {
			ptrRGB = RgbBuf + w * j * 4;
			for (int i = 0; i < w; i++) {

				b = *(ptrRGB++);
				g = *(ptrRGB++);
				r = *(ptrRGB++);
				ptrRGB++;
				y = (uint16_t)((66 * r + 129 * g + 25 * b + 128) >> 8) + 16;
				u = (uint16_t)((-38 * r - 74 * g + 112 * b + 128) >> 8) + 128;
				v = (uint16_t)((112 * r - 94 * g - 18 * b + 128) >> 8) + 128;
				*(ptrY++) = clamp1023(y) << 2;
				if (j % 2 == 0 && i % 2 == 0) {
					*(ptrU++) = clamp1023(u) << 2;
				}
				else {
					if (i % 2 == 0) {
						*(ptrV++) = clamp1023(v) << 2;
					}
				}
			}
		}
	}


	void BGRAToBGR888P(uint8_t* argb, int w, int h, uint8_t* bgrp)
	{
		uint8_t* b, * g, * r;
		b = bgrp;
		g = bgrp + w * h;
		r = bgrp + w * h * 2;
		for (int y = 0; y < h; y++)
		{
			for (int x = 0; x < w; x++)
			{
				*b = argb[0];
				*g = argb[1];
				*r = argb[2];
				r++;
				g++;
				b++;
				argb += 4;
			}
		}
	}

	void BGRAToRGB888P(uint8_t* argb, int w, int h, uint8_t* bgrp)
	{
		uint8_t* b, * g, * r;
		r = bgrp;
		g = bgrp + w * h;
		b = bgrp + w * h * 2;
		for (int y = 0; y < h; y++)
		{
			for (int x = 0; x < w; x++)
			{
				*b = argb[0];
				*g = argb[1];
				*r = argb[2];
				r++;
				g++;
				b++;
				argb += 4;
			}
		}
	}

	void BGRAToRGB24(uint8_t* argb, int w, int h, uint8_t* bgrp)
	{
		for (int y = 0; y < h; y++)
		{
			for (int x = 0; x < w; x++)
			{
				bgrp[2] = argb[0];
				bgrp[1] = argb[1];
				bgrp[0] = argb[2];
				bgrp += 3;
				argb += 4;
			}
		}
	}

	void BGRAToBGR24(uint8_t* argb, int w, int h, uint8_t* bgrp)
	{
		for (int y = 0; y < h; y++)
		{
			for (int x = 0; x < w; x++)
			{
				bgrp[0] = argb[0];
				bgrp[1] = argb[1];
				bgrp[2] = argb[2];
				bgrp += 3;
				argb += 4;
			}
		}
	}
};

