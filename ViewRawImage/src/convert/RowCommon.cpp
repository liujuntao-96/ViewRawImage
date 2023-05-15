#include <cstdint>

namespace convert {
#define WRITEWORD(p, v) *(uint32_t*)(p) = v

#define LOAD_YUV_CONSTANTS	\
		const int ub = 128;\
	const int ug = 25;\
	const int vg = 52;\
	const int vr = 102;\
	const int yg = 18997;\
	const int yb = -1160;

#define CALC_RGB16                                \
  int32_t y1 = ((uint32_t)(y32 * yg) >> 16) + yb; \
  int8_t ui = (int8_t)u;                          \
  int8_t vi = (int8_t)v;                          \
  ui -= 0x80;                                     \
  vi -= 0x80;                                     \
  int b16 = y1 + (ui * ub);                       \
  int g16 = y1 - (ui * ug + vi * vg);             \
  int r16 = y1 + (vi * vr)

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

	static __inline void YuvPixel(uint8_t y,
		uint8_t u,
		uint8_t v,
		uint8_t* b,
		uint8_t* g,
		uint8_t* r) {
		LOAD_YUV_CONSTANTS;
		uint32_t y32 = y * 0x0101;
		CALC_RGB16;
		*b = Clamp((int32_t)(b16) >> 6);
		*g = Clamp((int32_t)(g16) >> 6);
		*r = Clamp((int32_t)(r16) >> 6);
	}

	void YUY2ToARGBRow_C(const uint8_t* src_yuy2, uint8_t* rgb_buf, int width) {
		int x;
		for (x = 0; x < width - 1; x += 2) {
			YuvPixel(src_yuy2[0], src_yuy2[1], src_yuy2[3], rgb_buf + 0, rgb_buf + 1,
				rgb_buf + 2);
			rgb_buf[3] = 255;
			YuvPixel(src_yuy2[2], src_yuy2[1], src_yuy2[3], rgb_buf + 4, rgb_buf + 5,
				rgb_buf + 6);
			rgb_buf[7] = 255;
			src_yuy2 += 4;
			rgb_buf += 8;  // Advance 2 pixels.
		}
		if (width & 1) {
			YuvPixel(src_yuy2[0], src_yuy2[1], src_yuy2[3], rgb_buf + 0, rgb_buf + 1,
				rgb_buf + 2);
			rgb_buf[3] = 255;
		}
	}

	void UYVYToARGBRow_C(const uint8_t* src_uyvy,
		uint8_t* rgb_buf,
		int width) {
		int x;
		for (x = 0; x < width - 1; x += 2) {
			YuvPixel(src_uyvy[1], src_uyvy[0], src_uyvy[2], rgb_buf + 0, rgb_buf + 1,
				rgb_buf + 2);
			rgb_buf[3] = 255;
			YuvPixel(src_uyvy[3], src_uyvy[0], src_uyvy[2], rgb_buf + 4, rgb_buf + 5,
				rgb_buf + 6);
			rgb_buf[7] = 255;
			src_uyvy += 4;
			rgb_buf += 8;  // Advance 2 pixels.
		}
		if (width & 1) {
			YuvPixel(src_uyvy[1], src_uyvy[0], src_uyvy[2], rgb_buf + 0, rgb_buf + 1,
				rgb_buf + 2);
			rgb_buf[3] = 255;
		}
	}

	void I422ToARGBRow_C(const uint8_t* src_y,
		const uint8_t* src_u,
		const uint8_t* src_v,
		uint8_t* rgb_buf,
		int width) {
		int x;
		for (x = 0; x < width - 1; x += 2) {
			YuvPixel(src_y[0], src_u[0], src_v[0], rgb_buf + 0, rgb_buf + 1,
				rgb_buf + 2);
			rgb_buf[3] = 255;
			YuvPixel(src_y[1], src_u[0], src_v[0], rgb_buf + 4, rgb_buf + 5,
				rgb_buf + 6);
			rgb_buf[7] = 255;
			src_y += 2;
			src_u += 1;
			src_v += 1;
			rgb_buf += 8;  // Advance 2 pixels.
		}
		if (width & 1) {
			YuvPixel(src_y[0], src_u[0], src_v[0], rgb_buf + 0, rgb_buf + 1,
				rgb_buf + 2);
			rgb_buf[3] = 255;
		}
	}

