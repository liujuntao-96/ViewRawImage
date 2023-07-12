#include "PixDesc.h"
#include "Common.h"
#ifdef __cplusplus
extern "C" {
#endif
	static const PixFmtDescriptor pix_fmt_descriptors[PIX_FMT_NB] = {
		[PIX_FMT_RGBA8888] = {
		   .name = "RGBA8888",
		   .nb_components = 4,
		   .log2_chroma_w = 0,
		   .log2_chroma_h = 0,
		   .flags = PIX_FMT_FLAG_RGB,
		   .comp = {
			   { 0, 4, 8 },
			   { 0, 4, 8 },
			   { 0, 4, 8 },
			   { 0, 4, 8 },
			}
		},
		[PIX_FMT_R5G5B5A1] = {
		   .name = "R5G5B5A1",
		   .nb_components = 4,
		   .log2_chroma_w = 0,
		   .log2_chroma_h = 0,
		   .flags = PIX_FMT_FLAG_RGB,
		   .comp = {
			   { 0, 2, 5 },
			   { 0, 2, 5 },
			   { 0, 2, 5 },
			   { 0, 2, 1 },
			}
		},
		[PIX_FMT_RGBA4444] = {
		   .name = "RGBA4444",
		   .nb_components = 4,
		   .log2_chroma_w = 0,
		   .log2_chroma_h = 0,
		   .flags = PIX_FMT_FLAG_RGB | PIX_FMT_FLAG_4444,
		   .comp = {
			   { 0, 2, 4 },
			   { 0, 2, 4 },
			   { 0, 2, 4 },
			   { 0, 2, 4 },
			}
		},
		[PIX_FMT_BGR565] = {
		   .name = "BGR565",
		   .nb_components = 3,
		   .log2_chroma_w = 0,
		   .log2_chroma_h = 0,
		   .flags = PIX_FMT_FLAG_RGB,
		   .comp = {
			   { 0, 2, 5 },
			   { 0, 2, 6 },
			   { 0, 2, 5 },
			}
		},
		[PIX_FMT_BGRA8888] = {
		   .name = "BGRA8888",
		   .nb_components = 4,
		   .log2_chroma_w = 0,
		   .log2_chroma_h = 0,
		   .flags = PIX_FMT_FLAG_RGB,
		   .comp = {
			   { 0, 4, 8 },
			   { 0, 4, 8 },
			   { 0, 4, 8 },
			   { 0, 4, 8 },
			}
		},
		[PIX_FMT_BGRX8888] = {
		   .name = "BGRX8888",
		   .nb_components = 4,
		   .log2_chroma_w = 0,
		   .log2_chroma_h = 0,
		   .flags = PIX_FMT_FLAG_RGB,
		   .comp = {
			   { 0, 4, 8 },
			   { 0, 4, 8 },
			   { 0, 4, 8 },
			}
		},
		[PIX_FMT_B10G10R10A2] = {
		   .name = "B10G10R10A2",
		   .nb_components = 4,
		   .log2_chroma_w = 0,
		   .log2_chroma_h = 0,
		   .flags = PIX_FMT_FLAG_RGB,
		   .comp = {
			   { 0, 4, 10 },
			   { 0, 4, 10 },
			   { 0, 4, 10 },
			   { 0, 4, 2 },
			}
		},
		[PIX_FMT_B5G5R5A1] = {
		   .name = "B5G5R5A1",
		   .nb_components = 4,
		   .log2_chroma_w = 0,
		   .log2_chroma_h = 0,
		   .flags = PIX_FMT_FLAG_RGB,
		   .comp = {
			   { 0, 2, 5 },
			   { 0, 2, 5 },
			   { 0, 2, 5 },
			   { 0, 2, 1 },
			}
		},
		[PIX_FMT_B5G5R5X1] = {
			.name = "B5G5R5X1",
			.nb_components = 3,
			.log2_chroma_w = 0,
			.log2_chroma_h = 0,
			.flags = PIX_FMT_FLAG_RGB,
			.comp = {
				{ 0, 2, 8 },
				{ 0, 2, 8 },
				{ 0, 2, 8 },
			}
		},
		[PIX_FMT_BGRA4444] = {
			.name = "BGRA4444",
			.nb_components = 4,
			.log2_chroma_w = 0,
			.log2_chroma_h = 0,
			.flags = PIX_FMT_FLAG_RGB | PIX_FMT_FLAG_4444,
			.comp = {
				{ 0, 2, 4 },
				{ 0, 2, 4 },
				{ 0, 2, 4 },
				{ 0, 2, 4 },
			}
		},
		[PIX_FMT_BGRX4444] = {
			.name = "BGRX4444",
			.nb_components = 3,
			.log2_chroma_w = 0,
			.log2_chroma_h = 0,
			.flags = PIX_FMT_FLAG_RGB | PIX_FMT_FLAG_4444,
			.comp = {
				{ 0, 2, 4 },
				{ 0, 2, 4 },
				{ 0, 2, 4 },
			}
		},
		[PIX_FMT_RGB565] = {
			.name = "RGB565",
			.nb_components = 3,
			.log2_chroma_w = 0,
			.log2_chroma_h = 0,
			.flags = PIX_FMT_FLAG_RGB,
	  .comp = {
		  { 0, 2, 5 },
		  { 0, 2, 6 },
		  { 0, 2, 5 },
	   }
		   },
			   [PIX_FMT_ABGR8888] = {
	  .name = "ABGR8888",
	  .nb_components = 4,
	  .log2_chroma_w = 0,
	  .log2_chroma_h = 0,
	  .flags = PIX_FMT_FLAG_RGB,
	  .comp = {
		  { 0, 4, 8 },
		  { 0, 4, 8 },
		  { 0, 4, 8 },
		  { 0, 4, 8 },
	   }
		   },
			   [PIX_FMT_A1B5G5R5] = {
	  .name = "A1B5G5R5",
	  .nb_components = 4,
	  .log2_chroma_w = 0,
	  .log2_chroma_h = 0,
	  .flags = PIX_FMT_FLAG_RGB,
	  .comp = {
		  { 0, 2, 5 },
		  { 0, 2, 5 },
		  { 0, 2, 5 },
		  { 0, 2, 1 },
	   }
		   },
			   [PIX_FMT_ABGR4444] = {
	  .name = "ABGR4444",
	  .nb_components = 4,
	  .log2_chroma_w = 0,
	  .log2_chroma_h = 0,
	  .flags = PIX_FMT_FLAG_RGB | PIX_FMT_FLAG_4444,
	  .comp = {
		  { 0, 2, 4 },
		  { 0, 2, 4 },
		  { 0, 2, 4 },
		  { 0, 2, 4 },
	   }
		   },
			   [PIX_FMT_ARGB8888] = {
	  .name = "ARGB8888",
	  .nb_components = 4,
	  .log2_chroma_w = 0,
	  .log2_chroma_h = 0,
	  .flags = PIX_FMT_FLAG_RGB,
	  .comp = {
		  { 0, 4, 8 },
		  { 0, 4, 8 },
		  { 0, 4, 8 },
		  { 0, 4, 8 },
	   }
		   },
			   [PIX_FMT_A1R5G5B5] = {
	  .name = "A1R5G5B5",
	  .nb_components = 4,
	  .log2_chroma_w = 0,
	  .log2_chroma_h = 0,
	  .flags = PIX_FMT_FLAG_RGB,
	  .comp = {
		  { 0, 2, 5 },
		  { 0, 2, 5 },
		  { 0, 2, 5 },
		  { 0, 2, 1 },
	   }
		   },
			   [PIX_FMT_ARGB4444] = {
	  .name = "ARGB4444",
	  .nb_components = 4,
	  .log2_chroma_w = 0,
	  .log2_chroma_h = 0,
	  .flags = PIX_FMT_FLAG_RGB | PIX_FMT_FLAG_4444,
	  .comp = {
		  { 0, 2, 4 },
		  { 0, 2, 4 },
		  { 0, 2, 4 },
		  { 0, 2, 4 },
	   }
		   },
		  [PIX_FMT_YUYV] = {
	 .name = "YUYV",
	 .nb_components = 3,
	 .log2_chroma_w = 1,
	 .log2_chroma_h = 0,
	 .flags = 0,
	 .comp = {
		 { 0, 2, 8 },
		 { 0, 4, 8 },
		 { 0, 4, 8 },
	  }
	  },
		  [PIX_FMT_UYVY] = {
	 .name = "UYVY",
	 .nb_components = 3,
	 .log2_chroma_w = 1,
	 .log2_chroma_h = 0,
	 .flags = 0,
	 .comp = {
		 { 0, 2, 8 },
		 { 0, 4, 8 },
		 { 0, 4, 8 },
	  }
	  },
		  [PIX_FMT_YV12] = {
	 .name = "YV12",
	 .nb_components = 3,
	 .log2_chroma_w = 1,
	 .log2_chroma_h = 1,
	 .flags = PIX_FMT_FLAG_PLANAR,
	 .comp = {
		 { 0, 1, 8 },
		 { 2, 1, 8 },
		 { 1, 1, 8 },
	  }
	  },
		  [PIX_FMT_I420] = {
	 .name = "I420",
	 .nb_components = 3,
	 .log2_chroma_w = 1,
	 .log2_chroma_h = 1,
	 .flags = PIX_FMT_FLAG_PLANAR,
	 .comp = {
		 { 0, 1, 8 },
		 { 1, 1, 8 },
		 { 2, 1, 8 },
	  }
	  },
		  [PIX_FMT_NV12] = {
	 .name = "NV12",
	 .nb_components = 3,
	 .log2_chroma_w = 1,
	 .log2_chroma_h = 1,
	 .flags = PIX_FMT_FLAG_PLANAR,
	 .comp = {
		 { 0, 1, 8 },
		 { 1, 2, 8 },
		 { 1, 2, 8 },
	  }
	  },
		  [PIX_FMT_NV21] = {
	 .name = "NV21",
	 .nb_components = 3,
	 .log2_chroma_w = 1,
	 .log2_chroma_h = 1,
	 .flags = PIX_FMT_FLAG_PLANAR,
	 .comp = {
		 { 0, 1, 8 },
		 { 1, 2, 8 },
		 { 1, 2, 8 },
	  }
	  },
		  [PIX_FMT_NV16] = {
	 .name = "NV16",
	 .nb_components = 3,
	 .log2_chroma_w = 1,
	 .log2_chroma_h = 0,
	 .flags = PIX_FMT_FLAG_PLANAR,
	 .comp = {
		 { 0, 1, 8 },
		 { 1, 2, 8 },
		 { 1, 2, 8 },
	  }
	  },
		  [PIX_FMT_NV61] = {
	 .name = "NV61",
	 .nb_components = 3,
	 .log2_chroma_w = 1,
	 .log2_chroma_h = 0,
	 .flags = PIX_FMT_FLAG_PLANAR,
	 .comp = {
		 { 0, 1, 8 },
		 { 1, 2, 8 },
		 { 1, 2, 8 },
	  }
	  },
		 [PIX_FMT_P010MSB] = {
	.name = "P010MSB",
	.nb_components = 3,
	.log2_chroma_w = 1,
	.log2_chroma_h = 1,
	.flags = PIX_FMT_FLAG_PLANAR,
	.comp = {
		{ 0, 2, 10 },
		{ 1, 4, 10 },
		{ 1, 4, 10 },
	 }
	 },
		 [PIX_FMT_P010LSB] = {
	.name = "P010LSB",
	.nb_components = 3,
	.log2_chroma_w = 1,
	.log2_chroma_h = 1,
	.flags = PIX_FMT_FLAG_PLANAR,
	.comp = {
		{ 0, 2, 10 },
		{ 1, 4, 10 },
		{ 1, 4, 10 },
	 }
	 },
		 [PIX_FMT_I010] = {
	.name = "I010",
	.nb_components = 3,
	.log2_chroma_w = 1,
	.log2_chroma_h = 1,
	.flags = PIX_FMT_FLAG_PLANAR,
	.comp = {
		{ 0, 2, 10 },
		{ 1, 2, 10 },
		{ 2, 2, 10 },
	 }
	 },
		 [PIX_FMT_BGR888Planar] = {
	.name = "BGR888Planar",
	.nb_components = 3,
	.log2_chroma_w = 0,
	.log2_chroma_h = 0,
	.flags = PIX_FMT_FLAG_RGB | PIX_FMT_FLAG_PLANAR,
	.comp = {
		{ 0, 1, 8 },
		{ 1, 1, 8 },
		{ 2, 1, 8 },
	 }
	 },
		  [PIX_FMT_R10G10B10A2] = {
	 .name = "R10G10B10A2",
	 .nb_components = 4,
	 .log2_chroma_w = 0,
	 .log2_chroma_h = 0,
	 .flags = PIX_FMT_FLAG_RGB,
	 .comp = {
		 { 0, 4, 10 },
		 { 0, 4, 10 },
		 { 0, 4, 10 },
		 { 0, 4, 2 },
	  }
	  },

		  [PIX_FMT_RGB24] = {
 .name = "RGB24",
 .nb_components = 3,
 .log2_chroma_w = 0,
 .log2_chroma_h = 0,
 .flags = PIX_FMT_FLAG_RGB,
 .comp = {
	 { 0, 3, 8 },
	 { 0, 3, 8 },
	 { 0, 3, 8 },
  }
	  },

		  [PIX_FMT_BGR24] = {
.name = "BGR24",
.nb_components = 3,
.log2_chroma_w = 0,
.log2_chroma_h = 0,
.flags = PIX_FMT_FLAG_RGB,
.comp = {
	{ 0, 3, 8 },
	{ 0, 3, 8 },
	{ 0, 3, 8 },
 }
	  },


		 [PIX_FMT_RGB888Planar] = {
.name = "RGB888Planar",
.nb_components = 3,
.log2_chroma_w = 0,
.log2_chroma_h = 0,
.flags = PIX_FMT_FLAG_RGB | PIX_FMT_FLAG_PLANAR,
.comp = {
	{ 0, 1, 8 },
	{ 1, 1, 8 },
	{ 2, 1, 8 },
 }
	 },

	};


	static const TiledDescriptor tiled_descriptors[TILED_NB] = {
		[TILED_LINEAR] = {
			.name = "TILED_LINEAR",
			.align = 1
		},
		[TILED_2X2] = {
			.name = "TILED_2X2",
			.align = 2
		},
		[TILED_4X4] = {
			.name = "TILED_4X4",
			.align = 4
		}
	};

	const TiledDescriptor* tiled_desc_get(enum Tiled tiled)
	{
		if (tiled < 0 || tiled >= TILED_NB)
			return NULL;
		return &tiled_descriptors[tiled];
	}

	const PixFmtDescriptor* pix_fmt_desc_get(enum PixelFormat pix_fmt)
	{
		if (pix_fmt < 0 || pix_fmt >= PIX_FMT_NB)
			return NULL;
		return &pix_fmt_descriptors[pix_fmt];
	}

	enum PixelFormat pix_fmt_desc_get_id(const PixFmtDescriptor* desc)
	{
		if (desc < pix_fmt_descriptors ||
			desc >= pix_fmt_descriptors + ARRAY_ELEMS(pix_fmt_descriptors))
			return PIX_FMT_NONE;

		return desc - pix_fmt_descriptors;
	}

	int pix_fmt_count_planes(enum PixelFormat pix_fmt)
	{
		const PixFmtDescriptor* desc = pix_fmt_desc_get(pix_fmt);
		int i, planes[4] = { 0 }, ret = 0;

		if (!desc)
			return -1;

		for (i = 0; i < desc->nb_components; i++)
			planes[desc->comp[i].plane] = 1;
		for (i = 0; i < ARRAY_ELEMS(planes); i++)
			ret += planes[i];
		return ret;
	}

#ifdef __cplusplus
}
#endif

