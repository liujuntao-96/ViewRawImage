#pragma once

#include <string>
#include "util/ImgUtils.h"

namespace convertFile {
	class FileConvert
	{
	public:
		struct FileParam {
			std::string path;
			int w;
			int h;
			enum PixelFormat format;
			enum Tiled tiled;
		};
		FileConvert(FileParam src, FileParam dst);
		~FileConvert();

		void Convert();
	private:
		FileParam src_;
		FileParam dst_;
	};

};