	void NV12ToARGBRow_C(const uint8_t* src_y,
		const uint8_t* src_uv,
		uint8_t* rgb_buf,
		int width) {
		int x;
		for (x = 0; x < width - 1; x += 2) {
			YuvPixel(src_y[0], src_uv[0], src_uv[1], rgb_buf + 0, rgb_buf + 1,
				rgb_buf + 2);
			rgb_buf[3] = 255;
			YuvPixel(src_y[1], src_uv[0], src_uv[1], rgb_buf + 4, rgb_buf + 5,
				rgb_buf + 6);
			rgb_buf[7] = 255;
			src_y += 2;
			src_uv += 2;
			rgb_buf += 8;  // Advance 2 pixels.
		}
		if (width & 1) {
			YuvPixel(src_y[0], src_uv[0], src_uv[1], rgb_buf + 0, rgb_buf + 1,
				rgb_buf + 2);
			rgb_buf[3] = 255;
		}
	}

	void NV21ToARGBRow_C(const uint8_t* src_y,
		const uint8_t* src_vu,
		uint8_t* rgb_buf,
		int width) {
		int x;
		for (x = 0; x < width - 1; x += 2) {
			YuvPixel(src_y[0], src_vu[1], src_vu[0], rgb_buf + 0, rgb_buf + 1,
				rgb_buf + 2);
			rgb_buf[3] = 255;
			YuvPixel(src_y[1], src_vu[1], src_vu[0], rgb_buf + 4, rgb_buf + 5,
				rgb_buf + 6);
			rgb_buf[7] = 255;
			src_y += 2;
			src_vu += 2;
			rgb_buf += 8;  // Advance 2 pixels.
		}
		if (width & 1) {
			YuvPixel(src_y[0], src_vu[1], src_vu[0], rgb_buf + 0, rgb_buf + 1,
				rgb_buf + 2);
			rgb_buf[3] = 255;
		}
	}

	void NV16ToARGBRow_C(const uint8_t* src_y,
		const uint8_t* src_uv,
		uint8_t* rgb_buf,
		int width) {
		int x;
		for (x = 0; x < width - 1; x += 2) {
			YuvPixel(src_y[0], src_uv[0], src_uv[1], rgb_buf + 0, rgb_buf + 1,
				rgb_buf + 2);
			rgb_buf[3] = 255;
			YuvPixel(src_y[1], src_uv[0], src_uv[1], rgb_buf + 4, rgb_buf + 5,
				rgb_buf + 6);
			rgb_buf[7] = 255;
			src_y += 2;
			src_uv += 2;
			rgb_buf += 8;  // Advance 2 pixels.
		}
		if (width & 1) {
			YuvPixel(src_y[0], src_uv[0], src_uv[1], rgb_buf + 0, rgb_buf + 1,
				rgb_buf + 2);
			rgb_buf[3] = 255;
		}
	}

	void NV61ToARGBRow_C(const uint8_t* src_y,
		const uint8_t* src_vu,
		uint8_t* rgb_buf,
		int width) {
		int x;
		for (x = 0; x < width - 1; x += 2) {
			YuvPixel(src_y[0], src_vu[1], src_vu[0], rgb_buf + 0, rgb_buf + 1,
				rgb_buf + 2);
			rgb_buf[3] = 255;
			YuvPixel(src_y[1], src_vu[1], src_vu[0], rgb_buf + 4, rgb_buf + 5,
				rgb_buf + 6);
			rgb_buf[7] = 255;
			src_y += 2;
			src_vu += 2;
			rgb_buf += 8;  // Advance 2 pixels.
		}
		if (width & 1) {
			YuvPixel(src_y[0], src_vu[1], src_vu[0], rgb_buf + 0, rgb_buf + 1,
				rgb_buf + 2);
			rgb_buf[3] = 255;
		}
	}

