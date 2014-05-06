/*++

Copyright (c) 2005

Module Name:

    tstring.h
    tstring.cpp


Abstract:

    �ṩһ������ӦTCHAR��_tstring,��TCHAR���ʹ��
    ����ӦUnicode��ANSI C
    ֧��UTF-8����ת��
    ֧��Base64����ת��
    ע������Ϊ����ʹ�ö���Ƶģ�����ΪЧ�ʶ���ƣ�
    ������Ч��Ҫ��ܸߵĳ���Ӧ��Ѱ�Ҹ����ʵķ�ʽ

Author:



Revision History:


--*/

//#include "StdAfx.h"
#include <windows.h>
#include <cassert>
#include <algorithm>

#include "tstring.h"
//#include ".\\_PRIV\\Regex\\regexpr2.h"

//#ifdef _UNICODE
//
//#ifdef _MT
//#pragma comment (lib, ".\\tstring\\_PRIV\\Regex\\Lib\\RegexLib_Unicode_MT.lib")
//#else
//#pragma comment (lib, ".\\tstring\\_PRIV\\Regex\\Lib\\RegexLib_Unicode_ST.lib")
//#endif
//
//#else
//
//#ifdef _MT
//#pragma comment (lib, ".\\tstring\\_PRIV\\Regex\\Lib\\RegexLib_Ansi_MT.lib")
//#else
//#pragma comment (lib, ".\\tstring\\_PRIV\\Regex\\Lib\\RegexLib_Ansi_ST.lib")
//#endif
//
//#endif

