﻿/*
	Copyright (C) by Franksoft 2009 - 2012.

	This file is part of the YSLib project, and may only be used,
	modified, and distributed under the terms of the YSLib project
	license, LICENSE.TXT.  By continuing to use, modify, or distribute
	this file you indicate that you have read the license and
	understand and accept it fully.
*/

/*!	\defgroup YBase YBaseLib
\brief YFramework 基础库。
*/
/*!	\defgroup YStandardEx YStandardExtend
\ingroup YBase
\brief YFramework 标准扩展库。
*/

/*!	\file ydef.h
\ingroup YBase
\brief 系统环境和公用类型和宏的基础定义。
\version r3007;
\author FrankHB<frankhb1989@gmail.com>
\since 早于 build 132 。
\par 创建时间:
	2009-12-02 21:42:44 +0800;
\par 修改时间:
	2012-07-16 00:41 +0800;
\par 文本编码:
	UTF-8;
\par 模块名称:
	YDefinition;
*/


#ifndef YB_INC_YDEF_H_
#define YB_INC_YDEF_H_

#ifndef NDEBUG
#	define YB_USE_YASSERT
#endif

#ifdef __cplusplus
#	define YB_IMPL_CPP __cplusplus
#	ifdef _MSC_VER
#		undef YB_IMPL_MSCPP
#		define YB_IMPL_MSCPP _MSC_VER
#	elif defined(__GNUC__)
#		undef YB_IMPL_GNUCPP
#		define YB_IMPL_GNUCPP (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 \
			+ __GNUC_PATCHLEVEL__)
#	else
// TODO: complete version checking for compiler and library implementation;
//#ifdef __GNUC__
//#	include <tr1/type_traits>
#		error This language implementation is not supported!
#	endif
#else
#	error This header is only for C++!
#endif

#include <cstddef> // for std::nullptr_t, std::size_t, std::ptrdiff_t, offsetof;
#include <climits>
#include <cassert> // for assert;
#include <cstdint>
#include <cwchar> // for std::wint_t;
#include <utility> // for std::forward;
#include <type_traits> // for std::is_class, std::is_standard_layout;


/*!	\defgroup lang_impl_features Langrage Implementation Features
\brief 语言实现的特性。
\since build 294 。
*/

/*!
\ingroup lang_impl_features
\def YB_HAS_BUILTIN_ALIGNOF
\brief 内建 alignof 支持。
\since build 315 。
*/
#undef YB_HAS_ALIGNOF
#define YB_HAS_ALIGNOF (YB_IMPL_CPP >= 201103L || YB_IMPL_GNUCPP >= 40500)

/*!
\ingroup lang_impl_features
\def YB_HAS_BUILTIN_NULLPTR
\brief 内建 nullptr 支持。
\since build 313 。
*/
#undef YB_HAS_BUILTIN_NULLPTR
#define YB_HAS_BUILTIN_NULLPTR (YB_IMPL_CPP >= 201103L \
	|| YB_IMPL_GNUCPP >= 40600 || YB_IMPL_MSCPP >= 1600)

/*!
\ingroup lang_impl_features
\def YB_HAS_CONSTEXPR
\brief constexpr 支持。
\since build 313 。
*/
#undef YB_HAS_CONSTEXPR
#define YB_HAS_CONSTEXPR (YB_IMPL_CPP >= 201103L || YB_IMPL_GNUCPP >= 40600)

/*!
\ingroup lang_impl_features
\def YB_HAS_NOEXCPT
\brief noexcept 支持。
\since build 319 。
*/
#undef YB_HAS_NOEXCEPT
#define YB_HAS_NOEXCEPT (YB_IMPL_CPP >= 201103L || YB_IMPL_GNUCPP >= 40600)


/*!	\defgroup lang_impl_hints Langrage Implementation Hints
\brief 语言实现的提供的附加提示。
\note 应保证忽略时不导致运行时语义差异。
\since build 294 。
*/

/*!
\ingroup lang_impl_hints
\def YB_ATTRIBUTE
\brief 属性。
\since build 313 。
*/
#if YB_IMPL_GNUCPP >= 20500
#	define YB_ATTRIBUTE(attrs) __attribute__ (attrs)
#else
#	define YB_ATTRIBUTE(attrs)
#endif

