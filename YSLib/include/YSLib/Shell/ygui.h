﻿/*
	Copyright (C) by Franksoft 2009 - 2011.

	This file is part of the YSLib project, and may only be used,
	modified, and distributed under the terms of the YSLib project
	license, LICENSE.TXT.  By continuing to use, modify, or distribute
	this file you indicate that you have read the license and
	understand and accept it fully.
*/

/*!	\file ygui.h
\ingroup Shell
\brief 平台无关的图形用户界面实现。
\version 0.2489;
\author FrankHB<frankhb1989@gmail.com>
\par 创建时间:
	2009-11-16 20:06:58 +0800;
\par 修改时间:
	2011-03-28 10:17 +0800;
\par 字符集:
	UTF-8;
\par 模块名称:
	YSLib::Shell::YGUI;
*/


#ifndef INCLUDED_YGUI_H_
#define INCLUDED_YGUI_H_

#include "ycontrol.h"
#include "../Core/yshell.h"
#include "../Core/yfunc.hpp"
#include "../Service/ytimer.h"

YSL_BEGIN

YSL_BEGIN_NAMESPACE(Shells)

//! \brief 默认图形用户界面 Shell 。
class YGUIShell : public YShell
{
public:
	typedef YShell ParentType;
	typedef list<HWND> WNDs;
	//! \brief 输入保持状态。
	typedef enum
	{
		Free = 0,
		Pressed = 1,
		Held = 2
	} HeldStateType;

private:
	WNDs sWnds; //!< 窗口组。

public:
	HeldStateType KeyHeldState, TouchHeldState; //!< 输入接触状态。
	Drawing::Vec DraggingOffset; //!< 拖放偏移量。
	Timers::YTimer HeldTimer; //!< 输入接触保持计时器。
	Point ControlLocation, LastControlLocation; \
		//最近两次的指针设备操作时的控件全局位置（屏幕坐标）。

private:
	//独立焦点指针：自由状态时即时输入（按下）状态捕获的控件指针。
	IControl* p_KeyDown;
	IControl* p_TouchDown;
	bool control_entered; //!< 记录指针是否在控件内部。
	
public:
	/*!
	\brief 无参数构造。
	\note 向应用程序对象添加自身。
	*/
	YGUIShell();
	virtual DefEmptyDtor(YGUIShell)

	/*!
	\brief 向窗口组添加指定窗口句柄。
	\note 仅当句柄非空时添加。
	*/
	virtual void
	operator+=(HWND);

	/*!
	\brief 从窗口组中移除指定窗口句柄。
	*/
	virtual bool
	operator-=(HWND);

	DefPredicate(ControlEntered, control_entered)

	DefGetter(IControl*, KeyDownPtr, p_KeyDown) //独立键焦点指针。
	DefGetter(IControl*, TouchDownPtr, p_TouchDown) //独立屏幕焦点指针。

	/*!
	\brief 取得窗口组中首个窗口对象的句柄。
	*/
	HWND
	GetFirstWindowHandle() const;
	/*!
	\brief 取得窗口组中顶端窗口对象的句柄。
	*/
	HWND
	GetTopWindowHandle() const;
	/*!
	\brief 取得窗口组中指定屏幕的指定的点所处的最顶层窗口对象的句柄。
	*/
	HWND
	GetTopWindowHandle(YDesktop&, const Point&) const;
	/*!
	\brief 取传递指定屏幕的指针设备光标至 GUI 界面返回的部件指针。
	*/
	IWidget*
	GetCursorWidgetPtr(YDesktop&, const Point&) const;

	/*!
	\brief 清除指定屏幕中属于窗口组的窗口对象。
	*/
	void
	ClearScreenWindows(YDesktop&);

	/*!
	\brief 向屏幕分发窗口对象。
	*/
	void
	DispatchWindows();

	/*!
	\brief 从窗口组中移除所有指定窗口句柄，返回移除的对象数。
	\note 使用 set 实现，因此返回值应为 0 或 1 。
	*/
	WNDs::size_type
	RemoveAll(HWND);

	/*!
	\brief 移除窗口队列中首个窗口对象。
	*/
	void
	RemoveWindow();

	/*!
	\brief 重复检测输入接触保持事件。
	*/
	bool
	RepeatHeld(HeldStateType&, Timers::TimeSpan = 240, Timers::TimeSpan = 120);

	/*!
	\brief 复位接触保持状态。
	*/
	void
	ResetHeldState(HeldStateType&);

	/*!
	\brief 复位 GUI 状态。
	\note 需要在没有销毁时自动释放焦点的相关控件对象被销毁后立即调用，
		以避免处理无效指针。
	*/
	void
	ResetGUIStates();

	/*!
	\brief 向屏幕发送指定窗口对象。
	*/
	bool
	SendWindow(IWindow&);

private:
	void
	TryEntering(IControl&, Components::Controls::TouchEventArgs&);

	void
	TryLeaving(IControl&, Components::Controls::TouchEventArgs&);

	void
	ResetTouchHeldState();

	IControl*
	GetFocusedEnabledVisualControlPtr(IControl*);

	bool
	ResponseKeyUpBase(IControl&, Components::Controls::KeyEventArgs&);

	bool
	ResponseKeyDownBase(IControl&, Components::Controls::KeyEventArgs&);

	bool
	ResponseKeyHeldBase(IControl&, Components::Controls::KeyEventArgs&);

public:
	/*!
	\brief 响应标准按键状态。
	*/
	bool
	ResponseKey(YDesktop&, Components::Controls::KeyEventArgs&,
		Components::Controls::VisualEvent);

	/*!
	\brief 响应屏幕接触状态。
	*/
	bool
	ResponseTouch(IControl&, Components::Controls::TouchEventArgs&,
		Components::Controls::VisualEvent);

	/*!
	\brief Shell 处理函数。
	*/
	virtual int
	ShlProc(const Message&);
};

YSL_END_NAMESPACE(Shells)

using Shells::YGUIShell;

/*!
\brief 取当前线程空间中运行的 GUI Shell 句柄。
\deprecated 不能简单兼容多线程模型。
*/
GHHandle<YGUIShell>
FetchGUIShellHandle();

YSL_BEGIN_NAMESPACE(Components)

YSL_BEGIN_NAMESPACE(Controls)

/*!
\brief 取指定屏幕中的当前焦点对象指针。
*/
IControl*
GetFocusedObjectPtr(YDesktop&);

/*!
\brief 级联请求控件及上层容器焦点。
*/
void
RequestFocusCascade(IControl&);

/*!
\brief 级联释放控件及上层容器焦点。
*/
void
ReleaseFocusCascade(IControl&);


/*
\brief 判断指定控件是否被句柄指定的图形用户界面 Shell 锁定为独立焦点。
\throw GeneralEvent 图形用户界面 Shell 句柄为空。
*/
bool
IsFocusedByShell(const IControl&, GHHandle<YGUIShell> = FetchGUIShellHandle());

YSL_END_NAMESPACE(Controls)

YSL_END_NAMESPACE(Components)

YSL_BEGIN_NAMESPACE(Drawing)

/*!
\brief 绘制窗口边框。
*/
void
DrawWindowBounds(IWindow*, Color);

/*!
\brief 绘制部件边框。
\note 限无缓冲区的部件。
*/
void
DrawWidgetBounds(IWidget&, Color);

/*!
\brief 绘制部件边框。
\note 和 DrawWidgetBounds 类似，但对于有缓冲区的部件，
	在此部件所在窗口的图形上下文绘制。
*/
void
DrawWidgetOutline(IWidget&, Color);

YSL_END_NAMESPACE(Drawing)

YSL_END

#endif
