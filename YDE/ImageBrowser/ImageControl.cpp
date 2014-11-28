﻿/*
	© 2013-2014 FrankHB.

	This file is part of the YSLib project, and may only be used,
	modified, and distributed under the terms of the YSLib project
	license, LICENSE.TXT.  By continuing to use, modify, or distribute
	this file you indicate that you have read the license and
	understand and accept it fully.
*/

/*!	\file ImageControl.cpp
\ingroup UI
\brief 图像显示控件。
\version r898
\author FrankHB <frankhb1989@gmail.com>
\since build 436
\par 创建时间:
	2013-08-13 12:48:27 +0800
\par 修改时间:
	2014-11-27 12:40 +0800
\par 文本编码:
	UTF-8
\par 模块名称:
	ImageBrowser::ImageControl
*/


#include "ImageControl.h"
#include <Windows.h> // for VK_MBUTTON;

namespace ImageBrowser
{

ImagePanel::ImagePanel(const Rect& r, const Size& min_size,
	const Size& max_size)
	: Panel(r),
	min_panel_size(min_size), max_panel_size(max_size), btnClose({{}, 24, 24}),
	border(*this, 8, min_size)
{
	border.HostMode = true;
	*this += btnClose,
	yunseq(
	Background = SolidBrush({0x00, 0x00, 0x00, 0xC0}),
	btnClose.Background = [this](PaintEventArgs&& e){
		const auto& g(e.Target);
		const auto& pt(e.Location);
		auto& r(e.ClipArea);

		if(hover_updater.IsEntered())
			FillCircle(g, r, pt + Vec(12, 12), 10, {225, 96, 76});
		DrawCross(g, e.ClipArea, {pt + Vec(4, 4), {16, 16}}, ColorSpace::White);
		r = Rect(pt, GetSizeOf(e.GetSender()));
	},
	FetchEvent<Resize>(*this) += [this]{
		SetLocationOf(btnClose, CalcCloseButtonLocation());
		if(session_ptr)
		{
			GetPagesRef().Resize(GetSizeOf(*this));
			Invalidate(*this);
		}
	},
	FetchEvent<Click>(*this) += [this](CursorEventArgs&& e){
		if(session_ptr && e.Keys[VK_MBUTTON] && GetPagesRef().Zoom(
			round((1 - GetPagesRef().GetScale()) * 100), e))
			Invalidate(*this);
	},
	FetchEvent<CursorWheel>(*this) += [this](CursorWheelEventArgs&& e){
		if(session_ptr
			&& GetPagesRef().ZoomByRatio(e.GetDelta() > 0 ? 1.2F : 0.8F, e))
			Invalidate(*this);
	},
	FetchEvent<Paint>(*this).Add([this](PaintEventArgs&& e){
		if(session_ptr)
			GetPagesRef().Brush(std::move(e));
	}, BackgroundPriority),
	FetchEvent<Click>(btnClose) += []{
		YSLib::PostQuitMessage(0);
	}
	);
	SetLocationOf(btnClose, CalcCloseButtonLocation());
}

void
ImagePanel::Load(const std::wstring& path)
{
	session_ptr.reset(new Session(forward_as_tuple(ImagePages(reinterpret_cast<
		const ucs2_t*>(&path[0]), min_panel_size, max_panel_size),
		GAnimationSession<InvalidationUpdater>(),
		Timers::Timer(std::chrono::milliseconds(1000/12)))));
	auto& pages(GetPagesRef());

	SetSizeOf(*this, pages.GetViewSize());
	Invalidate(*this);

	const auto n(pages.GetPagesNum());

	YTraceDe(Notice, "Loaded page count = %u.", unsigned(n));
	if(n > 1)
		UI::SetupByTimer(get<1>(*session_ptr), *this, get<2>(*session_ptr),
			[this]{
			if(GetPagesRef().SwitchPageDiff(1))
				Invalidate(*this);
		});
}

void
ImagePanel::Unload()
{
	session_ptr.reset();
}

} // namespace ImageBrowser;

