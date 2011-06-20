﻿/*
	Copyright (C) by Franksoft 2010 - 2011.

	This file is part of the YSLib project, and may only be used,
	modified, and distributed under the terms of the YSLib project
	license, LICENSE.TXT.  By continuing to use, modify, or distribute
	this file you indicate that you have read the license and
	understand and accept it fully.
*/

/*!	\file form.h
\ingroup UI
\brief 样式无关的图形用户界面窗体。
\version 0.1534;
\author FrankHB<frankhb1989@gmail.com>
\par 创建时间:
	2010-04-30 00:51:36 +0800;
\par 修改时间:
	2011-06-16 19:53 +0800;
\par 字符集:
	UTF-8;
\par 模块名称:
	YSLib::UI::Form;
*/


#ifndef YSL_INC_UI_FORM_H_
#define YSL_INC_UI_FORM_H_

#include "ywindow.h"

YSL_BEGIN

YSL_BEGIN_NAMESPACE(Components)

YSL_BEGIN_NAMESPACE(Forms)

//窗体。
class Form : public Frame
{
//	Widgets::YUIContainer Client;

protected:

public:
	/*!
	\brief 构造：使用指定边界、背景图像、窗口指针和 Shell 句柄。
	*/
	explicit
	Form(const Rect& = Rect::Empty,
		const shared_ptr<Drawing::Image>& = share_raw(new Drawing::Image()),
		IWindow* = nullptr);
	/*!
	\brief 析构：空实现。
	*/
	virtual
	~Form()
	{}

/*	virtual void
	SetSize(SDst, SDst);

	virtual void
	Paint();
*/
};

YSL_END_NAMESPACE(Forms)

YSL_END_NAMESPACE(Components)

YSL_END

#endif
