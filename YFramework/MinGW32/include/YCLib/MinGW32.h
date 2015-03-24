﻿/*
	© 2013-2015 FrankHB.

	This file is part of the YSLib project, and may only be used,
	modified, and distributed under the terms of the YSLib project
	license, LICENSE.TXT.  By continuing to use, modify, or distribute
	this file you indicate that you have read the license and
	understand and accept it fully.
*/

/*!	\file MinGW32.h
\ingroup YCLib
\ingroup MinGW32
\brief YCLib MinGW32 平台公共扩展。
\version r566
\author FrankHB <frankhb1989@gmail.com>
\since build 412
\par 创建时间:
	2012-06-08 17:57:49 +0800
\par 修改时间:
	2015-03-24 11:18 +0800
\par 文本编码:
	UTF-8
\par 模块名称:
	YCLib_(MinGW32)::MinGW32
*/


#ifndef YCL_MinGW32_INC_MinGW32_h_
#define YCL_MinGW32_INC_MinGW32_h_ 1

#include "YCLib/YModules.h"
#include YFM_YCLib_Host
#include YFM_YCLib_NativeAPI
#if !YCL_MinGW
#	error "This file is only for MinGW."
#endif
#include <string> // for std::string, std::wstring;
#include <vector> // for std::vector;
#include <utility> // for std::pair;
#include <chrono> // for std::chrono::nanoseconds;

/*!
\ingroup name_collision_workarounds
\brief 禁止使用 GetObject 宏。
\since build 412
*/
#undef GetObject