	// P210 has 10 bits in msb of 16 bit NV12 style layout.
	void P210ToARGBRow_C(const uint16_t* src_y,
		const uint16_t* src_uv,
		uint8_t* dst_argb,
		int width, int shift) {
		int x;
		uint8_t y[2], u, v;

		for (x = 0; x < width - 1; x += 2) {
			y[0] = (src_y[0] >> shift) & 0xff;
			y[1] = (src_y[1] >> shift) & 0xff;
			u = (src_uv[0] >> shift) & 0xff;
			v = (src_uv[1] >> shift) & 0xff;

			YuvPixel(y[0], u, v, dst_argb + 0, dst_argb + 1,
				dst_argb + 2);
			dst_argb[3] = 255;
			YuvPixel(y[1], u, v, dst_argb + 4, dst_argb + 5,
				dst_argb + 6);
			dst_argb[7] = 255;
			src_y += 2;
			src_uv += 2;
			dst_argb += 8;  // Advance 2 pixels.
		}
		if (width & 1) {
			y[0] = (src_y[0] >> shift) & 0xff;
			y[1] = (src_y[1] >> shift) & 0xff;
			u = (src_uv[0] >> shift) & 0xff;
			v = (src_uv[1] >> shift) & 0xff;
			YuvPixel(y[0], u, v, dst_argb + 0, dst_argb + 1,
				dst_argb + 2);
			dst_argb[3] = 255;
		}
	}

	// 10 bit YUV to ARGB
	void I210ToARGBRow_C(const uint16_t* src_y,
		const uint16_t* src_u,
		const uint16_t* src_v,
		uint8_t* rgb_buf,
		int width, int shift) {
		int x;
		uint8_t y[2], u, v;
		for (x = 0; x < width - 1; x += 2) {
			y[0] = (src_y[0] >> shift) & 0xff;
			y[1] = (src_y[1] >> shift) & 0xff;
			u = (src_u[0] >> shift) & 0xff;
			v = (src_v[0] >> shift) & 0xff;
			YuvPixel(y[0], u, v, rgb_buf + 0, rgb_buf + 1,
				rgb_buf + 2);
			rgb_buf[3] = 255;
			YuvPixel(y[1], u, v, rgb_buf + 4, rgb_buf + 5,
				rgb_buf + 6);
			rgb_buf[7] = 255;
			src_y += 2;
			src_u += 1;
			src_v += 1;
			rgb_buf += 8;  // Advance 2 pixels.
		}
		if (width & 1) {
			y[0] = (src_y[0] >> shift) & 0xff;
			u = (src_u[0] >> shift) & 0xff;
			v = (src_v[0] >> shift) & 0xff;
			YuvPixel(y[0], u, v, rgb_buf + 0, rgb_buf + 1,
				rgb_buf + 2);
			rgb_buf[3] = 255;
		}
	}

	void ABGR4444ToARGBRow_C(const uint8_t* src_argb4444,
		uint8_t* dst_argb,
		int width) {
		int x;
		for (x = 0; x < width; ++x) {
			uint8_t a = src_argb4444[0] & 0x0f;
			uint8_t b = src_argb4444[0] >> 4;
			uint8_t g = src_argb4444[1] & 0x0f;
			uint8_t r = src_argb4444[1] >> 4;
			dst_argb[0] = (b << 4) | b;
			dst_argb[1] = (g << 4) | g;
			dst_argb[2] = (r << 4) | r;
			dst_argb[3] = (a << 4) | a;
			dst_argb += 4;
			src_argb4444 += 2;
		}
	}