/*!
\ingroup lang_impl_hints
\def YB_EXPECT(expr, constant)
\def YB_LIKELY(expr)
\def YB_UNLIKELY(expr)
\brief 分支预测提示。
\since build 313 。
*/
#if YB_IMPL_GNUCPP >= 29600
#	define YB_EXPECT(expr, constant) (__builtin_expect(expr, constant))
#	define YB_LIKELY(expr) (__builtin_expect(bool(expr), 1))
#	define YB_UNLIKELY(expr) (__builtin_expect(bool(expr), 0))
#else
#	define YB_EXPECT(expr, constant) (expr)
#	define YB_LIKELY (expr) (expr)
#	define YB_UNLIKELY (expr) (expr)
#endif


/*!
\def yalignof
\brief 指定特定类型的对齐。
\note 同 C++11 alignof 作用于类型时的语义。
\since build 315 。
\todo 判断是否可使用 TR1 的情形。
*/
#if YB_HAS_ALIGNOF
#	define yalignof alignof
#else
#	define yalignof(_type) std::alignment_of<_type>::value
#endif


/*!
\def yconstexpr
\brief 指定编译时常量表达式。
\note 同 C++11 constepxr 作用于编译时常量的语义。
*/
/*!
\def yconstfn
\brief 指定编译时常量函数。
\note 同 C++11 constepxr 作用于编译时常量函数的语义。
*/
#if YB_HAS_CONSTEXPR
#	define yconstexpr constexpr
#	define yconstfn constexpr
#else
#	define yconstexpr const
#	define yconstfn inline
#endif


/*!
\def YCL_USE_EXCEPTION_SPECIFICATION
\brief 使用 YSLib 动态异常规范。
*/
#ifndef NDEBUG
#	define YCL_USE_EXCEPTION_SPECIFICATION 1
#endif

/*!
\def ythrow
\brief YSLib 动态异常规范：根据是否使用异常规范宏指定或忽略动态异常规范。
\note ythrow = "yielded throwing" 。
*/
#if YCL_USE_EXCEPTION_SPECIFICATION
#	define ythrow throw
#else
#	define ythrow(...)
#endif

/*!
\def ynothrow
\brief YSLib 无异常抛出保证：若支持 noexcept 关键字，指定特定的 noexcept 异常规范。
*/
#ifdef YB_HAS_NOEXCEPT
#	define ynothrow ynoexcept
#else
#	define ynothrow(...)
#endif

/*!
\def ynothrow
\brief YSLib 无异常抛出保证：指定特定的异常规范。
\since build 319 。
*/
#if YB_HAS_NOEXCEPT
#	define ynoexcept noexcept
#else
#	define ynoexcept
#endif


/*!
\def yconstraint
\brief 约束：接口语义。
\note 和普通断言相比强调接口契约。对于移植特定的平台实现时应予以特别注意。
\since build 298 。

运行时检查的接口语义约束断言。不满足此断言的行为是接口明确地未定义的，行为不可预测。
*/
#define yconstraint assert

/*!
\def yassume
\brief 假定：环境语义。
\note 和普通断言相比强调非接口契约。对于移植特定的平台实现时应予以特别注意。
\since build 298 。

运行时检查的环境条件约束断言。用于明确地非 yconstraint 适用的情形。
*/
#define yassume assert


