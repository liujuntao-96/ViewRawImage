#include <limits.h>
#include <string.h>

#include "ImgUtils.h"
#include "Common.h"

#ifdef __cplusplus
extern "C" {
#endif

	int image_fill_pointers(uint8_t* data[4], enum PixelFormat pix_fmt, int height,
		uint8_t* ptr, const int linesizes[4])
	{
		int i, total_size, size[4] = { 0 }, has_plane[4] = { 0 };

		const PixFmtDescriptor* desc = pix_fmt_desc_get(pix_fmt);
		memset(data, 0, sizeof(data[0]) * 4);

		if (!desc)
			return -1;

		data[0] = ptr;
		if (linesizes[0] > (INT_MAX - 1024) / height)
			return -1;
		size[0] = linesizes[0] * height;

		//if (desc->flags & PIX_FMT_FLAG_PAL ||
		//	desc->flags & FF_PSEUDOPAL) {
		//	data[1] = ptr + size[0]; /* palette is stored here as 256 32 bits words */
		//	return size[0] + 256 * 4;
		//}

		for (i = 0; i < 4; i++)
			has_plane[desc->comp[i].plane] = 1;

		total_size = size[0];
		for (i = 1; i < 4 && has_plane[i]; i++) {
			int h, s = (i == 1 || i == 2) ? desc->log2_chroma_h : 0;
			data[i] = data[i - 1] + size[i - 1];
			h = (height + (1 << s) - 1) >> s;
			if (linesizes[i] > INT_MAX / h)
				return -1;
			size[i] = h * linesizes[i];
			if (total_size > INT_MAX - size[i])
				return -1;
			total_size += size[i];
		}

		return total_size;
	}

	static inline
		int image_get_linesize(int width, int plane,
			int max_step, int max_step_comp,
			const PixFmtDescriptor* desc)
	{
		int s, shifted_w, linesize;

		if (!desc)
			return -1;

		if (width < 0)
			return -1;
		s = (max_step_comp == 1 || max_step_comp == 2) ? desc->log2_chroma_w : 0;
		shifted_w = ((width + (1 << s) - 1)) >> s;
		if (shifted_w && max_step > INT_MAX / shifted_w)
			return -1;
		linesize = max_step * shifted_w;

		/*	if (desc->flags & PIX_FMT_FLAG_BITSTREAM)
				linesize = (linesize + 7) >> 3;*/
		return linesize;
	}

	void image_fill_max_pixsteps(int max_pixsteps[4], int max_pixstep_comps[4],
		const PixFmtDescriptor* pixdesc)
	{
		int i;
		memset(max_pixsteps, 0, 4 * sizeof(max_pixsteps[0]));
		if (max_pixstep_comps)
			memset(max_pixstep_comps, 0, 4 * sizeof(max_pixstep_comps[0]));

		for (i = 0; i < 4; i++) {
			const ComponentDescriptor* comp = &(pixdesc->comp[i]);
			if (comp->step > max_pixsteps[comp->plane]) {
				max_pixsteps[comp->plane] = comp->step;
				if (max_pixstep_comps)
					max_pixstep_comps[comp->plane] = i;
			}
		}
	}

	int image_fill_linesizes(int linesizes[4], enum PixelFormat pix_fmt, int width)
	{
		int i, ret;
		const PixFmtDescriptor* desc = pix_fmt_desc_get(pix_fmt);
		int max_step[4];       /* max pixel step for each plane */
		int max_step_comp[4];       /* the component for each plane which has the max pixel step */

		memset(linesizes, 0, 4 * sizeof(linesizes[0]));

		if (!desc)
			return -1;

		image_fill_max_pixsteps(max_step, max_step_comp, desc);
		for (i = 0; i < 4; i++) {
			if ((ret = image_get_linesize(width, i, max_step[i], max_step_comp[i], desc)) < 0)
				return ret;
			linesizes[i] = ret;
		}

		return 0;
	}

	int image_fill_arrays(uint8_t* dst_data[4], int dst_linesize[4],
		const uint8_t* src, enum PixelFormat pix_fmt,
		int width, int height, int align)
	{
		int ret, i;

		ret = image_fill_linesizes(dst_linesize, pix_fmt, width);
		if (ret < 0)
			return ret;

		for (i = 0; i < 4; i++)
			dst_linesize[i] = ALIGN(dst_linesize[i], align);

		return image_fill_pointers(dst_data, pix_fmt, height, (uint8_t*)src, dst_linesize);
	}


	int image_get_buffer_size(enum PixelFormat pix_fmt,
		int width, int height, int align)
	{
		uint8_t* data[4];
		int linesize[4];
		const PixFmtDescriptor* desc = pix_fmt_desc_get(pix_fmt);
		if (!desc)
			return -1;


		// do not include palette for these pseudo-paletted formats
		//if (desc->flags & FF_PSEUDOPAL)
		//	return FFALIGN(width, align) * height;

		return image_fill_arrays(data, linesize, NULL, pix_fmt,
			width, height, align);
	}

#ifdef __cplusplus
}
#endif