	void ARGB4444ToARGBRow_C(const uint8_t* src_argb4444,
		uint8_t* dst_argb,
		int width) {
		int x;
		for (x = 0; x < width; ++x) {
			uint8_t a = src_argb4444[0] & 0x0f;
			uint8_t r = src_argb4444[0] >> 4;
			uint8_t g = src_argb4444[1] & 0x0f;
			uint8_t b = src_argb4444[1] >> 4;
			dst_argb[0] = (b << 4) | b;
			dst_argb[1] = (g << 4) | g;
			dst_argb[2] = (r << 4) | r;
			dst_argb[3] = (a << 4) | a;
			dst_argb += 4;
			src_argb4444 += 2;
		}
	}

	void BGRA4444ToARGBRow_C(const uint8_t* src_argb4444,
		uint8_t* dst_argb,
		int width) {
		int x;
		for (x = 0; x < width; ++x) {
			uint8_t b = src_argb4444[0] & 0x0f;
			uint8_t g = src_argb4444[0] >> 4;
			uint8_t r = src_argb4444[1] & 0x0f;
			uint8_t a = src_argb4444[1] >> 4;
			dst_argb[0] = (b << 4) | b;
			dst_argb[1] = (g << 4) | g;
			dst_argb[2] = (r << 4) | r;
			dst_argb[3] = (a << 4) | a;
			dst_argb += 4;
			src_argb4444 += 2;
		}
	}

	void RGBA4444ToARGBRow_C(const uint8_t* src_argb4444,
		uint8_t* dst_argb,
		int width) {
		int x;
		for (x = 0; x < width; ++x) {
			uint8_t r = src_argb4444[0] & 0x0f;
			uint8_t g = src_argb4444[0] >> 4;
			uint8_t b = src_argb4444[1] & 0x0f;
			uint8_t a = src_argb4444[1] >> 4;
			dst_argb[0] = (b << 4) | b;
			dst_argb[1] = (g << 4) | g;
			dst_argb[2] = (r << 4) | r;
			dst_argb[3] = (a << 4) | a;
			dst_argb += 4;
			src_argb4444 += 2;
		}
	}

	void ARGB1555ToARGBRow_C(const uint8_t* src_argb1555,
		uint8_t* dst_argb,
		int width) {
		int x;
		for (x = 0; x < width; ++x) {
			uint16_t color = *(uint16_t*)src_argb1555;
			uint8_t a = color & 0x1;
			uint8_t r = (color >> 1) & 0x1f;
			uint8_t g = (color >> 6) & 0x1f;
			uint8_t b = (color >> 11) & 0x1f;

			dst_argb[0] = (b << 3) | (b >> 2);
			dst_argb[1] = (g << 3) | (g >> 2);
			dst_argb[2] = (r << 3) | (r >> 2);
			dst_argb[3] = -a;
			dst_argb += 4;
			src_argb1555 += 2;
		}
	}

	void ABGR1555ToARGBRow_C(const uint8_t* src_argb1555,
		uint8_t* dst_argb,
		int width) {
		int x;
		for (x = 0; x < width; ++x) {
			uint16_t color = *(uint16_t*)src_argb1555;
			uint8_t a = color & 0x1;
			uint8_t b = (color >> 1) & 0x1f;
			uint8_t g = (color >> 6) & 0x1f;
			uint8_t r = (color >> 11) & 0x1f;

			dst_argb[0] = (b << 3) | (b >> 2);
			dst_argb[1] = (g << 3) | (g >> 2);
			dst_argb[2] = (r << 3) | (r >> 2);
			dst_argb[3] = -a;
			dst_argb += 4;
			src_argb1555 += 2;
		}
	}

	void RGBA5551ToARGBRow_C(const uint8_t* src_argb1555,
		uint8_t* dst_argb,
		int width) {
		int x;
		for (x = 0; x < width; ++x) {
			uint8_t r = src_argb1555[0] & 0x1f;
			uint8_t g = (src_argb1555[0] >> 5) | ((src_argb1555[1] & 0x03) << 3);
			uint8_t b = (src_argb1555[1] & 0x7c) >> 2;
			uint8_t a = src_argb1555[1] >> 7;
			dst_argb[0] = (b << 3) | (b >> 2);
			dst_argb[1] = (g << 3) | (g >> 2);
			dst_argb[2] = (r << 3) | (r >> 2);
			dst_argb[3] = -a;
			dst_argb += 4;
			src_argb1555 += 2;
		}
	}