// ����BASE64���롢����ĳ���
char pszBase64TAB[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
unsigned int Base64Mask[] = {0, 1, 3, 7, 15, 31, 63, 127, 255};

using Common::_tstring;
//using regex::match_results;
//using regex::rpattern;

_tstring::_tstring(void)
{
}

_tstring::~_tstring(void)
{
}

_tstring::_tstring(wchar_t ch)
{
    operator = (ch);
}

_tstring::_tstring(char ch)
{
    operator = (ch);
}

_tstring::_tstring(const wchar_t* pwStr)
{
    operator = (pwStr);
}

_tstring::_tstring (const char* pStr)
{
    operator = (pStr);
}

_tstring::_tstring(const _tstring& str)
{
    operator = (str);
}

_tstring::_tstring(const std::string& str)
{
    operator = (str);
}

_tstring::_tstring(const std::wstring& str)
{
    operator = (str);
}

unsigned int _tstring::Replace(_tstring strOld, _tstring strNew)
{
    unsigned int nCount = 0;

    _tstring::iterator it_begin = std::search(begin(), end(), strOld.begin(), strOld.end()); 

    while(it_begin != end()) 
    {
        _tstring::iterator it_end = it_begin + strOld.size(); 
        replace(it_begin, it_end, strNew); 
        it_begin = std::search(begin(), end(), strOld.begin(), strOld.end());
        nCount++;
    }

    return nCount;
}

unsigned int _tstring::Replace(wchar_t chOld, wchar_t chNew)
{
    unsigned int nCount = 0;
    if (chOld == chNew)
    {
        return nCount;
    }

#ifdef _UNICODE
    size_t length = this->length();
    for (size_t i=0; i<length; i++)
    {
        if (this->operator [](i) == chOld)
        {
            this->operator [](i) = chNew;
            nCount++;
        }
    }
    return nCount;
#else
    return Replace(_tstring(chOld), _tstring(chNew));
#endif

}

unsigned int _tstring::Replace(char chOld, char chNew)
{
    unsigned int nCount = 0;
    if (chOld == chNew)
    {
        return nCount;
    }
#ifdef _UNICODE
    return Replace(_tstring(chOld), _tstring(chNew));
#else
    size_t length = this->length();
    for (size_t i=0; i<length; i++)
    {
        if (this->operator [](i) == chOld)
        {
            this->operator [](i) = chNew;
            nCount++;
        }
    }
    return nCount;
#endif
}

//************************************************************
//ʵ��sprintf����

void _tstring::Format(const char* pszFormat, ...)
{
    va_list args;
    va_start(args, pszFormat);
    unsigned int nLength = _vscprintf(pszFormat, args);

#ifdef _UNICODE
    char* pszTmp = new char [nLength+1];
    pszTmp[nLength] = '\0';
    _vsnprintf_s(pszTmp, _TRUNCATE, nLength, pszFormat, args);
    operator = (pszTmp);
    delete [] pszTmp;
#else
    resize(nLength);
    _vsnprintf_s(const_cast<char*>(this->c_str()), _TRUNCATE, nLength, pszFormat, args);
#endif

    va_end(args);
}


void _tstring::Format(const wchar_t* pwszFormat, ...)
{
    va_list args;
    va_start(args, pwszFormat);
    unsigned int nLength = _vscwprintf(pwszFormat, args);

#ifdef _UNICODE
    resize(nLength);
    _vsnwprintf_s(const_cast<wchar_t*>(c_str()), _TRUNCATE, nLength, pwszFormat, args);
#else
    wchar_t* pwszTmp = new wchar_t [nLength+1];
    pwszTmp[nLength] = L'\0';
    _vsnwprintf_s(pwszTmp, nLength, _TRUNCATE, pwszFormat, args);
    operator = (pwszTmp);
    delete [] pwszTmp;
#endif

    va_end(args);
}

_tstring _tstring::Right(size_t nCount)
{
    if (nCount >= length())
    {
        return *this;
    }

    return substr(length() - nCount, nCount);
}

_tstring _tstring::Left(size_t nCount)
{
    if (nCount >= length())
    {
        return *this;
    }
    return substr(0, nCount);
}

_tstring _tstring::Mid(size_t iFirst, size_t nCount)
{
    if (iFirst > length())
    {
        iFirst = length();
    }

    if (nCount + iFirst > length())
    {
        nCount = length()-iFirst;
    }

    return substr(iFirst, nCount);
}

size_t _tstring::Find(const TCHAR ch, unsigned int nStart)
{
    if (nStart > length())
    {
        return static_cast<size_t>(-1);
    }
    return find(ch, nStart);
}

size_t _tstring::Find(const TCHAR* pszch, unsigned int nStart)
{
    if (nStart > length())
    {
        return static_cast<size_t>(-1);
    }
    return find(_tstring(pszch), nStart);
}

size_t _tstring::ReverseFind(const TCHAR ch)
{
    for (size_t nLength = length(); nLength > 0; nLength--)
    {
        if (this->at(nLength-1) == ch)
        {
            return nLength-1;
        }
    }
    return static_cast<size_t>(-1);
}

//************************************************************
//����=�ź���

_tstring& _tstring::operator = (char ch)
{
#ifdef _UNICODE
    std::string str;
    str.resize(1);
    str[0] = ch;
    operator = (str);
#else
    resize(1);
    operator [](0) = ch;
#endif

    return *this;
}

_tstring& _tstring::operator = (wchar_t ch)
{
#ifdef _UNICODE
    resize(1);
    operator [](0) = ch;
#else
    std::wstring str;
    str.resize(1);
    str[0] = ch;
    operator = (str);
#endif

    return *this;
}

_tstring& _tstring::operator = (const char* pStr)
{
    //��鴫������Ƿ�Ϊ��ָ��
    //assert(pStr!=NULL);
    if (pStr==NULL)
    {
        *this = _T("");
        return *this;
    }
#ifdef _UNICODE
    std::wstring::operator = (toWideString(pStr));
#else
    std::string::operator = (pStr);
#endif
    return *this;
}


_tstring& _tstring::operator = (const wchar_t* pwStr)
{
    //��鴫������Ƿ�Ϊ��ָ��
    //assert(pwStr!=NULL);
    if (pwStr==NULL)
    {
        *this = _T("");
        return *this;
    }

#ifdef _UNICODE
    std::wstring::operator = (pwStr);
#else
    std::string::operator = (toNarrowString(pwStr));
#endif
    return *this;
}

_tstring& _tstring::operator = (const _tstring& Str)
{
    if (&Str != this)
    {
        operator = (Str.c_str());
    }
    return *this;
}

_tstring& _tstring::operator = (const std::string& Str)
{
    operator = (Str.c_str());
    return *this;
}

_tstring& _tstring::operator = (const std::wstring& Str)
{
    operator = (Str.c_str());
    return *this;
}

//************************************************************


//************************************************************
//������ʽƥ�书��
//************************************************************
//
////ƥ��: strPatternΪ�����ִ�, strResultΪƥ��ɹ����ִ�
//bool _tstring::match(const _tstring& strPattern, _tstring& strResult)
//{
//    match_results results;
//    rpattern pat(strPattern.c_str());
//
//    match_results::backref_type br = pat.match(this->c_str(), results);
//
//    if (br.matched)
//    {
//        strResult = br.str();
//        return true;
//    }
//    else
//    {
//        return false;
//    }
//}
//
////ƥ��: ����Ҫ����ֵֻ����Ƿ�ƥ��ɹ�
//bool _tstring::match(const _tstring& strPattern)
//{
//
//    match_results results;
//    rpattern pat(strPattern.c_str());
//
//    match_results::backref_type br = pat.match(this->c_str(), results);
//    if (br.matched)
//    {
//        return true;
//    }
//    else
//    {
//        return false;
//    }
//}
//
//bool _tstring::match(const char* pStr, _tstring& strResult)
//{
//    return match(_tstring(pStr), strResult);
//}
//
//bool _tstring::match(const wchar_t* pwStr, _tstring& strResult)
//{
//    return match(_tstring(pwStr), strResult);
//}
//
//bool _tstring::match(const std::string& str, _tstring& strResult)
//{
//    return match(_tstring(str), strResult);
//}
//
//bool _tstring::match(const std::wstring& str, _tstring& strResult)
//{
//    return match(_tstring(str), strResult);
//}
//
//bool _tstring::match(const char* pStr)
//{
//    return match(_tstring(pStr));
//}
//bool _tstring::match(const wchar_t* pwStr)
//{
//    return match(_tstring(pwStr));
//}
//bool _tstring::match(const std::string& str)
//{
//    return match(_tstring(str));
//}
//bool _tstring::match(const std::wstring& str)
//{
//    return match(_tstring(str));
//}

//************************************************************

_tstring& _tstring::MakeLower()
{
    _tstring::iterator itr = this->begin();
    while (itr != this->end())
    {
        if (*itr >= _T('A') && *itr <= _T('Z'))
        {
            *itr += (_T('a') - _T('A'));
        }
        itr++;
    }
    return *this;
}


_tstring& _tstring::MakeUpper()
{
    _tstring::iterator itr = this->begin();
    while (itr != this->end())
    {
        if (*itr >= _T('a') && *itr <= _T('z'))
        {
            *itr -= (_T('a') - _T('A'));
        }
        itr++;
    }
    return *this;
}



int _tstring::CompareNoCase(_tstring str)
{
    _tstring strSrc = *this;
    strSrc.MakeLower();
    str.MakeLower();
    return str.compare(str);
}

//************************************************************
//�ִ�����UNICODE��ANSI C֮���໥ת������
//************************************************************

//��ANSI C�ִ�ת����UNICODE�ִ�
std::wstring _tstring::toWideString(const char* pStr)
{
    //��鴫������Ƿ�Ϊ��ָ��
    //assert(pStr!=NULL);
    if (pStr==NULL)
    {
        return std::wstring();
    }

    size_t nLen = strlen(pStr);
    if (nLen == 0)
    {
        return std::wstring(L"");
    }
    int nChars = MultiByteToWideChar(CP_ACP, 0, pStr, static_cast<int>(nLen), NULL, 0);
    if (nChars == 0)
    {
        return std::wstring(L"");
    }
    std::wstring str;
    str.resize(nChars); 
    MultiByteToWideChar(CP_ACP, 0, pStr, static_cast<int>(nLen), 
        const_cast<wchar_t*>(str.c_str()), nChars);

    return str;
}

//��UNICODE�ִ�ת����ANSI C�ִ�
std::string _tstring::toNarrowString(const wchar_t* pwStr)
{
    //��鴫������Ƿ�Ϊ��ָ��
    //assert(pwStr!=NULL);
    if (pwStr==NULL)
    {
        return std::string();
    }

    size_t nLen = wcslen(pwStr);
    if (nLen == 0)
    {
        return std::string("");
    }
    int nChars = WideCharToMultiByte(CP_ACP, 0, pwStr, static_cast<int>(nLen), NULL, 0, NULL, NULL); 
    if (nChars == 0)
    {
        return std::string("");
    }
    std::string str;
    str.resize(nChars) ;
    WideCharToMultiByte(CP_ACP, 0, pwStr, static_cast<int>(nLen), 
        const_cast<char*>(str.c_str()), nChars, NULL, NULL);

    return str;
}
//************************************************************

std::string _tstring::toNarrowString()
{
#ifndef _UNICODE
    return std::string(this->c_str());
#else
    return toNarrowString(this->c_str());
#endif
}

std::wstring _tstring::toWideString()
{
#ifndef _UNICODE
    return toWideString(this->c_str());
#else
    return std::wstring(this->c_str());
#endif
}

//************************************************************
void _tstring::fromUTF8(const char* pszUTF8)
{
    operator = (UTF8towcs(pszUTF8));
}

std::string _tstring::toUTF8()
{
#ifdef _UNICODE
    return wcstoUTF8(this->c_str());
#else
    return wcstoUTF8(toWideString(this->c_str()).c_str());
#endif
}

//************************************************************

//************************************************************
//UTF-8��UNICODE��ת������

std::string _tstring::wcstoUTF8(const wchar_t* pwStr)
{
    //��鴫������Ƿ�Ϊ��ָ��
    //assert(pwStr!=NULL);
    if (pwStr==NULL)
    {
        return std::string();
    }

    int count_bytes = 0;
    wchar_t    byte_one = 0;
    wchar_t byte_other = 0x3f;    // ����λ����������ȡλֵ

    unsigned char utf_one = 0;
    unsigned char utf_other = 0x80;    // ����λ���ñ�UTF-8����

    size_t test_length = 0;
    size_t test_chars = 0;

    wchar_t    tmp_wchar = L'\0';    // ���ڿ��ַ�λ����ȡ��λ��(����6λ)
    unsigned char tmp_char = '\0';

    const wchar_t* src_wstr = pwStr;

    std::string strUTF8;

    do    // ��ѭ���ɼ�⵽�ַ�����β��L'\0'��ת��֮
    {
        for (;;)    // ����ֽ����г���
        {
            if (src_wstr[test_chars] <= 0x7f){
                count_bytes = 1;    // ASCII�ַ�: 0xxxxxxx( ~ 01111111)
                byte_one = 0x7f;    // ����λ������, ��ȡ��Чλֵ, ��ͬ
                utf_one = 0x0;
                break;
            }
            if ( (src_wstr[test_chars] > 0x7f) && (src_wstr[test_chars] <= 0x7ff) ){
                count_bytes = 2;    // 110xxxxx 10xxxxxx[1](���11��1λ, ��дΪ11*1)
                byte_one = 0x1f;    // 00011111, ������(1λ�������ݼ�)
                utf_one = 0xc0;     // 11000000
                break;
            }
            if ( (src_wstr[test_chars] > 0x7ff) && (src_wstr[test_chars] <= 0xffff) ){
                count_bytes = 3;    // 1110xxxx 10xxxxxx[2](MaxBits: 16*1)
                byte_one = 0xf;     // 00001111
                utf_one = 0xe0;     // 11100000
                break;
            }
            if ( (src_wstr[test_chars] > 0xffff) && (src_wstr[test_chars] <= 0x1fffff) ){
                count_bytes = 4;    // 11110xxx 10xxxxxx[3](MaxBits: 21*1)
                byte_one = 0x7;     // 00000111
                utf_one = 0xf0;     // 11110000
                break;
            }
            if ( (src_wstr[test_chars] > 0x1fffff) && (src_wstr[test_chars] <= 0x3ffffff) ){
                count_bytes = 5;    // 111110xx 10xxxxxx[4](MaxBits: 26*1)
                byte_one = 0x3;     // 00000011
                utf_one = 0xf8;     // 11111000
                break;
            }
            if ( (src_wstr[test_chars] > 0x3ffffff) && (src_wstr[test_chars] <= 0x7fffffff) ){
                count_bytes = 6;    // 1111110x 10xxxxxx[5](MaxBits: 31*1)
                byte_one = 0x1;     // 00000001
                utf_one = 0xfc;     // 11111100
                break;
            }
            return strUTF8;    // ���ϽԲ�������Ϊ�Ƿ�����
        }
        // ���¼�����ȡ���ֽ��е���Ӧλ, ������ΪUTF-8����ĸ����ֽ�
        tmp_wchar = src_wstr[test_chars];
        if (strUTF8.length() < (test_length+count_bytes))
        {
            strUTF8.resize(test_length+count_bytes);
        }

        for (int i = count_bytes; i > 1; i--)
        {    // һ�����ַ��Ķ��ֽڽ���ֵ
            tmp_char = (unsigned char)(tmp_wchar & byte_other);
            strUTF8[test_length + i - 1] = (tmp_char | utf_other);
            tmp_wchar >>= 6;
        }

        tmp_char = (unsigned char)(tmp_wchar & byte_one);
        strUTF8[test_length] = (tmp_char | utf_one);
        // λֵ��ȡ����__End!
        test_length += count_bytes;
        test_chars ++;

    }while (src_wstr[test_chars] != L'\0');

    return strUTF8;
}

std::wstring _tstring::UTF8towcs(const char* pStr)
{
    //��鴫������Ƿ�Ϊ��ָ��
    //assert(pStr!=NULL);
    if (pStr==NULL)
    {
        return std::wstring();
    }

    int count_bytes = 0;
    unsigned char byte_one = 0;
    unsigned char byte_other = 0x3f;    // ����λ����������ȡλֵ
    long test_length = 0;
    size_t test_chars = 0;
    wchar_t tmp_wchar = L'\0';

    const unsigned char* src_str = (const unsigned char*)pStr;//static_cast<const unsigned char*>(pStr);
    std::wstring strUnicode;

    do    // ��ѭ���ɼ�⵽�ַ����Ľ�����'\0'��ת��֮
    {
        for (;;)    // ����ֽ����г���
        {
            if (src_str[test_length] <= 0x7f){
                count_bytes = 1;    // ASCII�ַ�: 0xxxxxxx( ~ 01111111)
                byte_one = 0x7f;    // ����λ������, ��ȡ��Чλֵ, ��ͬ
                break;
            }
            if ( (src_str[test_length] >= 0xc0) && (src_str[test_length] <= 0xdf) ){
                count_bytes = 2;    // 110xxxxx(110 00000 ~ 110 111111) 
                byte_one = 0x1f;
                break;
            }
            if ( (src_str[test_length] >= 0xe0) && (src_str[test_length] <= 0xef) ){
                count_bytes = 3;    // 1110xxxx(1110 0000 ~ 1110 1111)
                byte_one = 0xf;
                break;
            }
            if ( (src_str[test_length] >= 0xf0) && (src_str[test_length] <= 0xf7) ){
                count_bytes = 4;    // 11110xxx(11110 000 ~ 11110 111)
                byte_one = 0x7;
                break;
            }
            if ( (src_str[test_length] >= 0xf8) && (src_str[test_length] <= 0xfb) ){
                count_bytes = 5;    // 111110xx(111110 00 ~ 111110 11)
                byte_one = 0x3;
                break;
            }
            if ( (src_str[test_length] >= 0xfc) && (src_str[test_length] <= 0xfd) ){
                count_bytes = 6;    // 1111110x(1111110 0 ~ 1111110 1)
                byte_one = 0x1;
                break;
            }
            return strUnicode;    // ���ϽԲ�������Ϊ�Ƿ�����
        }
        // ���¼�����ȡUTF-8�����ַ������ֽڵ���Чλֵ
        tmp_wchar = src_str[test_length] & byte_one;
        for (int i=1; i<count_bytes; i++)
        {
            tmp_wchar <<= 6;    // ����6λ��������ֽڵ���Чλֵ"λ��"��ֵ
            tmp_wchar = tmp_wchar | (src_str[test_length + i] & byte_other);
        }
        // λֵ��ȡ__End!
        if (strUnicode.length() < test_chars+1)
        {
            strUnicode.resize(test_chars+1);
        }
        strUnicode[test_chars] = tmp_wchar;

        test_length += count_bytes;
        test_chars ++;

    }while (src_str[test_length] != '\0');

    return strUnicode;
}
//************************************************************

std::string _tstring::cstobase64(const char* pStr, size_t nLength)
{
    std::string strBase64;
    if (pStr == NULL || nLength == 0)
    {
        return strBase64;
    }

    int nDigit;
    int nNumBits = 6;
    size_t nIndex = 0;
    size_t nInputSize = nLength;

    int nBitsRemaining = 0;
    long lBitsStorage =0;
    long lScratch =0;
    int nBits;
    unsigned char c;

    while (nNumBits > 0)
    {
        while ((nBitsRemaining < nNumBits) && (nIndex < nInputSize)) 
        {
            c = pStr[nIndex++];
            lBitsStorage <<= 8;
            lBitsStorage |= (c & 0xff);
            nBitsRemaining += 8;
        }
        if( nBitsRemaining < nNumBits ) 
        {
            lScratch = lBitsStorage << (nNumBits - nBitsRemaining);
            nBits = nBitsRemaining;
            nBitsRemaining = 0;
        }     
        else 
        {
            lScratch = lBitsStorage >> (nBitsRemaining - nNumBits);
            nBits = nNumBits;
            nBitsRemaining -= nNumBits;
        }
        nDigit = (int)(lScratch & Base64Mask[nNumBits]);
        nNumBits = nBits;
        if( nNumBits <=0 )
            break;
        
        strBase64 += pszBase64TAB[nDigit];
    }

    // �ں��油��=��
    while (strBase64.length() % 4 != 0 )
    {
        strBase64 += '=';
    }

    return strBase64;
}

std::string _tstring::base64tocs(const char* pStr)
{
    std::string strDecode;
    size_t nIndex =0;
    int nDigit;
    int nDecode[256];
    size_t nSize;
    int nNumBits = 6;

    if (pStr == NULL || (nSize = strlen(pStr)) == 0)
    {
        return strDecode;
    }
    
    // Build Decode Table
    for (int i = 0; i < 256; i++) 
    {
        nDecode[i] = -2; // Illegal digit
    }

    for (int i=0; i < 64; i++)
    {
        nDecode[pszBase64TAB[i]] = i;
        nDecode['='] = -1;
    }

    long lBitsStorage  =0;
    int nBitsRemaining = 0;
    int nScratch = 0;
    unsigned char c;
    
    int i;
    // Decode the Input
    for (nIndex=0, i=0; nIndex<nSize; nIndex++)
    {
        c = pStr[nIndex];

        // �������в��Ϸ����ַ�
        if (c> 0x7F)
        {
            continue;
        }

        nDigit = nDecode[c];
        if( nDigit >= 0 ) 
        {
            lBitsStorage = (lBitsStorage << nNumBits) | (nDigit & 0x3F);
            nBitsRemaining += nNumBits;
            while (nBitsRemaining > 7) 
            {
                nScratch = lBitsStorage >> (nBitsRemaining - 8);
                strDecode += (char)(nScratch & 0xFF);
                i++;
                nBitsRemaining -= 8;
            }
        }
    }

    return strDecode;
}

void _tstring::fromBASE64(const char* pszBASE64)
{
    *this = base64tocs(pszBASE64);
}

std::string _tstring::toBASE64()
{
#ifdef _UNICODE
    std::string strSource = toNarrowString(this->c_str());
    return cstobase64(strSource.c_str(), strSource.length());
#else
    return cstobase64(this->c_str(), this->length());
#endif
}