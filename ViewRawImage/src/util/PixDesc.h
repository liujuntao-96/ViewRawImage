#pragma once
#include <inttypes.h>
#include "PixFmt.h"
#ifdef __cplusplus
extern "C" {
#endif

#define PIX_FMT_FLAG_PLANAR       (1 << 0)

#define PIX_FMT_FLAG_RGB          (1 << 1)

#define PIX_FMT_FLAG_4444		  (1 << 2)

	typedef struct ComponentDescriptor {
		int plane;
		int step;
		int depth;
	} ComponentDescriptor;

	typedef struct PixFmtDescriptor {
		const char* name;
		uint8_t nb_components;
		uint8_t log2_chroma_w;
		uint8_t log2_chroma_h;
		uint64_t flags;
		ComponentDescriptor comp[4];
	} PixFmtDescriptor;

	typedef struct TiledDescriptor {
		const char* name;
		int align;
	}TiledDescriptor;

	const TiledDescriptor* tiled_desc_get(enum Tiled tiled);

	const PixFmtDescriptor* pix_fmt_desc_get(enum PixelFormat pix_fmt);

	enum PixelFormat pix_fmt_desc_get_id(const PixFmtDescriptor* desc);

	int pix_fmt_count_planes(enum PixelFormat pix_fmt);
#ifdef __cplusplus
}
#endif