	void BGRA5551ToARGBRow_C(const uint8_t* src_argb1555,
		uint8_t* dst_argb,
		int width) {
		int x;
		for (x = 0; x < width; ++x) {
			uint8_t b = src_argb1555[0] & 0x1f;
			uint8_t g = (src_argb1555[0] >> 5) | ((src_argb1555[1] & 0x03) << 3);
			uint8_t r = (src_argb1555[1] & 0x7c) >> 2;
			uint8_t a = src_argb1555[1] >> 7;
			dst_argb[0] = (b << 3) | (b >> 2);
			dst_argb[1] = (g << 3) | (g >> 2);
			dst_argb[2] = (r << 3) | (r >> 2);
			dst_argb[3] = -a;
			dst_argb += 4;
			src_argb1555 += 2;
		}
	}

	void BGR565ToARGBRow_C(const uint8_t* src_rgb565,
		uint8_t* dst_argb,
		int width) {
		int x;
		for (x = 0; x < width; ++x) {
			uint8_t b = src_rgb565[0] & 0x1f;
			uint8_t g = (src_rgb565[0] >> 5) | ((src_rgb565[1] & 0x07) << 3);
			uint8_t r = src_rgb565[1] >> 3;
			dst_argb[0] = (b << 3) | (b >> 2);
			dst_argb[1] = (g << 2) | (g >> 4);
			dst_argb[2] = (r << 3) | (r >> 2);
			dst_argb[3] = 255u;
			dst_argb += 4;
			src_rgb565 += 2;
		}
	}

	void RGB565ToARGBRow_C(const uint8_t* src_rgb565,
		uint8_t* dst_argb,
		int width) {
		int x;
		for (x = 0; x < width; ++x) {
			uint8_t r = src_rgb565[0] & 0x1f;
			uint8_t g = (src_rgb565[0] >> 5) | ((src_rgb565[1] & 0x07) << 3);
			uint8_t b = src_rgb565[1] >> 3;
			dst_argb[0] = (b << 3) | (b >> 2);
			dst_argb[1] = (g << 2) | (g >> 4);
			dst_argb[2] = (r << 3) | (r >> 2);
			dst_argb[3] = 255u;
			dst_argb += 4;
			src_rgb565 += 2;
		}
	}


	void ARGBToARGB1555Row_C(const uint8_t* src_argb, uint8_t* dst_rgb, int width) {
		int x;
		for (x = 0; x < width - 1; x += 2) {
			uint8_t b0 = src_argb[0] >> 3;
			uint8_t g0 = src_argb[1] >> 3;
			uint8_t r0 = src_argb[2] >> 3;
			uint8_t a0 = src_argb[3] >> 7;
			uint8_t b1 = src_argb[4] >> 3;
			uint8_t g1 = src_argb[5] >> 3;
			uint8_t r1 = src_argb[6] >> 3;
			uint8_t a1 = src_argb[7] >> 7;
			*(uint16_t*)(dst_rgb + 0) = b0 | (g0 << 5) | (r0 << 10) | (a0 << 15);
			*(uint16_t*)(dst_rgb + 2) = b1 | (g1 << 5) | (r1 << 10) | (a1 << 15);
			dst_rgb += 4;
			src_argb += 8;
		}
		if (width & 1) {
			uint8_t b0 = src_argb[0] >> 3;
			uint8_t g0 = src_argb[1] >> 3;
			uint8_t r0 = src_argb[2] >> 3;
			uint8_t a0 = src_argb[3] >> 7;
			*(uint16_t*)(dst_rgb) = b0 | (g0 << 5) | (r0 << 10) | (a0 << 15);
		}
	}

