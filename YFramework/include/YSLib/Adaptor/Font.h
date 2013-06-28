﻿/*
	Copyright by FrankHB 2009 - 2013.

	This file is part of the YSLib project, and may only be used,
	modified, and distributed under the terms of the YSLib project
	license, LICENSE.TXT.  By continuing to use, modify, or distribute
	this file you indicate that you have read the license and
	understand and accept it fully.
*/

/*!	\file Font.h
\ingroup Adaptor
\brief 平台无关的字体库。
\version r3038
\author FrankHB <frankhb1989@gmail.com>
\since build 296
\par 创建时间:
	2009-11-12 22:02:40 +0800
\par 修改时间:
	2013-06-29 06:06 +0800
\par 文本编码:
	UTF-8
\par 模块名称:
	YSLib::Adaptor::Font
*/


#ifndef YSL_INC_Adaptor_Font_h_
#define YSL_INC_Adaptor_Font_h_ 1

#include "../Core/yfunc.hpp"
#include "../Core/yobject.h"
#include <string>
#include "../Core/yexcept.h"
#include "YTextBase.h"

//包含 FreeType2 。

#include <ft2build.h>

#include FT_FREETYPE_H
#include FT_CACHE_H

YSL_BEGIN

YSL_BEGIN_NAMESPACE(Drawing)

//前向声明。
class Font;
class FontCache;
class FontFamily;
class Typeface;


/*!
\brief 字体大小。
\since build 277
*/
typedef u8 FontSize;
/*!
\brief 字体文件路径。
\since build 277
*/
typedef std::string FontPath;
/*!
\brief 字型家族名称。
\since build 277
*/
typedef std::string FamilyName;
/*!
\brief 字型样式名称。
\since build 277
*/
typedef std::string StyleName;


/*!
\brief 字体样式。
\since build 197
*/
enum class FontStyle : u8
{
	Regular = 0, //!< 常规字体。
	Bold = 1, //!< 粗体。
	Italic = 2, //!< 斜体。
	Underline = 4, //!< 下划线。
	Strikeout = 8 //!< 删除线。
};

DefBitmaskOperations(FontStyle, u8)


/*!
\brief 取样式名称。
\return 非空结果。
\since build 277
*/
yconstfn const char*
FetchName(FontStyle style) ynothrow
{
	return style == FontStyle::Bold ? "Bold" : (style == FontStyle::Italic
		? "Italic" : (style == FontStyle::Underline ? "Underline" : (style
		== FontStyle::Strikeout ? "Strikeout" : "Regular")));
}


/*!
\brief 字体异常。
\since build 241
*/
class YF_API FontException : public LoggedEvent
{
public:
	typedef ::FT_Error FontError;

private:
	FontError err;

public:
	FontException(FontError e, const std::string& msg = "")
		: LoggedEvent(msg),
		err(e)
	{}

	DefGetter(const ynothrow, FontError, ErrorCode, err)
};


/*!
\brief 本机字体大小。
\since build 419
*/
class YF_API NativeFontSize final
{
private:
	::FT_Size size;

public:
	NativeFontSize(FT_Face, FontSize);
	NativeFontSize(NativeFontSize&&) ynothrow;
	~NativeFontSize() ynothrow;

	::FT_SizeRec&
	GetSizeRec() const;
};


/*!
\brief 字型家族 (Typeface Family) 标识。
\since build 145
*/
class YF_API FontFamily final : private noncopyable
{
public:
	typedef map<const StyleName, Typeface*> FaceMap; //!< 字型组索引类型。

	FontCache& Cache;

private:
	FamilyName family_name;

protected:
	FaceMap mFaces; //!< 字型组索引类型。

public:
	/*!
	\brief 使用字体缓存引用和名称构造字型家族。
	*/
	FontFamily(FontCache&, const FamilyName&);

	/*!
	\brief 向字型组和字型组索引添加字型对象。
	\since build 277
	*/
	void
	operator+=(Typeface&);
	/*!
	\brief 从字型组和字型组索引中移除指定字型对象。
	\since build 277
	*/
	bool
	operator-=(Typeface&);

