﻿/*
	Copyright (C) by Franksoft 2012.

	This file is part of the YSLib project, and may only be used,
	modified, and distributed under the terms of the YSLib project
	license, LICENSE.TXT.  By continuing to use, modify, or distribute
	this file you indicate that you have read the license and
	understand and accept it fully.
*/

/*!	\file YBrush.h
\ingroup UI
\brief 图形用户界面画刷。
\version r1184;
\author FrankHB<frankhb1989@gmail.com>
\since build 293 。
\par 创建时间:
	2012-01-10 19:55:30 +0800;
\par 修改时间:
	2012-03-16 16:15 +0800;
\par 文本编码:
	UTF-8;
\par 模块名称:
	YSLib::UI::Border;
*/


#ifndef YSL_INC_UI_BORDER_H_
#define YSL_INC_UI_BORDER_H_

#include "ywgtevt.h"
#include "../Service/yres.h"

YSL_BEGIN

YSL_BEGIN_NAMESPACE(Components)

/*!
\brief 单色画刷。
\since build 293 。
*/
class SolidBrush
{
public:
	Drawing::Color Color;

	SolidBrush(Drawing::Color);

	void
	operator()(PaintEventArgs&&);
};

inline
SolidBrush::SolidBrush(Drawing::Color c)
	: Color(c)
{}


/*!
\brief 图像画刷。
\since build 294 。
*/
class ImageBrush
{
public:
	mutable shared_ptr<Drawing::Image> Image;

	yconstfn DefDeCtor(ImageBrush)
	ImageBrush(const shared_ptr<Drawing::Image>&);
	ImageBrush(shared_ptr<Drawing::Image>&&);
	DefDeCopyCtor(ImageBrush)
	DefDeMoveCtor(ImageBrush)

	DefDeCopyAssignment(ImageBrush)
	DefDeMoveAssignment(ImageBrush)

	void
	operator()(PaintEventArgs&&);
};

inline
ImageBrush::ImageBrush(const shared_ptr<Drawing::Image>& h)
	: Image(h)
{}
inline
ImageBrush::ImageBrush(shared_ptr<Drawing::Image>&& h)
	: Image(std::move(h))
{}


/*!
\brief 边框样式。
\warning 非虚析构。
\since build 276 。
*/
class BorderStyle
{
public:
	Color ActiveColor, InactiveColor;

	BorderStyle();
};


/*!
\brief 边框画刷。
\warning 非虚析构。
\since build 284 。
*/
class BorderBrush
{
public:
	weak_ptr<BorderStyle> StylePtr;

	yconstexpr DefDeCtor(BorderBrush)
	BorderBrush(const shared_ptr<BorderStyle>&);
	BorderBrush(shared_ptr<BorderStyle>&&);
	BorderBrush(const weak_ptr<BorderStyle>&);
	BorderBrush(weak_ptr<BorderStyle>&&);
	inline DefDeCopyCtor(BorderBrush)
	BorderBrush(BorderBrush&&);

	inline DefDeCopyAssignment(BorderBrush)
	inline DefDeMoveAssignment(BorderBrush)

	void
	operator()(PaintEventArgs&&);
};

inline
BorderBrush::BorderBrush(const shared_ptr<BorderStyle>& p)
	: StylePtr(p)
{}
inline
BorderBrush::BorderBrush(shared_ptr<BorderStyle>&& p)
	: StylePtr(std::move(p))
{}
inline
BorderBrush::BorderBrush(const weak_ptr<BorderStyle>& p)
	: StylePtr(p)
{}
inline
BorderBrush::BorderBrush(weak_ptr<BorderStyle>&& p)
	: StylePtr(std::move(p))
{}
inline
BorderBrush::BorderBrush(BorderBrush&& bdb)
	: StylePtr(bdb.StylePtr.lock())
{}

YSL_END_NAMESPACE(Components)

YSL_END

#endif

