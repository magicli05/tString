/*++

Copyright (c) 2005

Module Name:

    tstring.h
    tstring.cpp


Abstract:

    提供一个自适应TCHAR的_tstring,与TCHAR配合使用
    自适应Unicode与ANSI C
    支持UTF-8编码转换
    支持Base64编码转换
    注：它是为方便使用而设计的，并非为效率而设计，
    所以在效率要求很高的场合应该寻找更合适的方式

Author:


Revision History:



--*/

#ifndef _H_T_STRING
#define _H_T_STRING

#include <string>
#include <TCHAR.h>

namespace Common
{
#define _tANSICHAR(x)       Common::_tstring(x).toNarrowString().c_str()
#define _tUNICODECHAR(x)    Common::_tstring(x).toWideString().c_str()
#define _tUTF8CHAR(x)       Common::_tstring(x).toUTF8().c_str()

#define _tstringA(x)        Common::_tstring(x).toNarrowString()
#define _tstringW(x)        Common::_tstring(x).toWideString()

class _tstring
#ifdef _UNICODE
    : public std::wstring
#else
    : public std::string
#endif
{
public:
    _tstring(void);
    ~_tstring(void);

public:
    void fromUTF8(const char* pszUTF8);
    std::string toUTF8();

    void fromBASE64(const char* pszBASE64);
    std::string toBASE64();

    std::string toNarrowString();
    std::wstring toWideString();

public:
    _tstring& MakeLower();
    _tstring& MakeUpper();

    int CompareNoCase(_tstring str);

    unsigned int Replace(_tstring strOld, _tstring strNew);
    unsigned int Replace(wchar_t chOld, wchar_t chNew);
    unsigned int Replace(char chOld, char chNew);

    _tstring Right(size_t nCount);
    _tstring Left(size_t nCount);
    _tstring Mid(size_t iFirst, size_t nCount);

    size_t Find(const TCHAR ch, unsigned int nStart = 0);
    size_t Find(const TCHAR* pszch, unsigned int nStart = 0);
    size_t ReverseFind(const TCHAR ch);

public:
    //实现sprintf功能
    void Format(const char* pszFormat, ...);
    void Format(const wchar_t* pwszFormat, ...);


//正则表达式匹配功能:
//public:
//
//    //匹配: strPattern为规则字串, strResult为匹配成功的字串
//    virtual bool match(const _tstring& strPattern, _tstring& strResult);
//
//    //匹配: 不需要返回值只获得是否匹配成功
//    virtual bool match(const _tstring& strPattern);
//
//
//    virtual bool match(const char* pStr, _tstring& strResult);
//    virtual bool match(const wchar_t* pwStr, _tstring& strResult);
//    virtual bool match(const std::string& str, _tstring& strResult);
//    virtual bool match(const std::wstring& str, _tstring& strResult);
//
//    virtual bool match(const char* pStr);
//    virtual bool match(const wchar_t* pwStr);
//    virtual bool match(const std::string& str);
//    virtual bool match(const std::wstring& str);

public:

#ifdef _UNICODE
    using std::wstring::operator =;
#else
    using std::string::operator =;
#endif
    //重载=号函数
    _tstring& operator = (char ch);
    _tstring& operator = (wchar_t ch);
    _tstring& operator = (const char* pStr);
    _tstring& operator = (const wchar_t* pwStr);
    _tstring& operator = (const _tstring& Str);
    _tstring& operator = (const std::string& Str);
    _tstring& operator = (const std::wstring& Str);

public:
    //构造函数
    _tstring(wchar_t ch);
    _tstring(char ch);
    _tstring(const char* pStr);
    _tstring(const wchar_t* pwStr);
    _tstring(const _tstring& str);
    _tstring(const std::string& str);
    _tstring(const std::wstring& str);

public:

    //ANSI C串与UNICODE串转换功能
    static std::wstring toWideString(const char* pStr);
    static std::string toNarrowString(const wchar_t* pwStr);

    //UTF-8与UNICODE串转换功能
    static std::string wcstoUTF8(const wchar_t* pwStr);
    static std::wstring UTF8towcs(const char* pStr);

    static std::string cstobase64(const char* pStr, size_t nLength);
    static std::string base64tocs(const char* pStr);
};

};

#endif