	void ARGBToABGR1555Row_C(const uint8_t* src_argb, uint8_t* dst_rgb, int width) {
		int x;
		for (x = 0; x < width - 1; x += 2) {
			uint8_t b0 = src_argb[0] >> 3;
			uint8_t g0 = src_argb[1] >> 3;
			uint8_t r0 = src_argb[2] >> 3;
			uint8_t a0 = src_argb[3] >> 7;
			uint8_t b1 = src_argb[4] >> 3;
			uint8_t g1 = src_argb[5] >> 3;
			uint8_t r1 = src_argb[6] >> 3;
			uint8_t a1 = src_argb[7] >> 7;
			*(uint16_t*)(dst_rgb + 0) = r0 | (g0 << 5) | (b0 << 10) | (a0 << 15);
			*(uint16_t*)(dst_rgb + 2) = r1 | (g1 << 5) | (b1 << 10) | (a1 << 15);
			dst_rgb += 4;
			src_argb += 8;
		}
		if (width & 1) {
			uint8_t b0 = src_argb[0] >> 3;
			uint8_t g0 = src_argb[1] >> 3;
			uint8_t r0 = src_argb[2] >> 3;
			uint8_t a0 = src_argb[3] >> 7;
			*(uint16_t*)(dst_rgb) = r0 | (g0 << 5) | (b0 << 10) | (a0 << 15);
		}
	}

	void ARGBToRGBA1555Row_C(const uint8_t* src_argb, uint8_t* dst_rgb, int width) {
		int x;
		for (x = 0; x < width - 1; x += 2) {
			uint8_t b0 = src_argb[0] >> 3;
			uint8_t g0 = src_argb[1] >> 3;
			uint8_t r0 = src_argb[2] >> 3;
			uint8_t a0 = src_argb[3] >> 7;
			uint8_t b1 = src_argb[4] >> 3;
			uint8_t g1 = src_argb[5] >> 3;
			uint8_t r1 = src_argb[6] >> 3;
			uint8_t a1 = src_argb[7] >> 7;

			*(uint16_t*)(dst_rgb + 0) = a0 | (b0 << 1) | (g0 << 6) | (r0 << 11);
			*(uint16_t*)(dst_rgb + 2) = a1 | (b1 << 1) | (g1 << 6) | (r1 << 11);
			dst_rgb += 4;
			src_argb += 8;
		}
		if (width & 1) {
			uint8_t b0 = src_argb[0] >> 3;
			uint8_t g0 = src_argb[1] >> 3;
			uint8_t r0 = src_argb[2] >> 3;
			uint8_t a0 = src_argb[3] >> 7;
			*(uint16_t*)(dst_rgb) = a0 | (b0 << 1) | (g0 << 6) | (r0 << 11);
		}
	}

	void ARGBToBGRA1555Row_C(const uint8_t* src_argb, uint8_t* dst_rgb, int width) {
		int x;
		for (x = 0; x < width - 1; x += 2) {
			uint8_t b0 = src_argb[0] >> 3;
			uint8_t g0 = src_argb[1] >> 3;
			uint8_t r0 = src_argb[2] >> 3;
			uint8_t a0 = src_argb[3] >> 7;
			uint8_t b1 = src_argb[4] >> 3;
			uint8_t g1 = src_argb[5] >> 3;
			uint8_t r1 = src_argb[6] >> 3;
			uint8_t a1 = src_argb[7] >> 7;
			*(uint16_t*)(dst_rgb + 0) = a0 | (r0 << 1) | (g0 << 6) | (b0 << 11);
			*(uint16_t*)(dst_rgb + 2) = a1 | (r1 << 1) | (g1 << 6) | (b1 << 11);
			dst_rgb += 4;
			src_argb += 8;
		}
		if (width & 1) {
			uint8_t b0 = src_argb[0] >> 3;
			uint8_t g0 = src_argb[1] >> 3;
			uint8_t r0 = src_argb[2] >> 3;
			uint8_t a0 = src_argb[3] >> 7;
			*(uint16_t*)(dst_rgb) = a0 | (r0 << 1) | (g0 << 6) | (b0 << 11);
		}
	}

