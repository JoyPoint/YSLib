﻿/*
	Copyright (C) by Franksoft 2010 - 2012.

	This file is part of the YSLib project, and may only be used,
	modified, and distributed under the terms of the YSLib project
	license, LICENSE.TXT.  By continuing to use, modify, or distribute
	this file you indicate that you have read the license and
	understand and accept it fully.
*/

/*!	\file ShlReader.h
\ingroup YReader
\brief Shell 阅读器框架。
\version r2149;
\author FrankHB<frankhb1989@gmail.com>
\since build 263 。
\par 创建时间:
	2011-11-24 17:08:33 +0800;
\par 修改时间:
	2012-03-18 13:52 +0800;
\par 文本编码:
	UTF-8;
\par 模块名称:
	YReader::ShlReader;
*/


#ifndef INCLUDED_SHLREADER_H_
#define INCLUDED_SHLREADER_H_

#include "Shells.h"
#include "DSReader.h"
#include "HexBrowser.h"
#include "ColorPicker.h"

YSL_BEGIN_NAMESPACE(YReader)

class ShlReader;


class ReaderBox : public Control
{
public:
	ShlReader& Shell;
	/*!
	\brief 弹出菜单按钮。
	\since build 274 。
	*/
	Button btnMenu;
	/*!
	\brief 显示信息框按钮。
	\since build 274 。
	*/
	Button btnInfo;
	/*!
	\brief 返回按钮。
	\since build 274 。
	*/
	Button btnReturn;
	/*!
	\brief 后退按钮。
	\since build 286 。
	*/
	Button btnPrev;
	/*!
	\brief 前进按钮。
	\since build 286 。
	*/
	Button btnNext;
	ProgressBar pbReader;
	Label lblProgress;

	ReaderBox(const Rect&, ShlReader&);

	virtual IWidget*
	GetTopWidgetPtr(const Point&, bool(&)(const IWidget&));

	/*!
	\brief 刷新：按指定参数绘制界面并更新状态。
	\since build 294 。
	*/
	virtual void
	Refresh(PaintEventArgs&&);

	/*!
	\brief 更新进度数据。
	\since build 271 。
	*/
	void
	UpdateData(DualScreenReader&);
};


class TextInfoBox : public DialogBox
{
public:
	ShlReader& Shell;
	Label lblEncoding;
	Label lblSize;
	/*!
	\brief 指示迭代器位置。
	\since build 273 。
	*/
	//@{
	Label lblTop;
	Label lblBottom;
	//@}

	TextInfoBox(ShlReader&);

	/*!
	\brief 刷新：按指定参数绘制界面并更新状态。
	\since build 294 。
	*/
	virtual void
	Refresh(PaintEventArgs&&);

	void
	UpdateData(DualScreenReader&);
};


/*!
\brief 阅读器设置。
\since build 287 。
*/
struct ReaderSetting
{
	Color UpColor, DownColor, FontColor;
	Drawing::Font Font;
	/*!
	\brief 平滑滚屏。
	\since build 293 。
	*/
	bool SmoothScroll;
	/*!
	\brief 滚屏间隔。
	\since build 293 。
	*/
	std::chrono::milliseconds ScrollDuration;
	/*!
	\brief 平滑滚屏间隔。
	\since build 293 。
	*/
	std::chrono::milliseconds SmoothScrollDuration;
};


