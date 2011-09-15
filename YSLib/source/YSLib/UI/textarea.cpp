﻿/*
	Copyright (C) by Franksoft 2011.

	This file is part of the YSLib project, and may only be used,
	modified, and distributed under the terms of the YSLib project
	license, LICENSE.TXT.  By continuing to use, modify, or distribute
	this file you indicate that you have read the license and
	understand and accept it fully.
*/

/*!	\file textarea.cpp
\ingroup UI
\brief 样式无关的用户界面文本区域部件。
\version r1328;
\author FrankHB<frankhb1989@gmail.com>
\par 创建时间:
	2011-06-30 20:10:27 +0800;
\par 修改时间:
	2011-09-14 08:39 +0800;
\par 字符集:
	UTF-8;
\par 模块名称:
	YSLib::UI::TextArea;
*/


#include "textarea.h"

YSL_BEGIN

YSL_BEGIN_NAMESPACE(Components)

TextArea::TextArea(const Rect& r, FontCache& fc)
	: Widget(r), TextRegion(fc),
	Rotation(Drawing::RDeg0)
{
	//初始化视图。
	TextRegion::SetSize(GetWidth(), GetHeight());
}

Rect
TextArea::Refresh(const PaintEventArgs& e)
{
	Widget::Refresh(e);

	const auto& g(e.Target);
	const auto& pt(e.Location);
	const auto& r(e.ClipArea);

	BlitTo(g.GetBufferPtr(), *this, g.GetSize(),
		r, static_cast<const Point&>(r) - pt, r, Rotation);
	return GetBoundsOf(*this);
}

YSL_END_NAMESPACE(Components)

YSL_END