	DefGetter(const ynothrow, const FamilyName&, FamilyName, family_name)
	/*!
	\brief 取指定样式的字型指针。
	\note 若非 Regular 样式失败则尝试取 Regular 样式的字型指针。
	*/
	Typeface*
	GetTypefacePtr(FontStyle) const;
	/*!
	\brief 取指定样式名称的字型指针。
	*/
	Typeface*
	GetTypefacePtr(const StyleName&) const;
	//! \since build 419
	Typeface&
	GetTypefaceRef(FontStyle) const;
	//! \since build 419
	Typeface&
	GetTypefaceRef(const StyleName&) const;
};


/*!
\brief 字型标识。
\since build 145
*/
class YF_API Typeface final : private noncopyable
{
	//! \since build 419
	friend class Font;

public:
	const FontPath Path;

private:
	//! \since build 419
	//@{
	struct BitmapKey
	{
		::FT_UInt Flags;
		::FT_UInt GlyphIndex;
		FontSize Size;

		PDefHOp(bool, ==, const BitmapKey& key) const ynothrow
			ImplRet(Flags == key.Flags && GlyphIndex == key.GlyphIndex
				&& Size == key.Size)
	};

	struct BitmapKeyHash
	{
		PDefHOp(size_t, (), const BitmapKey& key) const ynothrow
			ImplRet(key.Size * 8 + key.Flags * 31 + key.GlyphIndex / 16)
	};

	struct SmallBitmapData
	{
		::FTC_SBitRec_ sbit;

		SmallBitmapData(::FT_GlyphSlot);
		SmallBitmapData(SmallBitmapData&&);
		~SmallBitmapData();
	};
	//@}

	::FT_Long face_index;
	::FT_Int cmap_index;
	StyleName style_name;
	//! \since build 418
	//@{
	::FT_Face face;
	std::reference_wrapper<FontFamily> family;
	//@}
	//! \since build 419
	mutable unordered_map<BitmapKey, SmallBitmapData, BitmapKeyHash>
		bitmap_cache;
	//! \since build 419
	mutable unordered_map<ucs4_t, ::FT_UInt> glyph_index_cache;

public:
	/*!
	\brief 使用字体缓存引用在指定字体文件路径读取指定索引的字型并构造对象。
	\post 断言： <tt>face</tt> 。
	\post 断言： \c cmap_index 在 face 接受的范围内。
	*/
	Typeface(FontCache&, const FontPath&, u32 = 0);

	/*!
	\brief 比较：相等关系。
	*/
	bool
	operator==(const Typeface&) const;
	/*!
	\brief 比较：严格递增偏序关系。
	*/
	bool
	operator<(const Typeface&) const;

	DefGetterMem(const ynothrow, FamilyName, FamilyName, GetFontFamily())
	/*!
	\brief 取字型家族。
	\since build 278
	*/
	DefGetter(const ynothrow, const FontFamily&, FontFamily, family)
	DefGetter(const ynothrow, const StyleName&, StyleName, style_name)
	/*!
	\brief 取字符映射索引号。
	\since build 278
	*/
	DefGetter(const ynothrow, ::FT_Int, CMapIndex, cmap_index)

private:
	//! \since build 419
	//@{
	SmallBitmapData&
	LookupBitmap(const BitmapKey&) const;

	::FT_UInt
	LookupGlyphIndex(ucs4_t) const;

public:
	PDefH(void, ClearBitmapCache, )
		ImplExpr(bitmap_cache.clear())

	PDefH(void, ClearGlyphIndexCache, )
		ImplExpr(glyph_index_cache.clear())
	//@}
};


/*!
\brief 取默认字型引用。
\throw LoggedEvent 记录异常事件。
\note 仅抛出以上异常。
\since build 194
*/
YF_API const Typeface&
FetchDefaultTypeface() ythrow(LoggedEvent);


/*!
\brief 字符位图。
\warning 若为空时调用成员函数时行为未定义。
\since build 147
*/
class YF_API CharBitmap final
{
public:
	typedef ::FT_Byte* BufferType;
	//! \since build 415
	enum FormatType
	{
		None = ::FT_PIXEL_MODE_NONE,
		Mono = ::FT_PIXEL_MODE_MONO,
		Gray = ::FT_PIXEL_MODE_GRAY,
		Gray2 = ::FT_PIXEL_MODE_GRAY2,
		Gray4 = ::FT_PIXEL_MODE_GRAY4,
		LCD = ::FT_PIXEL_MODE_LCD,
		LCD_V = ::FT_PIXEL_MODE_LCD_V
	};
	typedef ::FTC_SBit NativeType;
	//! \since build 415
	typedef ::FT_Short PitchType;
	typedef ::FT_Byte ScaleType;
	typedef ::FT_Char SignedScaleType;

private:
	NativeType bitmap;

public:
	/*!
	\brief 使用本机类型对象构造字符位图对象。
	*/
	yconstfn
	CharBitmap(const NativeType& b)
		: bitmap(b)
	{}

