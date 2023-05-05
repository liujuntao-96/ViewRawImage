#include <cstdint>

namespace convert {
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

	void ARGB1555ToARGBRow_C(const uint8_t* src_argb1555,
		uint8_t* dst_argb,
		int width) {
		int x;
		for (x = 0; x < width; ++x) {
			uint8_t a = src_argb1555[0] & 0x1;
			uint8_t r = (src_argb1555[0] >> 1) & 0x1f;
			uint8_t g = (src_argb1555[1] & 0x7) << 3 | (src_argb1555[0] & 0x3);
			uint8_t b = src_argb1555[1] >> 3;
			//uint8_t b = src_argb1555[0] & 0x1f;
			//uint8_t g = (src_argb1555[0] >> 5) | ((src_argb1555[1] & 0x03) << 3);
			//uint8_t r = (src_argb1555[1] & 0x7c) >> 2;
			//uint8_t a = src_argb1555[1] >> 7;

			dst_argb[0] = (b << 3) | (b >> 2);
			dst_argb[1] = (g << 3) | (g >> 2);
			dst_argb[2] = (r << 3) | (r >> 2);
			dst_argb[3] = -a;
			dst_argb += 4;
			src_argb1555 += 2;
		}
	}
};