	void ARGBToARGB4444Row_C(const uint8_t* src_argb, uint8_t* dst_rgb, int width) {
		int x;
		for (x = 0; x < width - 1; x += 2) {
			uint8_t b0 = src_argb[0] >> 4;
			uint8_t g0 = src_argb[1] >> 4;
			uint8_t r0 = src_argb[2] >> 4;
			uint8_t a0 = src_argb[3] >> 4;
			uint8_t b1 = src_argb[4] >> 4;
			uint8_t g1 = src_argb[5] >> 4;
			uint8_t r1 = src_argb[6] >> 4;
			uint8_t a1 = src_argb[7] >> 4;
			*(uint16_t*)(dst_rgb + 0) = b0 | (g0 << 4) | (r0 << 8) | (a0 << 12);
			*(uint16_t*)(dst_rgb + 2) = b1 | (g1 << 4) | (r1 << 8) | (a1 << 12);
			dst_rgb += 4;
			src_argb += 8;
		}
		if (width & 1) {
			uint8_t b0 = src_argb[0] >> 4;
			uint8_t g0 = src_argb[1] >> 4;
			uint8_t r0 = src_argb[2] >> 4;
			uint8_t a0 = src_argb[3] >> 4;
			*(uint16_t*)(dst_rgb) = b0 | (g0 << 4) | (r0 << 8) | (a0 << 12);
		}
	}
	void ARGBToABGR4444Row_C(const uint8_t* src_argb, uint8_t* dst_rgb, int width) {
		int x;
		for (x = 0; x < width - 1; x += 2) {
			uint8_t b0 = src_argb[0] >> 4;
			uint8_t g0 = src_argb[1] >> 4;
			uint8_t r0 = src_argb[2] >> 4;
			uint8_t a0 = src_argb[3] >> 4;
			uint8_t b1 = src_argb[4] >> 4;
			uint8_t g1 = src_argb[5] >> 4;
			uint8_t r1 = src_argb[6] >> 4;
			uint8_t a1 = src_argb[7] >> 4;
			*(uint16_t*)(dst_rgb + 0) = r0 | (g0 << 4) | (b0 << 8) | (a0 << 12);
			*(uint16_t*)(dst_rgb + 2) = r1 | (g1 << 4) | (b1 << 8) | (a1 << 12);
			dst_rgb += 4;
			src_argb += 8;
		}
		if (width & 1) {
			uint8_t b0 = src_argb[0] >> 4;
			uint8_t g0 = src_argb[1] >> 4;
			uint8_t r0 = src_argb[2] >> 4;
			uint8_t a0 = src_argb[3] >> 4;
			*(uint16_t*)(dst_rgb) = r0 | (g0 << 4) | (b0 << 8) | (a0 << 12);
		}
	}
	void ARGBToRGBA4444Row_C(const uint8_t* src_argb, uint8_t* dst_rgb, int width) {
		int x;
		for (x = 0; x < width - 1; x += 2) {
			uint8_t b0 = src_argb[0] >> 4;
			uint8_t g0 = src_argb[1] >> 4;
			uint8_t r0 = src_argb[2] >> 4;
			uint8_t a0 = src_argb[3] >> 4;
			uint8_t b1 = src_argb[4] >> 4;
			uint8_t g1 = src_argb[5] >> 4;
			uint8_t r1 = src_argb[6] >> 4;
			uint8_t a1 = src_argb[7] >> 4;
			*(uint16_t*)(dst_rgb + 0) = a0 | (b0 << 4) | (g0 << 8) | (r0 << 12);
			*(uint16_t*)(dst_rgb + 2) = a1 | (b1 << 4) | (g1 << 8) | (r1 << 12);
			dst_rgb += 4;
			src_argb += 8;
		}
		if (width & 1) {
			uint8_t b0 = src_argb[0] >> 4;
			uint8_t g0 = src_argb[1] >> 4;
			uint8_t r0 = src_argb[2] >> 4;
			uint8_t a0 = src_argb[3] >> 4;
			*(uint16_t*)(dst_rgb) = a0 | (b0 << 4) | (g0 << 8) | (r0 << 12);
		}
	}

