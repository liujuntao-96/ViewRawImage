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
};