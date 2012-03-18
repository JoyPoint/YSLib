﻿/*
	Copyright (C) by Franksoft 2009 - 2012.

	This file is part of the YSLib project, and may only be used,
	modified, and distributed under the terms of the YSLib project
	license, LICENSE.TXT.  By continuing to use, modify, or distribute
	this file you indicate that you have read the license and
	understand and accept it fully.
*/

/*!	\file cstdio.cpp
\ingroup YStandardEx
\brief YCLib C++ 标准库扩展。
\version r1070;
\author FrankHB<frankhb1989@gmail.com>
\since build 245 。
\par 创建时间:
	2011-09-21 08:38:51 +0800;
\par 修改时间:
	2012-03-17 19:05 +0800;
\par 文本编码:
	UTF-8;
\par 模块名称:
	YStandardEx::CStandardIO;
*/


#include "ystdex/cstdio.h"

namespace ystdex
{
	bool
	fexists(const_path_t path)
	{
		std::FILE* file(std::fopen(path, "rb"));

		if(file)
		{
			std::fclose(file);
			return true;
		}
		return false;
	}


	ifile_iterator&
	ifile_iterator::operator++()
	{
		assert(stream);

		const auto val(std::fgetc(stream));

		if(YCL_UNLIKELY(val == EOF))
			stream = nullptr;
		else
			value = val;
		return *this;
	}
}