/*!
\brief 设置面板。
\since build 275 。
*/
class SettingPanel : public DialogPanel
{
	/*!
	\brief 友元类：共享设置状态。
	\since build 287 。
	*/
	friend class TextReaderSession;

protected:
	/*!
	\brief 标签：文本区域示例。
	\note 不加入 SettingPanel 容器，加入上屏桌面。
	\since build 280 。
	*/
	Label lblAreaUp, lblAreaDown;
	/*!
	\brief 按钮：字体大小递减/递增。
	\since build 280 。
	*/
	Button btnFontSizeDecrease, btnFontSizeIncrease;
	/*!
	\brief 按钮：设置。
	\since build 279 。
	*/
	Button btnSetUpBack, btnSetDownBack, btnTextColor;
	/*!
	\brief 下拉列表：字体设置。
	\since build 282 。
	*/
	DropDownList ddlFont;
	/*!
	\brief 下拉列表：编码设置。
	\since build 290 。
	*/
	DropDownList ddlEncoding;
	/*!
	\brief 复选框：选定自动光滑滚屏。
	\since build 292 。
	*/
	CheckBox chkSmoothScroll;
	/*!
	\brief 标签：选定自动光滑说明。
	\since build 292 。
	*/
	Label lblSmoothScroll;
	/*!
	\brief 下拉列表：自动滚屏时间设置。
	\since build 292 。
	*/
	DropDownList ddlScrollTiming;
	ColorBox boxColor;

private:
	/*!
	\brief 当前设置的颜色指针。
	\none 为空指针时忽略。
	\since build 277 。
	*/
	Color* pColor;
	/*!
	\brief 当前设置的编码。
	\since build 292 。
	*/
	Text::Encoding current_encoding;
	/*!
	\brief 滚屏间隔。
	\since build 292 。
	*/
	std::chrono::milliseconds scroll_duration;
	/*!
	\brief 平滑滚屏间隔。
	\since build 292 。
	*/
	std::chrono::milliseconds smooth_scroll_duration;

public:
	SettingPanel();

	/*!
	\brief 读取设置。
	\since build 293 。
	*/
	SettingPanel&
	operator<<(const ReaderSetting&);

	/*!
	\brief 保存设置。
	\since build 293 。
	*/
	SettingPanel&
	operator>>(ReaderSetting&);

	/*!
	\brief 更新示例文本。
	\since build 280 。
	*/
	void
	UpdateInfo();
};


class FileInfoPanel : public Panel
{
public:
	Label lblPath, lblSize, lblAccessTime, lblModifiedTime, lblOperations;
	
	FileInfoPanel();
};


/*!
\brief 书签。
\warning 非虚析构。
\since build 286 。
*/
class BookMark
{
public:
	IO::Path Path;
	size_t Position;

	BookMark(const IO::Path&, size_t);
	DefDeCopyCtor(BookMark)
	DefDeMoveCtor(BookMark)
};

inline
BookMark::BookMark(const IO::Path& path, size_t pos)
	: Path(path), Position(pos)
{}


/*!
\brief 判断书签是否相等。
\since build 286 。
*/
inline bool
operator==(const BookMark& x, const BookMark& y)
{
	return x.Path == y.Path && x.Position == y.Position;
}


/*!
\brief 阅读记录。
\warning 非虚析构。
\since build 286 。
*/
class ReadingList
{
public:
	/*!
	\brief 阅读记录列表类型。
	*/
	typedef list<BookMark> ListType;

private:
	/*!
	\brief 阅读记录：书签项列表。
	*/
	ListType reading_list;
	/*!
	\brief 阅读迭代器：当前浏览项在阅读记录中的插入位置。
	*/
	ListType::iterator now_reading;

public:
	/*!
	\brief 构造：阅读迭代器指向阅读记录末尾。
	*/
	ReadingList();
	DefDelCopyCtor(ReadingList)
	DefDelMoveCtor(ReadingList)

	/*!
	\brief 检查操作可用性。
	\return 后退和向前操作是否可用。
	*/
	pair<bool, bool>
	CheckBoundary();

	/*!
	\brief 清除所有记录。
	*/
	PDefH(void, Clear)
		ImplExpr(reading_list.clear(), now_reading = reading_list.end())

	/*!
	\brief 清除当前迭代器之后的所有记录并使迭代器指向末尾。
	\since build 288 。
	*/
	void
	DropSubsequent();

	/*!
	\brief 插入阅读记录。
	*/
	void
	Insert(const IO::Path&, size_t);

	/*!
	\brief 切换阅读记录。
	\param 是否后退。
	\return 被移除的记录。
	\warning 不检查是否越界。

	后退或前进，同时移除该记录。
	*/
	BookMark
	Switch(bool);
};


/*!
\brief 阅读器会话。
\since build 287 。
*/
class ReaderSession
{
public:
	ShlReader& Shell;

