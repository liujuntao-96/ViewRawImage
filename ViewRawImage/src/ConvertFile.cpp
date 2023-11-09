#include <fstream>
#include <cassert>
#include <SDKDDKVer.h>
#include <afxdlgs.h>

#include "ConvertFile.h"
#include "util/ImgUtils.h"
#include "convert/Convert.h"
#include "convert/ConvertToARGB.h"


namespace convertFile {
	FileConvert::FileConvert(FileParam src, FileParam dst) : src_(src), dst_(dst)
	{
	}
	FileConvert::~FileConvert()
	{
	}
	void FileConvert::Convert()
	{
		uint8_t* src_buffer, * src_bufferT, * temp_buffer, * dst_buffer, * dst_bufferT;
		//uint8_t* src_data[4];
		//int srclinesize[4];
		int src_size = image_get_buffer_size(src_.format, src_.w, src_.h, 1);
		//uint8_t* temp_data[4];
		//int templinesize[4];
		int temp_size = src_.w * src_.h * 4;
		//uint8_t* dst_data[4];
		//int dstlinesize[4];
		int dst_size = image_get_buffer_size(dst_.format, src_.w, src_.h, 1);

		std::ifstream srcFile(src_.path, std::ios::in | std::ios::binary | std::ios::ate);
		if (!srcFile.is_open()) {
			AfxMessageBox(_T("输入文件打开失败！"));
			return;
		}
		else
		{
			if (srcFile.tellg() < src_size)
			{
				srcFile.close();
				AfxMessageBox(_T("文件小于格式一帧的大小！"));
				return;
			}
		}
		srcFile.seekg(0);

		std::ofstream dstFile(dst_.path, std::ios::out | std::ios::binary);
		if (!dstFile.is_open()) {
			AfxMessageBox(_T("输出文件打开失败！"));
			return;
		}

		src_buffer = (uint8_t*) new char[src_size];
		src_bufferT = (uint8_t*) new char[src_size];
		temp_buffer = (uint8_t*) new char[temp_size];
		dst_buffer = (uint8_t*) new char[dst_size];
		dst_bufferT = (uint8_t*) new char[dst_size];
		memset(src_buffer, 0, src_size);
		memset(src_bufferT, 0, src_size);
		memset(temp_buffer, 0, temp_size);
		memset(dst_buffer, 0, dst_size);
		memset(dst_bufferT, 0, dst_size);
		//image_fill_arrays(src_data, srclinesize, src_buffer, src_.format, src_.w, src_.h, 1);
		//image_fill_arrays(temp_data, templinesize, temp_buffer, PIX_FMT_BGRA8888, src_.w, src_.h, 1);
		//image_fill_arrays(dst_data, dstlinesize, dst_buffer, dst_.format, src_.w, src_.h, 1);

		while (srcFile.read((char*)src_buffer, src_size))
		{
			uint8_t* p = src_buffer;
			if (src_.tiled != TILED_LINEAR) {
				convert::ConvertTiled(src_.w, src_.h, src_.tiled, src_.format, p, src_bufferT);
				p = src_bufferT;
			}
			if (src_.format != dst_.format) {
				convert::ConvertToARGB(p, temp_buffer, src_.w, src_.h, src_.format);
				p = temp_buffer;

				convert::ConvertFromARGB(p, dst_buffer, src_.w, src_.h, dst_.format);
				p = dst_buffer;
			}
			if (dst_.tiled != TILED_LINEAR)
			{
				convert::ConvertFromTiled(src_.w, src_.h, dst_.tiled, dst_.format, p, dst_bufferT);
				p = dst_bufferT;
			}
			dstFile.write((char*)p, dst_size);
		}
		srcFile.close();
		dstFile.close();
		delete[] src_buffer;
		delete[] src_bufferT;
		delete[] temp_buffer;
		delete[] dst_buffer;
		delete[] dst_bufferT;
	}
};