	yconstfn DefCvt(const ynothrow, NativeType, bitmap)

	yconstfn DefGetter(const ynothrow, BufferType, Buffer, bitmap->buffer)
	//! \since build 415
	yconstfn DefGetter(const ynothrow, FormatType, Format,
		FormatType(bitmap->format))
	//! \since build 414
	yconstfn DefGetter(const ynothrow, ScaleType, GrayLevel, bitmap->max_grays)
	yconstfn DefGetter(const ynothrow, ScaleType, Height, bitmap->height)
	yconstfn DefGetter(const ynothrow, SignedScaleType, Left, bitmap->left)
	//! \since build 415
	yconstfn DefGetter(const ynothrow, PitchType, Pitch, bitmap->pitch)
	yconstfn DefGetter(const ynothrow, SignedScaleType, Top, bitmap->top)
	yconstfn DefGetter(const ynothrow, ScaleType, Width, bitmap->width)
	yconstfn DefGetter(const ynothrow, SignedScaleType, XAdvance,
		bitmap->xadvance)
	yconstfn DefGetter(const ynothrow, SignedScaleType, YAdvance,
		bitmap->yadvance)
};


/*!
\brief 字体缓存。
\since build 209
*/
class YF_API FontCache final : private noncopyable,
	private OwnershipTag<Typeface>, private OwnershipTag<FontFamily>
{
	friend class Typeface;
	/*!
	\brief 友元类：访问 scaler 等对象。
	\since build 280
	*/
	friend class Font;

public:
	typedef set<Typeface*, ystdex::deref_comp<const Typeface>>
		FaceSet; //!< 字型组类型。
	//! \brief 字型家族组索引类型。
	typedef unordered_map<FamilyName, unique_ptr<FontFamily>> FamilyMap;

	/*!
	\brief 字形缓冲区大小。
	\note 单位为字节。
	\since build 277
	*/
	static yconstexpr size_t DefaultGlyphCacheSize = 128U << 10;

private:
	::FT_Library library; //!< 库实例。

protected:
	FaceSet sFaces; //!< 字型组。
	FamilyMap mFamilies; //!< 字型家族组索引。

	Typeface* pDefaultFace; //!< 默认字型指针。

public:
	/*!
	\brief 构造：分配指定大小的字形缓存空间。
	\note 当前暂时忽略参数。
	\since build 316
	*/
	explicit
	FontCache(size_t = DefaultGlyphCacheSize);
	/*!
	\brief 析构：释放空间。
	*/
	~FontCache();

public:
	/*!
	\brief 取字型组。
	\since build 281
	*/
	DefGetter(const ynothrow, const FaceSet&, Faces, sFaces)
	DefGetter(const ynothrow, const FamilyMap&, FamilyIndices, mFamilies) \
		//!< 取字型家族组索引。
//	Font*
//	GetFontPtr() const;
	/*!
	\brief 取指定名称的字型家族指针。
	*/
	const FontFamily*
	GetFontFamilyPtr(const FamilyName&) const;
	/*!
	\brief 取默认字型指针。
	\throw LoggedEvent 记录异常事件。
	\note 仅抛出以上异常。
	*/
	const Typeface*
	GetDefaultTypefacePtr() const ythrow(LoggedEvent);
//	Typeface*
//	GetTypefacePtr(u16) const; //!< 取字型组储存的指定索引的字型指针。
	/*!
	\brief 取指定名称的字型指针。
	*/
	const Typeface*
	GetTypefacePtr(const FamilyName&, const StyleName&) const;

private:
	/*!
	\brief 向字型家族组添加字型家族。
	\since build 418
	*/
	void
	operator+=(unique_ptr<FontFamily>);
	/*!
	\brief 向字型组添加字型对象。
	\since build 277
	*/
	void
	operator+=(Typeface&);

	/*!
	\brief 从字型家族组中移除指定字型对象。
	\since build 277
	*/
	bool
	operator-=(FontFamily&);
	/*!
	\brief 从字型组中移除指定字型对象。
	\since build 277
	*/
	bool
	operator-=(Typeface&);

	/*!
	\brief 清除容器。
	*/
	void
	ClearContainers();

public:
	/*!
	\brief 从字体文件组中载入字型信息。
	\return 成功载入的字型数。
	\since build 316
	*/
	size_t
	LoadTypefaces(const FontPath&);

public:
	/*!
	\brief 初始化默认字型。
	*/
	void
	InitializeDefaultTypeface();
};