namespace platform_ex
{

/*!
\brief Windows 平台扩展接口。
\since build 427
*/
inline namespace Windows
{

/*!
\ingroup exception_types
\brief Win32 错误引起的宿主异常。
\since build 426
*/
class YF_API Win32Exception : public Exception
{
public:
	//! \since build 435
	//@{
	using ErrorCode = unsigned long;

public:
	/*!
	\pre 错误码不等于 0 。
	\warning 初始化参数时可能会改变 ::GetLastError() 的结果。
	\since build 545
	*/
	Win32Exception(ErrorCode, const std::string& = "Win32 exception",
		LevelType = YSLib::Emergent);
	//! \since build 586
	DefDeCopyCtor(Win32Exception)
	/*!
	\brief 虚析构：类定义外默认实现。
	\since build 586
	*/
	~Win32Exception() override;

	DefGetter(const ynothrow, ErrorCode, ErrorCode, ErrorCode(code().value()))
	//! \since build 437
	DefGetter(const ynothrow, std::string, Message,
		FormatMessage(GetErrorCode()))

	explicit DefCvt(const ynothrow, ErrorCode, GetErrorCode())

	/*!
	\brief 取错误类别。
	\return \c std::error_category 派生类的 const 引用。
	\since build 545
	*/
	static const std::error_category&
	GetErrorCategory();

	/*!
	\brief 格式化错误消息字符串。
	\return 若发生异常则结果为空，否则为区域固定为 en-US 的系统消息字符串。
	*/
	static std::string
	FormatMessage(ErrorCode) ynothrow;
	//@}
};

/*!
\brief 按 ::GetLastError 的结果和指定参数抛出 Windows::Win32Exception 对象。
\since build 426
*/
#	define YF_Raise_Win32Exception(...) \
	{ \
		const auto err(::GetLastError()); \
	\
		throw platform_ex::Windows::Win32Exception(err, __VA_ARGS__); \
	}

/*!
\brief 按表达式求值和指定参数抛出 Windows::Win32Exception 对象。
\since build 549
*/
#	define YF_Raise_Win32Exception_On_Failure(_expr, ...) \
	{ \
		const auto err(Win32Exception::ErrorCode(_expr)); \
	\
		if(err != ERROR_SUCCESS) \
			throw platform_ex::Windows::Win32Exception(err, __VA_ARGS__); \
	}


/*!
\brief 安装控制台处理器。
\throw Win32Exception 设置失败。
\note 默认行为使用 <tt>::ExitProcess</tt> ，可能造成 C/C++ 运行时无法正常清理。
\warning 默认不应在 \c std::at_quick_exit 注册依赖静态或线程生存期对象状态的回调。
\see http://msdn.microsoft.com/en-us/library/windows/desktop/ms682658(v=vs.85).aspx
\see http://msdn.microsoft.com/en-us/library/windows/desktop/ms686016(v=vs.85).aspx
\see $2014-10 @ %Documentation::Workflow::Annual2014.
\since build 565

若第一参数为空，则使用具有以下行为的处理器：
对 \c CTRL_C_EVENT \c CTRL_CLOSE_EVENT 、 \c CTRL_BREAK_EVENT 、
\c CTRL_LOGOFF_EVENT 、和 \c CTRL_SHUTDOWN_EVENT ，调用
<tt>std::_Exit(STATUS_CONTROL_C_EXIT)</tt> 。
第二参数指定添加或移除。
*/
YF_API void
FixConsoleHandler(int(WINAPI*)(unsigned long) = {}, bool = true);

/*!
\brief 判断是否在 Wine 环境下运行。
\note 检查 HKEY_CURRENT_USER 和 HKEY_LOCAL_MACHINE 下的 Software\Wine 键实现。
\since build 435
*/
YF_API bool
CheckWine();


// TODO: Add more Windows specific APIs.

/*!	\defgroup native_encoding_conv Native Encoding Conversion
\brief 本机文本编码转换。
\pre 长度参数非零且不上溢 \c int 时断言：字符串指针参数非空。
\exception 长度参数上溢 \int 或转换中溢出。
\note 长度为零时直接返回空字符串，无其它效果。
\since build 587

转换第一个 \c unsigned 参数指定编码的字符串为第二个 \c unsigned 参数指定的编码。
*/
//@{
YF_API std::string
MBCSToMBCS(std::size_t, const char*, unsigned = CP_UTF8, unsigned = CP_ACP);
inline PDefH(std::string, MBCSToMBCS, const char* str,
	unsigned cp_src = CP_UTF8, unsigned cp_dst = CP_ACP)
	ImplRet(Windows::MBCSToMBCS(ystdex::ntctslen(str), str, cp_src, cp_dst))
inline PDefH(std::string, MBCSToMBCS, const std::string& str,
	unsigned cp_src = CP_UTF8, unsigned cp_dst = CP_ACP)
	ImplRet(Windows::MBCSToMBCS(str.length(), str.c_str(), cp_src, cp_dst))

YF_API std::string
WCSToMBCS(std::size_t, const wchar_t*, unsigned = CP_ACP);
inline PDefH(std::string, WCSToMBCS, const wchar_t* str, unsigned cp = CP_ACP)
	ImplRet(Windows::WCSToMBCS(ystdex::ntctslen(str), str, cp))
inline PDefH(std::string, WCSToMBCS, const std::wstring& str,
	unsigned cp = CP_ACP)
	ImplRet(Windows::WCSToMBCS(str.length(), str.c_str(), cp))

YF_API std::wstring
MBCSToWCS(std::size_t, const char*, unsigned = CP_ACP);
inline PDefH(std::wstring, MBCSToWCS, const char* str, unsigned cp = CP_ACP)
	ImplRet(Windows::MBCSToWCS(ystdex::ntctslen(str), str, cp))
inline PDefH(std::wstring, MBCSToWCS, const std::string& str,
	unsigned cp = CP_ACP)
	ImplRet(Windows::MBCSToWCS(str.length(), str.c_str(), cp))

//! \since build 540
//@{
inline PDefH(std::string, WCSToUTF8, const wchar_t* str, std::size_t len)
	ImplRet(WCSToMBCS(len, str, CP_UTF8))
inline PDefH(std::string, WCSToUTF8, const wchar_t* str)
	ImplRet(Windows::WCSToUTF8(str, ystdex::ntctslen(str)))
inline PDefH(std::string, WCSToUTF8, const std::wstring& str)
	ImplRet(Windows::WCSToUTF8(str.c_str(), str.length()))

inline PDefH(std::wstring, UTF8ToWCS, const char* str, std::size_t len)
	ImplRet(MBCSToWCS(len, str, CP_UTF8))
inline PDefH(std::wstring, UTF8ToWCS, const char* str)
	ImplRet(Windows::UTF8ToWCS(str, ystdex::ntctslen(str)))
inline PDefH(std::wstring, UTF8ToWCS, const std::string& str)
	ImplRet(Windows::UTF8ToWCS(str.c_str(), str.length()))
//@}
//@}


/*
\brief 文件系统目录查找状态。
\since build 489
*/
class YF_API DirectoryFindData : private ystdex::noncopyable
{
private:
	//! \brief 查找起始的目录名称。
	std::wstring dir_name;
	//! \brief Win32 查找数据。
	::WIN32_FIND_DATAW find_data;
	//! \brief 查找节点句柄。
	::HANDLE h_node = {};
	//! \brief 当前查找的项目名称。
	std::wstring d_name{};

public:
	/*!
	\brief 构造：使用指定的目录路径。
	\pre 断言：路径非空且不以 '\\' 结尾。
	\throw platform::FileOperationFailure 打开路径失败，或指定的路径不是目录。
	\note 目录路径无视结尾的斜杠和反斜杠。
	*/
	//@{
	//! \note 使用 UTF-8 目录路径。
	DirectoryFindData(std::string);
	//! \note 使用 UTF-16 目录路径。
	DirectoryFindData(std::wstring);
	//@}
	//! \brief 析构：若查找节点句柄非空则关闭查找状态。
	~DirectoryFindData();

	//! \since build 556
	DefBoolNeg(explicit, h_node)

	//! \since build 564
	DefGetter(const ynothrow, unsigned long, Attributes,
		find_data.dwFileAttributes)
	DefGetter(const ynothrow, const ::WIN32_FIND_DATAW&, FindData, find_data)
	DefGetter(const ynothrow, const std::wstring&, DirName, dir_name)

private:
	/*!
	\brief 关闭查找状态。
	\post 断言：关闭成功。
	*/
	void
	Close() ynothrow;

public:
	/*!
	\brief 读取：迭代当前查找状态。
	\return 若迭代结束后节点非空，表示当前查找项目名的指针；否则为空指针。

	若查找节点句柄非空则迭代当前查找状态查找下一个文件系统项。
	否则查找节点句柄为空或迭代失败则关闭查找状态并置查找节点句柄空。
	最终查找节点非空时保存记录当前查找的项目状态。
	*/
	std::wstring*
	Read();

	//! \brief 复位查找状态：若查找节点句柄非空则关闭查找状态并置查找节点句柄空。
	void
	Rewind() ynothrow;
};


/*!
\brief 注册表键。
\warning 不检查外部进程交互：并发访问可能导致错误的结果。
\since build 549
\todo 增加和实现创建和删除值等功能接口。
*/
class YF_API RegistryKey
{
private:
	::HKEY h_key;

public:
	//! \since build 564
	RegistryKey(::HKEY h_parent, const wchar_t* name, unsigned long ul_opt = 0,
		::REGSAM access = KEY_READ)
	{
		YF_Raise_Win32Exception_On_Failure(::RegOpenKeyExW(h_parent,
			name, ul_opt, access, &h_key), "RegOpenKeyEx");
	}
	//! \since build 549
	RegistryKey(RegistryKey&& key)
		: h_key(key.h_key)
	{
		key.h_key = {};
	}
	~RegistryKey()
	{
		::RegCloseKey(h_key);
	}

	DefDeMoveAssignment(RegistryKey)

	DefGetter(const ynothrow, ::HKEY, Key, h_key)
	/*!
	\brief 取指定名称和类型的值的存储表示。
	\return 成功得到的值的类型和内容。
	\note 类型为 \c REG_NONE 时表示允许任意类型的值。
	\since build 564
	*/
	//@{
	//! \brief 间接断言：第一参数非空。
	std::pair<unsigned long, std::vector<ystdex::byte>>
	GetRawValue(const wchar_t*, unsigned long = REG_NONE) const;
	PDefH(std::pair<unsigned long YPP_Comma std::vector<ystdex::byte>>,
		GetRawValue, const std::wstring& name, unsigned long type = REG_NONE)
		const
		ImplRet(GetRawValue(name.c_str(), type))
	//@}
	std::size_t
	GetSubKeyCount() const;
	std::vector<std::wstring>
	GetSubKeyNames() const;
	std::size_t
	GetValueCount() const;
	std::vector<std::wstring>
	GetValueNames() const;

	//! \throw Win32Exception 刷新失败。
	void
	Flush();
};

/*!
\brief 取注册表字符串值。
\pre 间接断言：字符串参数非空。
\return 若成功则为指定的值，否则为空串。
\note 字符串内容保证不以空字符结尾。
\relates RegistryKey
\since build 522
*/
//@{
YF_API std::wstring
FetchRegistryString(const RegistryKey&, const wchar_t*);
inline PDefH(std::wstring, FetchRegistryString, ::HKEY h_parent,
	const wchar_t* key_name, const wchar_t* name)
	ImplRet(FetchRegistryString(RegistryKey(h_parent, key_name), name))
//@}


/*!
\brief 取系统目录路径。
\note 保证以一个分隔符结尾。
\since build 552
*/
YF_API std::wstring
FetchSystemPath(size_t s = MAX_PATH);

/*!
\brief 转换文件时间为以 POSIX 历元起始度量的时间间隔。
\throw std::system_error 输入的时间表示不被实现支持。
\since build 544
*/
YF_API std::chrono::nanoseconds
ConvertTime(::FILETIME&);

} // inline namespace Windows;

} // namespace platform_ex;

#endif

