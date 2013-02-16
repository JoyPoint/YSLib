﻿/*
	Copyright by FrankHB 2013.

	This file is part of the YSLib project, and may only be used,
	modified, and distributed under the terms of the YSLib project
	license, LICENSE.TXT.  By continuing to use, modify, or distribute
	this file you indicate that you have read the license and
	understand and accept it fully.
*/

/*!	\file Host.h
\ingroup Helper
\brief 宿主环境。
\version r233
\author FrankHB <frankhb1989@gmail.com>
\since build 379
\par 创建时间:
	2013-02-08 01:28:03 +0800
\par 修改时间:
	2013-02-16 05:45 +0800
\par 文本编码:
	UTF-8
\par 非公开模块名称:
	Helper::Host
*/


#ifndef Inc_Helper_Host_h_
#define Inc_Helper_Host_h_ 1

#include "Helper/DSMain.h" // for Devices::DSScreen, Host::Window;
#include "YCLib/NativeAPI.h"
#if YCL_MULTITHREAD == 1
#	include <thread>
#	include <mutex>
#endif

YSL_BEGIN

#if YCL_HOSTED
YSL_BEGIN_NAMESPACE(Host)

//! \since build 381
yconstexpr auto WindowClassName(L"YFramework Window");


/*!
\brief 宿主窗口。
\since build 379
\todo 处理 Windows API 返回值。
*/
class Window
{
public:
	typedef ::HWND NativeHandle;

private:
	//! \since build 380
	std::reference_wrapper<Environment> env;
	NativeHandle h_wnd;

public:
	Window(NativeHandle, Environment& = FetchGlobalInstance().GetHost());
	DefDelCopyCtor(Window)
	DefDelMoveCtor(Window)
	virtual
	~Window();

	DefGetter(const ynothrow, NativeHandle, NativeHandle, h_wnd)
	DefGetter(const ynothrow, Environment&, Host, env)

	void
	Close();

	virtual void
	OnDestroy();

	virtual void
	OnLostFocus();

	virtual void
	OnPaint();

	//! \since build 381
	void
	Show() ynothrow;
};


/*!
\brief 宿主环境。
\since build 378
*/
class Environment
{
private:
	/*!
	\brief 本机窗口对象映射。
	\note 不使用 ::SetWindowLongPtr 等 Windows API 保持跨平台及避免和其它代码冲突。
	\warning 销毁窗口前移除映射，否则可能导致未定义行为。
	\warning 非线程安全，应仅在宿主线程上操作。
	\since build 381
	*/
	map<Window::NativeHandle, Window*> wnd_map;
	/*!
	\brief 窗口对象映射锁。
	\since build 381
	*/
	mutable std::mutex wmap_mtx;
#	if YCL_MULTITHREAD == 1
#		if YCL_MINGW32
	/*!
	\brief 本机主窗口指针。
	\since build 380
	*/
	unique_ptr<Window> p_main_wnd;
#		endif
	//! \brief 宿主背景线程。
	std::thread host_thrd;
#	endif

public:
	Environment();
	~Environment();

	/*!
	\brief 取 GUI 前景窗口。
	\since build 381
	\todo 线程安全。
	*/
	Window*
	GetForegroundWindow() const ynothrow;

	/*!
	\brief 插入窗口映射项。
	\note 线程安全。
	\since build 381
	*/
	void
	AddMappedItem(Window::NativeHandle, Window*);

	/*!
	\brief 调整全局 GUI 坐标到窗口坐标。
	\todo 线程安全。
	\since build 381
	*/
	Drawing::Point
	AdjustCursor(platform::CursorInfo&, const Window&) const ynothrow;

	/*!
	\brief 取本机句柄对应的窗口指针。
	\note 线程安全。
	\since build 381
	*/
	Window*
	FindWindow(Window::NativeHandle) const ynothrow;

	/*!
	\brief 宿主消息循环。
	\since build 379
	*/
	static void
	HostLoop();

#	if YCL_MULTITHREAD == 1
	//! \brief 初始化宿主资源和本机消息循环线程。
	void
	HostTask();
#	endif

	/*!
	\brief 移除窗口映射项。
	\note 线程安全。
	\since build 381
	*/
	void
	RemoveMappedItem(Window::NativeHandle) ynothrow;

	//! \since build 380
	void
	UpdateWindow(Devices::DSScreen&);
};

YSL_END_NAMESPACE(Host)
#endif

YSL_END

#endif