/*!
\brief 字体：字模，包含字型、样式和大小。
\since build 145
*/
class YF_API Font final
{
public:
	static yconstexpr FontSize DefaultSize = 12,
		MinimalSize = 4, MaximalSize = 96;

private:
	//! \since build 419
	std::reference_wrapper<Typeface> typeface;
	//! \since build 419
	FontSize font_size;
	/*!
	\brief 字体样式。
	\since build 297
	*/
	FontStyle style;

public:
	/*!
	\brief 默认构造：使用默认字型家族、大小和样式的字体对象。
	\since build 351
	*/
	Font()
		: Font(FetchDefaultTypeface().GetFontFamily())
	{}
	/*!
	\brief 构造：使用指定字型家族、大小和样式的字体对象。
	\since build 351
	*/
	explicit
	Font(const FontFamily&, FontSize = DefaultSize,
		FontStyle = FontStyle::Regular);
	//! \since build 419 as workaround for G++ 4.7.1
	//@{
	DefDeCopyCtor(Font)
	Font(Font&& fnt)
		: Font(fnt)
	{}
	DefDeCopyAssignment(Font)
	//@}

	DefPred(const ynothrow, Bold, bool(style & FontStyle::Bold))
	DefPred(const ynothrow, Italic, bool(style & FontStyle::Italic))
	DefPred(const ynothrow, Underline, bool(style & FontStyle::Underline))
	DefPred(const ynothrow, Strikeout, bool(style & FontStyle::Strikeout))

	/*!
	\brief 取跨距。
	\since build 280
	*/
	s8
	GetAdvance(ucs4_t, FTC_SBit = {}) const;
	/*!
	\brief 取升部。
	\since build 280
	*/
	s8
	GetAscender() const;
	DefGetter(const ynothrow, FontCache&, Cache, GetFontFamily().Cache)
	/*!
	\brief 取降部。
	\since build 280
	*/
	s8
	GetDescender() const;
	DefGetterMem(const ynothrow, const FamilyName&, FamilyName,
		GetFontFamily())
	DefGetterMem(const ynothrow, const FontFamily&, FontFamily, GetTypeface())
	DefGetter(const ynothrow, FontSize, Size, font_size)
	//! \since build 414
	DefGetter(const ynothrow, FontStyle, Style, style)
	/*!
	\brief 取当前字型和大小渲染的指定字符的字形。
	\param c 指定需要被渲染的字符。
	\param flags FreeType 渲染标识。
	\warning 返回的位图在下一次调用 FontCache 方法或底层 FreeType 缓存时不保证有效。
	\warning flags 可能被移除，应仅用于内部实现。
	\since build 280
	*/
	CharBitmap
	GetGlyph(ucs4_t c, ::FT_UInt flags = FT_LOAD_RENDER | FT_LOAD_TARGET_NORMAL)
		const;
	/*!
	\brief 取字体对应的字符高度。
	\since build 280
	*/
	FontSize
	GetHeight() const ynothrow;
	DefGetter(const ynothrow, StyleName, StyleName, FetchName(style))

private:
	/*!
	\brief 取内部信息。
	\since build 419
	*/
	::FT_Size_Metrics
	GetInternalInfo() const;

public:
	/*!
	\brief 取字型引用。
	\since build 280
	*/
	DefGetter(const ynothrow, Typeface&, Typeface, typeface)

	/*!
	\brief 设置字体大小。
	*/
	void
	SetSize(FontSize = DefaultSize);
	/*!
	\brief 设置样式。
	\since build 280
	*/
	bool
	SetStyle(FontStyle);
};

YSL_END_NAMESPACE(Drawing)

YSL_END

#endif

