#pragma once
#ifdef __cplusplus
extern "C" {
#endif
	enum PixelFormat {
		PIX_FMT_NONE = -1,
		PIX_FMT_RGBA8888,
		PIX_FMT_R5G5B5A1,
		PIX_FMT_RGBA4444,
		PIX_FMT_BGR565,
		PIX_FMT_BGRA8888,
		PIX_FMT_BGRX8888,
		PIX_FMT_B10G10R10A2,
		PIX_FMT_B5G5R5A1,
		PIX_FMT_B5G5R5X1,
		PIX_FMT_BGRA4444,
		PIX_FMT_BGRX4444,
		PIX_FMT_RGB565,
		PIX_FMT_ABGR8888,
		PIX_FMT_A1B5G5R5,
		PIX_FMT_ABGR4444,
		PIX_FMT_ARGB8888,
		PIX_FMT_A1R5G5B5,
		PIX_FMT_ARGB4444,
		PIX_FMT_YUYV,
		PIX_FMT_UYVY,
		PIX_FMT_YV12,
		PIX_FMT_I420,
		PIX_FMT_NV12,
		PIX_FMT_NV21,
		PIX_FMT_NV16,
		PIX_FMT_NV61,
		PIX_FMT_P010MSB,
		PIX_FMT_P010LSB,
		PIX_FMT_I010,
		PIX_FMT_BGR888Planar,
		PIX_FMT_NB
	};

	enum Tiled {
		TILED_NONE = -1,
		TILED_LINEAR,
		TILED_2X2,
		TILED_4X4,
		TILED_NB
	};
#ifdef __cplusplus
}
#endif