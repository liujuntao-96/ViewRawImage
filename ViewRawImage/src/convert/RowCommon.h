#pragma once

namespace convert {
	void YUY2ToARGBRow_C(const uint8_t* src_yuy2, uint8_t* rgb_buf, int width);
	void UYVYToARGBRow_C(const uint8_t* src_uyvy, uint8_t* rgb_buf, int width);
	void I422ToARGBRow_C(const uint8_t* src_y, const uint8_t* src_u, const uint8_t* src_v, uint8_t* rgb_buf, int width);
	void NV12ToARGBRow_C(const uint8_t* src_y, const uint8_t* src_uv, uint8_t* rgb_buf, int width);
	void NV21ToARGBRow_C(const uint8_t* src_y, const uint8_t* src_vu, uint8_t* rgb_buf, int width);
	void NV16ToARGBRow_C(const uint8_t* src_y, const uint8_t* src_uv, uint8_t* rgb_buf, int width);
	void NV61ToARGBRow_C(const uint8_t* src_y, const uint8_t* src_vu, uint8_t* rgb_buf, int width);
	void P210ToARGBRow_C(const uint16_t* src_y, const uint16_t* src_uv, uint8_t* dst_argb, int width, int shift);
	void I210ToARGBRow_C(const uint16_t* src_y, const uint16_t* src_u, const uint16_t* src_v, uint8_t* rgb_buf, int width, int shift);
	void ABGR4444ToARGBRow_C(const uint8_t* src_argb4444, uint8_t* dst_argb, int width);
	void ARGB4444ToARGBRow_C(const uint8_t* src_argb4444, uint8_t* dst_argb, int width);
	void BGRA4444ToARGBRow_C(const uint8_t* src_argb4444, uint8_t* dst_argb, int width);
	void RGBA4444ToARGBRow_C(const uint8_t* src_argb4444, uint8_t* dst_argb, int width);
	void ARGB1555ToARGBRow_C(const uint8_t* src_argb1555, uint8_t* dst_argb, int width);
	void ABGR1555ToARGBRow_C(const uint8_t* src_argb1555, uint8_t* dst_argb, int width);
	void RGBA5551ToARGBRow_C(const uint8_t* src_argb1555, uint8_t* dst_argb, int width);
	void BGRA5551ToARGBRow_C(const uint8_t* src_argb1555, uint8_t* dst_argb, int width);
	void RGB565ToARGBRow_C(const uint8_t* src_rgb565, uint8_t* dst_argb, int width);
	void BGR565ToARGBRow_C(const uint8_t* src_rgb565, uint8_t* dst_argb, int width);
	void ARGBToBGRA1555Row_C(const uint8_t* src_argb, uint8_t* dst_rgb, int width);
	void ARGBToRGBA1555Row_C(const uint8_t* src_argb, uint8_t* dst_rgb, int width);
	void ARGBToARGB1555Row_C(const uint8_t* src_argb, uint8_t* dst_rgb, int width);
	void ARGBToABGR1555Row_C(const uint8_t* src_argb, uint8_t* dst_rgb, int width);
	void ARGBToBGRA4444Row_C(const uint8_t* src_argb, uint8_t* dst_rgb, int width);
	void ARGBToRGBA4444Row_C(const uint8_t* src_argb, uint8_t* dst_rgb, int width);
	void ARGBToABGR4444Row_C(const uint8_t* src_argb, uint8_t* dst_rgb, int width);
	void ARGBToARGB4444Row_C(const uint8_t* src_argb, uint8_t* dst_rgb, int width);
	void ARGBToBGR565Row_C(const uint8_t* src_argb, uint8_t* dst_rgb, int width);
	void ARGBToRGB565Row_C(const uint8_t* src_argb, uint8_t* dst_rgb, int width);
};