	ReaderSession(ShlReader&);
	DefDelCopyCtor(ReaderSession)
	DefDelMoveCtor(ReaderSession)
	virtual DefEmptyDtor(ReaderSession)
};


/*!
\brief 文本阅读器会话。
\since build 287 。
*/
class TextReaderSession : public ReaderSession
{
private:
	/*!
	\brief 路径。
	\since build 286 。
	*/
	IO::Path path;

protected:
	/*!
	\brief 滚屏计时器。
	\since build 289 。
	*/
	Timers::Timer tmrScroll;

public:
	DualScreenReader Reader;
	ReaderBox boxReader;
	TextInfoBox boxTextInfo;
	/*!
	\brief 设置面板。
	\since build 275 。
	*/
	SettingPanel pnlSetting;
	unique_ptr<TextFile> pTextFile;
	MenuHost mhMain;

	TextReaderSession(ShlReader&);
	/*!
	\brief 析构：释放资源。
	\since build 286 。
	*/
	virtual
	~TextReaderSession();

private:
	/*!
	\brief 执行阅读器命令。
	\since build 274 。
	*/
	void
	Execute(IndexEventArgs::ValueType);

public:
	/*!
	\brief 读取文件。
	\note 不刷新按钮状态。
	\since build 286 。
	*/
	void
	LoadFile(const IO::Path&);

	/*!
	\brief 当自动滚屏有效状态为 true 时超时自动滚屏。
	\since build 289 。
	*/
	void
	Scroll();

private:
	void
	ShowMenu(Menu::ID, const Point&);

	/*!
	\brief 切换编码。
	\note 若文本文件无效、参数为 Encoding() 或与当前编码相同则忽略。
	\since build 292 。
	*/
	void
	Switch(Text::Encoding);

	/*!
	\brief 更新近期浏览记录并更新按钮状态。
	\param 是否后退。
	\since build 286 。
	*/
	void
	UpdateReadingList(bool);

public:
	/*!
	\brief 更新按钮状态。
	\note 检查近期浏览记录状态确定可用性。
	\since build 286 。
	*/
	void
	UpdateButtons();

private:
	void
	OnClick(TouchEventArgs&&);

	void
	OnKeyDown(KeyEventArgs&&);
};


/*!
\brief 十六进制阅读器会话。
\since build 287 。
*/
class HexReaderSession : public ReaderSession
{
public:
	HexViewArea HexArea;
	FileInfoPanel pnlFileInfo;

	HexReaderSession(ShlReader&);
	/*!
	\brief 析构：释放资源。
	\since build 286 。
	*/
	virtual
	~HexReaderSession();

	void
	UpdateInfo();
};


class ShlReader : public ShlDS
{
public:
	typedef ShlDS ParentType;

	/*!
	\brief 临时参数：路径。
	\since build 286 。
	*/
	static IO::Path CurrentPath;
	/*!
	\brief 临时参数：指定文件类型是否为文本文件。
	\since build 286 。
	*/
	static bool CurrentIsText;

private:
	/*!
	\brief 备用桌面画刷。
	\since build 294 。
	*/
	HBrush bg_up, bg_dn;

protected:
	unique_ptr<ReaderSession> pManager;

private:
	/*!
	\brief 背景任务：用于滚屏。
	\since build 298 。
	*/
	std::function<void()> background_task;

public:
	/*!
	\brief 近期浏览记录。
	\since build 286 。
	*/
	ReadingList LastRead;
	/*!
	\brief 当前阅读器设置。
	\since build 287 。
	*/
	ReaderSetting CurrentSetting;

	ShlReader();

	/*!
	\brief 处理激活消息。
	\since build 289 。
	*/
	virtual void
	OnActivated(const Message&);

	/*!
	\brief 处理停用消息。
	\since build 289 。
	*/
	virtual void
	OnDeactivated();

	/*!
	\brief 处理输入消息：发送绘制消息，当处于滚屏状态时自动执行滚屏。
	\since build 289 。
	*/
	virtual void
	OnInput();
};

YSL_END_NAMESPACE(YReader)

#endif