namespace ystdex
{

/*!
\brief 字节类型。
\note ISO C++ 对访问存储的 glvalue 的类型有严格限制，当没有对象生存期保证时，
	仅允许（可能 cv 修饰的） char 和 unsigned char 及其指针/引用或 void* ，
	而不引起未定义行为(undefined behavior) 。
*/
typedef unsigned char byte;

typedef int errno_t;
using std::ptrdiff_t;
using std::size_t;
using std::wint_t;

#if YB_HAS_BUILTIN_NULLPTR

using std::nullptr_t;

#else

/*!
\brief 空指针类。
\see 代码参考：http://topic.csdn.net/u/20100924/17/\
BE0C26F8-5127-46CD-9136-C9A96AAFDA76.html 。
*/
const class nullptr_t
{
public:
	/*
	\brief 转换任意类型至空非成员或静态成员指针。
	*/
	template<typename T>
	inline operator T*() const
	{
		return 0;
	}

	/*
	\brief 转换任意类型至空非静态成员指针。
	*/
	template<typename C, typename T>
	inline operator T C::*() const
	{
		return 0;
	}
	/*
	\brief 支持关系运算符重载。
	*/
	template<typename T> bool
	equals(const T& rhs) const
	{
		return rhs == 0;
	}

	/*
	\brief 禁止取 nullptr 的指针。
	*/
	void operator&() const = delete;
} nullptr = {};

//! \since build 316 。
//@{
template<typename T>
inline bool
operator==(nullptr_t lhs, const T& rhs)
{
	return lhs.equals(rhs);
}
template<typename T>
inline bool
operator==(const T& lhs, nullptr_t rhs)
{
	return rhs.equals(lhs);
}

template<typename T>
inline bool
operator!=(nullptr_t lhs, const T& rhs)
{
	return !lhs.equals(rhs);
}
template<typename T>
inline bool
operator!=(const T& lhs, nullptr_t rhs)
{
	return !rhs.equals(lhs);
}
//@}

#endif


/*!
\brief 空基类模板。
\since build 260 。
*/
template<typename...>
struct empty_base
{};


/*!	\defgroup HelperFunctions Helper Functions
\brief 助手功能/函数。

仅帮助简化编码形式或确定接口，并不包含编译期之后逻辑功能实现的代码设施。
\since build 252 。
*/

/*!
\brief 成员偏移计算静态类型检查。
\see ISO C++ 18.2/4 。
\since build 325 。
*/
template<bool _bMemObjPtr, bool _bNoExcept, class _type>
class offsetof_check
{
	static_assert(std::is_class<_type>::value, "Non class type found.");
	static_assert(std::is_standard_layout<_type>::value,
		"Non standard layout type found.");
	static_assert(_bMemObjPtr, "Non-static member object violation found.");
	static_assert(_bNoExcept, "Exception guarantee violation found.");
};

/*!
\def yoffsetof
\brief 带有静态类型检查的成员偏移计算。
\see ISO C++ 18.2/4 。
\note 某些 G++ 和 Clang++ 版本可使用 __builtin_offsetof 及 -Winvalid-offsetof ，
	但可移植性较差。
\since build 325 。
*/
#define yoffsetof(_type, _member) \
	(decltype(sizeof(ystdex::offsetof_check<std::is_member_object_pointer< \
	decltype(&_type::_member)>::value, ynoexcept(offsetof(_type, _member)), \
	_type>))(offsetof(_type, _member)))


/*!
\brief 根据参数类型使用 std::forward 传递对应参数。
\since build 245 。

传递参数：按类型保持值类别(value catory) 和常量性。
当表达式类型为函数或函数引用类型时，结果为左值(lvalue) ，否则：
当且仅当左值引用类型时结果为左值（此时类型不变）；
否则结果为 xvalue （对应的右值引用类型）。
*/
#define yforward(_expr) std::forward<decltype(_expr)>(_expr)

/*!
\brief 无序列依赖表达式组求值实现。
\return 第一个参数的引用。
\since build 296 。
*/
template<typename _type, typename... _tParams>
yconstfn auto
unsequenced(_type&& arg, _tParams&&...) -> decltype(yforward(arg))
{
	return yforward(arg);
}

/*!
\brief 无序列依赖表达式组求值。
\note 由于实现限制，无法用于 void 类型表达式组。
\note 使用一元形式 %yunsequenced((_expr)) 的形式标记表达式组但不取消序列关系。
\note 支持嵌套使用。
\warning 非一元形式禁止用于产生对于同一对象的未序列化的(unsequenced) 副作用
	的表达式，否则存在未定义行为。
\warning 非一元形式不适用于对顺序有依赖的表达式，包括所有可能抛出异常且对抛出顺序
	敏感（例如 std::bad_cast 处理顺序不同可能造成内存泄露）的表达式。
\since build 266 。
*/
#define yunseq ystdex::unsequenced

} // namespace ystdex;

#endif