	void ARGBToBGRA4444Row_C(const uint8_t* src_argb, uint8_t* dst_rgb, int width) {
		int x;
		for (x = 0; x < width - 1; x += 2) {
			uint8_t b0 = src_argb[0] >> 4;
			uint8_t g0 = src_argb[1] >> 4;
			uint8_t r0 = src_argb[2] >> 4;
			uint8_t a0 = src_argb[3] >> 4;
			uint8_t b1 = src_argb[4] >> 4;
			uint8_t g1 = src_argb[5] >> 4;
			uint8_t r1 = src_argb[6] >> 4;
			uint8_t a1 = src_argb[7] >> 4;
			*(uint16_t*)(dst_rgb + 0) = a0 | (r0 << 4) | (g0 << 8) | (b0 << 12);
			*(uint16_t*)(dst_rgb + 2) = a1 | (r1 << 4) | (g1 << 8) | (b1 << 12);
			dst_rgb += 4;
			src_argb += 8;
		}
		if (width & 1) {
			uint8_t b0 = src_argb[0] >> 4;
			uint8_t g0 = src_argb[1] >> 4;
			uint8_t r0 = src_argb[2] >> 4;
			uint8_t a0 = src_argb[3] >> 4;
			*(uint16_t*)(dst_rgb) = a0 | (r0 << 4) | (g0 << 8) | (b0 << 12);
		}
	}

	void ARGBToRGB565Row_C(const uint8_t* src_argb, uint8_t* dst_rgb, int width) {
		int x;
		for (x = 0; x < width - 1; x += 2) {
			uint8_t b0 = src_argb[0] >> 3;
			uint8_t g0 = src_argb[1] >> 2;
			uint8_t r0 = src_argb[2] >> 3;
			uint8_t b1 = src_argb[4] >> 3;
			uint8_t g1 = src_argb[5] >> 2;
			uint8_t r1 = src_argb[6] >> 3;
			WRITEWORD(dst_rgb, b0 | (g0 << 5) | (r0 << 11) | (b1 << 16) | (g1 << 21) |
				(r1 << 27));
			dst_rgb += 4;
			src_argb += 8;
		}
		if (width & 1) {
			uint8_t b0 = src_argb[0] >> 3;
			uint8_t g0 = src_argb[1] >> 2;
			uint8_t r0 = src_argb[2] >> 3;
			*(uint16_t*)(dst_rgb) = b0 | (g0 << 5) | (r0 << 11);
		}
	}

	void ARGBToBGR565Row_C(const uint8_t* src_argb, uint8_t* dst_rgb, int width) {
		int x;
		for (x = 0; x < width - 1; x += 2) {
			uint8_t b0 = src_argb[0] >> 3;
			uint8_t g0 = src_argb[1] >> 2;
			uint8_t r0 = src_argb[2] >> 3;
			uint8_t b1 = src_argb[4] >> 3;
			uint8_t g1 = src_argb[5] >> 2;
			uint8_t r1 = src_argb[6] >> 3;
			WRITEWORD(dst_rgb, r0 | (g0 << 5) | (b0 << 11) | (r1 << 16) | (g1 << 21) |
				(b1 << 27));
			dst_rgb += 4;
			src_argb += 8;
		}
		if (width & 1) {
			uint8_t b0 = src_argb[0] >> 3;
			uint8_t g0 = src_argb[1] >> 2;
			uint8_t r0 = src_argb[2] >> 3;
			*(uint16_t*)(dst_rgb) = r0 | (g0 << 5) | (b0 << 11);
		}
	}
};