/* Copyright 2013 the SumatraPDF project authors (see AUTHORS file).
   License: Simplified BSD (see COPYING.BSD) */

/* The most basic things, including string handling functions */
#include "BaseUtil.h"

namespace str {

#define EntryCheck(arg1, arg2) \
    if (arg1 == arg2) \
        return true; \
    if (!arg1 || !arg2) \
        return false

// return true if s1 == s2, case sensitive
bool Eq(const char *s1, const char *s2)
{
    EntryCheck(s1, s2);
    return 0 == strcmp(s1, s2);
}

// return true if s1 == s2, case sensitive
bool Eq(const WCHAR *s1, const WCHAR *s2)
{
    EntryCheck(s1, s2);
    return 0 == wcscmp(s1, s2);
}

// return true if s1 == s2, case insensitive
bool EqI(const char *s1, const char *s2)
{
    EntryCheck(s1, s2);
    return 0 == _stricmp(s1, s2);
}

// return true if s1 == s2, case insensitive
bool EqI(const WCHAR *s1, const WCHAR *s2)
{
    EntryCheck(s1, s2);
    return 0 == _wcsicmp(s1, s2);
}

// compares two strings ignoring case and whitespace
bool EqIS(const WCHAR *s1, const WCHAR *s2)
{
    while (*s1 && *s2) {
        // skip whitespace
        for (; IsWs(*s1); s1++);
        for (; IsWs(*s2); s2++);

        if (towlower(*s1) != towlower(*s2))
            return false;
        if (*s1) { s1++; s2++; }
    }

    return !*s1 && !*s2;
}

bool EqN(const char *s1, const char *s2, size_t len)
{
    EntryCheck(s1, s2);
    return 0 == strncmp(s1, s2, len);
}

bool EqN(const WCHAR *s1, const WCHAR *s2, size_t len)
{
    EntryCheck(s1, s2);
    return 0 == wcsncmp(s1, s2, len);
}

bool EqNI(const char *s1, const char *s2, size_t len)
{
    EntryCheck(s1, s2);
    return 0 == _strnicmp(s1, s2, len);
}

bool EqNI(const WCHAR *s1, const WCHAR *s2, size_t len)
{
    EntryCheck(s1, s2);
    return 0 == _wcsnicmp(s1, s2, len);
}

/* return true if 'str' starts with 'txt', NOT case-sensitive */
bool StartsWithI(const char *str, const char *txt)
{
    EntryCheck(str, txt);
    return 0 == _strnicmp(str, txt, str::Len(txt));
}

/* return true if 'str' starts with 'txt', NOT case-sensitive */
bool StartsWithI(const WCHAR *str, const WCHAR *txt)
{
    EntryCheck(str, txt);
    return 0 == _wcsnicmp(str, txt, str::Len(txt));
}

#undef EntryCheck

// TODO: implement with templates? (must happen in the header, though)
// template <typename T> bool EndsWith(const T*, const T*) ?

bool EndsWith(const char *txt, const char *end)
{
    if (!txt || !end)
        return false;
    size_t txtLen = str::Len(txt);
    size_t endLen = str::Len(end);
    if (endLen > txtLen)
        return false;
    return str::Eq(txt + txtLen - endLen, end);
}

bool EndsWith(const WCHAR *txt, const WCHAR *end)
{
    if (!txt || !end)
        return false;
    size_t txtLen = str::Len(txt);
    size_t endLen = str::Len(end);
    if (endLen > txtLen)
        return false;
    return str::Eq(txt + txtLen - endLen, end);
}

bool EndsWithI(const char *txt, const char *end)
{
    if (!txt || !end)
        return false;
    size_t txtLen = str::Len(txt);
    size_t endLen = str::Len(end);
    if (endLen > txtLen)
        return false;
    return str::EqI(txt + txtLen - endLen, end);
}

bool EndsWithI(const WCHAR *txt, const WCHAR *end)
{
    if (!txt || !end)
        return false;
    size_t txtLen = str::Len(txt);
    size_t endLen = str::Len(end);
    if (endLen > txtLen)
        return false;
    return str::EqI(txt + txtLen - endLen, end);
}

void ReplacePtr(char **s, const char *snew)
{
    free(*s);
    *s = NULL;
    if (snew)
        *s = str::Dup(snew);
}

void ReplacePtr(WCHAR **s, const WCHAR *snew)
{
    free(*s);
    *s = NULL;
    if (snew)
        *s = str::Dup(snew);
}

/* Concatenate 2 strings. Any string can be NULL.
   Caller needs to free() memory. */
char *Join(const char *s1, const char *s2, const char *s3)
{
    if (!s1) s1= "";
    if (!s2) s2 = "";
    if (!s3) s3 = "";

    return Format("%s%s%s", s1, s2, s3);
}

/* Concatenate 2 strings. Any string can be NULL.
   Caller needs to free() memory. */
WCHAR *Join(const WCHAR *s1, const WCHAR *s2, const WCHAR *s3)
{
    if (!s1) s1 = L"";
    if (!s2) s2 = L"";
    if (!s3) s3 = L"";

    return Format(L"%s%s%s", s1, s2, s3);
}

char *DupN(const char *s, size_t lenCch)
{
    if (!s)
        return NULL;
    char *res = (char *)memdup((void *)s, lenCch + 1);
    if (res)
        res[lenCch] = 0;
    return res;
}

WCHAR *DupN(const WCHAR *s, size_t lenCch)
{
    if (!s)
        return NULL;
    WCHAR *res = (WCHAR *)memdup((void *)s, (lenCch + 1) * sizeof(WCHAR));
    if (res)
        res[lenCch] = 0;
    return res;
}

void ToLower(char *s)
{
    if (!s) return;
    for (; *s; s++)
        *s = tolower(*s);
}

void ToLower(WCHAR *s)
{
    if (!s) return;
    for (; *s; s++)
        *s = towlower(*s);
}

/* Caller needs to free() the result */
char *ToMultiByte(const WCHAR *txt, UINT codePage)
{
    AssertCrash(txt);
    if (!txt) return NULL;

    int requiredBufSize = WideCharToMultiByte(codePage, 0, txt, -1, NULL, 0, NULL, NULL);
    if (0 == requiredBufSize)
        return NULL;
    char *res = AllocArray<char>(requiredBufSize);
    if (!res)
        return NULL;
    WideCharToMultiByte(codePage, 0, txt, -1, res, requiredBufSize, NULL, NULL);
    return res;
}

/* Caller needs to free() the result */
char *ToMultiByte(const char *src, UINT codePageSrc, UINT codePageDest)
{
    AssertCrash(src);
    if (!src) return NULL;

    if (codePageSrc == codePageDest)
        return str::Dup(src);

    ScopedMem<WCHAR> tmp(ToWideChar(src, codePageSrc));
    if (!tmp)
        return NULL;

    return ToMultiByte(tmp.Get(), codePageDest);
}

/* Caller needs to free() the result */
WCHAR *ToWideChar(const char *src, UINT codePage)
{
    AssertCrash(src);
    if (!src) return NULL;

    int requiredBufSize = MultiByteToWideChar(codePage, 0, src, -1, NULL, 0);
    if (0 == requiredBufSize)
        return NULL;
    WCHAR *res = AllocArray<WCHAR>(requiredBufSize);
    if (!res)
        return NULL;
    MultiByteToWideChar(codePage, 0, src, -1, res, requiredBufSize);
    return res;
}

// Encode unicode character as utf8 to dst buffer and advance dst pointer.
// The caller must ensure there is enough free space (4 bytes) in dst
void Utf8Encode(char *& dst, int c)
{
    uint8 *tmp = (uint8*)dst;
    if (c < 0x00080) {
        *tmp++ = (uint8)(c & 0xFF);
    } else if (c < 0x00800) {
        *tmp++ = 0xC0 + (uint8)((c >> 6)&0x1F);
        *tmp++ = 0x80 + (uint8)(c & 0x3F);
    } else if (c < 0x10000) {
        *tmp++ = 0xE0 + (uint8)((c >> 12)&0x0F);
        *tmp++ = 0x80 + (uint8)((c >> 6) & 0x3F);
        *tmp++ = 0x80 + (uint8)(c & 0x3F);
    } else {
        *tmp++ = 0xF0 + (uint8)((c >> 18) & 0x07);
        *tmp++ = 0x80 + (uint8)((c >> 12) & 0x3F);
        *tmp++ = 0x80 + (uint8)((c >> 6) & 0x3F);
        *tmp++ = 0x80 + (uint8)(c & 0x3F);
    }
    dst = (char*)tmp;
}

// format string to a buffer profided by the caller
// the hope here is to avoid allocating memory (assuming vsnprintf
// doesn't allocate)
bool BufFmtV(char *buf, size_t bufCchSize, const char *fmt, va_list args)
{
    int count = _vsnprintf_s(buf, bufCchSize, _TRUNCATE, fmt, args);
    buf[bufCchSize-1] = 0;
    if ((count >= 0) && ((size_t)count < bufCchSize))
        return true;
    return false;
}

char *FmtV(const char *fmt, va_list args)
{
    char    message[256];
    size_t  bufCchSize = dimof(message);
    char  * buf = message;
    for (;;)
    {
        int count = _vsnprintf_s(buf, bufCchSize, _TRUNCATE, fmt, args);
        if ((count >= 0) && ((size_t)count < bufCchSize))
            break;
        /* we have to make the buffer bigger. The algorithm used to calculate
           the new size is arbitrary (aka. educated guess) */
        if (buf != message)
            free(buf);
        if (bufCchSize < 4*1024)
            bufCchSize += bufCchSize;
        else
            bufCchSize += 1024;
        buf = AllocArray<char>(bufCchSize);
        if (!buf)
            break;
    }

    if (buf == message)
        buf = str::Dup(message);

    return buf;
}

char *Format(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    char *res = FmtV(fmt, args);
    va_end(args);
    return res;
}

bool BufFmtV(WCHAR *buf, size_t bufCchSize, const WCHAR *fmt, va_list args)
{
    int count = _vsnwprintf_s(buf, bufCchSize, _TRUNCATE, fmt, args);
    buf[bufCchSize-1] = 0;
    if ((count >= 0) && ((size_t)count < bufCchSize))
        return true;
    return false;
}

WCHAR *FmtV(const WCHAR *fmt, va_list args)
{
    WCHAR   message[256];
    size_t  bufCchSize = dimof(message);
    WCHAR * buf = message;
    for (;;)
    {
        int count = _vsnwprintf_s(buf, bufCchSize, _TRUNCATE, fmt, args);
        if ((count >= 0) && ((size_t)count < bufCchSize))
            break;
        /* we have to make the buffer bigger. The algorithm used to calculate
           the new size is arbitrary (aka. educated guess) */
        if (buf != message)
            free(buf);
        if (bufCchSize < 4*1024)
            bufCchSize += bufCchSize;
        else
            bufCchSize += 1024;
        buf = AllocArray<WCHAR>(bufCchSize);
        if (!buf)
            break;
    }
    if (buf == message)
        buf = str::Dup(message);

    return buf;
}

WCHAR *Format(const WCHAR *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    WCHAR *res = FmtV(fmt, args);
    va_end(args);
    return res;
}

// Trim whitespace characters, in-place, inside s.
// Returns number of trimmed characters.
size_t TrimWS(WCHAR *s, TrimOpt opt)
{
    size_t sLen = str::Len(s);
    WCHAR *ns = s;
    WCHAR *e = s + sLen;
    WCHAR *ne = e;
    if ((TrimLeft == opt) || (TrimBoth == opt)) {
        while (IsWs(*ns)) {
            ++ns;
        }
    }

    if ((TrimRight == opt) || (TrimBoth == opt)) {
        while (((ne - 1) >= ns) && IsWs(ne[-1])) {
            --ne;
        }
    }
    *ne = 0;
    size_t trimmed = (ns - s) + (e - ne);
    if (ns != s) {
        size_t toCopy = (sLen - trimmed + 1) * sizeof(WCHAR); // +1 for terminating 0
        memmove(s, ns, toCopy);
    }
    return trimmed;
}

/* replace in <str> the chars from <oldChars> with their equivalents
   from <newChars> (similar to UNIX's tr command)
   Returns the number of replaced characters. */
size_t TransChars(char *str, const char *oldChars, const char *newChars)
{
    size_t findCount = 0;

    for (char *c = str; *c; c++) {
        const char *found = str::FindChar(oldChars, *c);
        if (found) {
            *c = newChars[found - oldChars];
            findCount++;
        }
    }

    return findCount;
}

size_t TransChars(WCHAR *str, const WCHAR *oldChars, const WCHAR *newChars)
{
    size_t findCount = 0;

    for (WCHAR *c = str; *c; c++) {
        const WCHAR *found = str::FindChar(oldChars, *c);
        if (found) {
            *c = newChars[found - oldChars];
            findCount++;
        }
    }

    return findCount;
}

char *Replace(const char *s, const char *toReplace, const char *replaceWith)
{
    Vec<char> res;
    char first = *toReplace++;
    size_t replaceWithLen = str::Len(replaceWith);
    while (*s) {
        if (*s != first) {
            res.Append(*s++);
            continue;
        }

        const char *tmp = s + 1;
        const char *tmp2 = toReplace;
        while (*tmp == *tmp2) {
            if (*tmp == 0) {
                res.Append(replaceWith, replaceWithLen);
                goto Exit;
            }
            ++tmp;
            ++tmp2;
        }
        if (!*tmp2) {
            res.Append(replaceWith, replaceWithLen);
            s = tmp;
        } else {
            res.Append(*s++);
        }
    }
Exit:
    return res.StealData();
}

// replaces all whitespace characters with spaces, collapses several
// consecutive spaces into one and strips heading/trailing ones
// returns the number of removed characters
size_t NormalizeWS(WCHAR *str)
{
    WCHAR *src = str, *dst = str;
    bool addedSpace = true;

    for (; *src; src++) {
        if (!IsWs(*src)) {
            *dst++ = *src;
            addedSpace = false;
        }
        else if (!addedSpace) {
            *dst++ = ' ';
            addedSpace = true;
        }
    }

    if (dst > str && IsWs(*(dst - 1)))
        dst--;
    *dst = '\0';

    return src - dst;
}

// Remove all characters in "toRemove" from "str", in place.
// Returns number of removed characters.
size_t RemoveChars(char *str, const char *toRemove)
{
    size_t removed = 0;
    char *dst = str;
    while (*str) {
        char c = *str++;
        if (!str::FindChar(toRemove, c))
            *dst++ = c;
        else
            ++removed;
    }
    *dst = '\0';
    return removed;
}

size_t RemoveChars(WCHAR *str, const WCHAR *toRemove)
{
    size_t removed = 0;
    WCHAR *dst = str;
    while (*str) {
        WCHAR c = *str++;
        if (!str::FindChar(toRemove, c))
            *dst++ = c;
        else
            ++removed;
    }
    *dst = '\0';
    return removed;
}

// Note: BufSet() should only be used when absolutely necessary (e.g. when
// handling buffers in OS-defined structures)
// returns the number of characters written (without the terminating \0)
size_t BufSet(char *dst, size_t dstCchSize, const char *src)
{
    CrashAlwaysIf(0 == dstCchSize);

    size_t srcCchSize = str::Len(src);
    size_t toCopy = min(dstCchSize - 1, srcCchSize);

    errno_t err = strncpy_s(dst, dstCchSize, src, toCopy);
    CrashIf(err || dst[toCopy] != '\0');

    return toCopy;
}

size_t BufSet(WCHAR *dst, size_t dstCchSize, const WCHAR *src)
{
    CrashAlwaysIf(0 == dstCchSize);

    size_t srcCchSize = str::Len(src);
    size_t toCopy = min(dstCchSize - 1, srcCchSize);

    errno_t err = wcsncpy_s(dst, dstCchSize, src, toCopy);
    CrashIf(err || dst[toCopy] != '\0');

    return toCopy;
}

// append as much of s at the end of dst (which must be properly null-terminated)
// as will fit. 
size_t BufAppend(char *dst, size_t dstCchSize, const char *s)
{
    CrashAlwaysIf(0 == dstCchSize);

    size_t currDstCchLen = str::Len(dst);
    if (currDstCchLen + 1 >= dstCchSize)
        return 0;
    size_t left = dstCchSize - currDstCchLen - 1;
    size_t srcCchSize = str::Len(s);
    size_t toCopy = min(left, srcCchSize);

    errno_t err = strncat_s(dst, dstCchSize, s, toCopy);
    CrashIf(err || dst[currDstCchLen + toCopy] != '\0');

    return toCopy;
}

size_t BufAppend(WCHAR *dst, size_t dstCchSize, const WCHAR *s)
{
    CrashAlwaysIf(0 == dstCchSize);

    size_t currDstCchLen = str::Len(dst);
    if (currDstCchLen + 1 >= dstCchSize)
        return 0;
    size_t left = dstCchSize - currDstCchLen - 1;
    size_t srcCchSize = str::Len(s);
    size_t toCopy = min(left, srcCchSize);

    errno_t err = wcsncat_s(dst, dstCchSize, s, toCopy);
    CrashIf(err || dst[currDstCchLen + toCopy] != '\0');

    return toCopy;
}

/* Convert binary data in <buf> of size <len> to a hex-encoded string */
char *MemToHex(const unsigned char *buf, size_t len)
{
    /* 2 hex chars per byte, +1 for terminating 0 */
    char *ret = AllocArray<char>(2 * len + 1);
    if (!ret)
        return NULL;
    char *dst = ret;
    for (; len > 0; len--) {
        sprintf_s(dst, 3, "%02x", *buf++);
        dst += 2;
    }
    return ret;
}

/* Reverse of MemToHex. Convert a 0-terminatd hex-encoded string <s> to
   binary data pointed by <buf> of max size bufLen.
   Returns false if size of <s> doesn't match bufLen or is not a valid
   hex string. */
bool HexToMem(const char *s, unsigned char *buf, size_t bufLen)
{
    for (; bufLen > 0; bufLen--) {
        int c;
        if (1 != sscanf_s(s, "%02x", &c))
            return false;
        s += 2;
        *buf++ = (unsigned char)c;
    }
    return *s == '\0';
}

// format a number with a given thousand separator e.g. it turns 1234 into "1,234"
// Caller needs to free() the result.
WCHAR *FormatNumWithThousandSep(size_t num, LCID locale)
{
    WCHAR thousandSep[4];
    if (!GetLocaleInfo(locale, LOCALE_STHOUSAND, thousandSep, dimof(thousandSep)))
        str::BufSet(thousandSep, dimof(thousandSep), L",");
    ScopedMem<WCHAR> buf(str::Format(L"%Iu", num));

    size_t resLen = str::Len(buf) + str::Len(thousandSep) * (str::Len(buf) + 3) / 3 + 1;
    WCHAR *res = AllocArray<WCHAR>(resLen);
    if (!res)
        return NULL;
    WCHAR *next = res;
    int i = 3 - (str::Len(buf) % 3);
    for (const WCHAR *src = buf; *src; ) {
        *next++ = *src++;
        if (*src && i == 2)
            next += str::BufSet(next, resLen - (next - res), thousandSep);
        i = (i + 1) % 3;
    }
    *next = '\0';

    return res;
}

// Format a floating point number with at most two decimal after the point
// Caller needs to free the result.
WCHAR *FormatFloatWithThousandSep(double number, LCID locale)
{
    size_t num = (size_t)(number * 100 + 0.5);

    ScopedMem<WCHAR> tmp(FormatNumWithThousandSep(num / 100, locale));
    WCHAR decimal[4];
    if (!GetLocaleInfo(locale, LOCALE_SDECIMAL, decimal, dimof(decimal)))
        str::BufSet(decimal, dimof(decimal), L".");

    // always add between one and two decimals after the point
    ScopedMem<WCHAR> buf(str::Format(L"%s%s%02d", tmp, decimal, num % 100));
    if (str::EndsWith(buf, L"0"))
        buf[str::Len(buf) - 1] = '\0';

    return buf.StealData();
}

// cf. http://rosettacode.org/wiki/Roman_numerals/Encode#C.2B.2B
WCHAR *FormatRomanNumeral(int number)
{
    if (number < 1)
        return NULL;

    static struct {
        int value;
        const WCHAR *numeral;
    } romandata[] = {
        { 1000, L"M" }, { 900, L"CM" }, { 500, L"D" }, { 400, L"CD" },
        {  100, L"C" }, {  90, L"XC" }, {  50, L"L" }, {  40, L"XL" },
        {   10, L"X" }, {   9, L"IX" }, {   5, L"V" }, {   4, L"IV" }, { 1, L"I" }
    };

    size_t len = 0;
    for (int num = number, i = 0; i < dimof(romandata); i++) {
        for (; num >= romandata[i].value; num -= romandata[i].value) {
            len += romandata[i].numeral[1] ? 2 : 1;
        }
    }
    assert(len > 0);

    WCHAR *roman = AllocArray<WCHAR>(len + 1), *c = roman;
    for (int num = number, i = 0; i < dimof(romandata); i++) {
        for (; num >= romandata[i].value; num -= romandata[i].value) {
            c += str::BufSet(c, romandata[i].numeral[1] ? 3 : 2, romandata[i].numeral);
        }
    }

    return roman;
}

/* compares two strings "naturally" by sorting numbers within a string
   numerically instead of by pure ASCII order; we imitate Windows Explorer
   by sorting special characters before alphanumeric characters
   (e.g. ".hg" < "2.pdf" < "100.pdf" < "zzz")
   TODO: use StrCmpLogicalW instead once we no longer support Windows 2000 */
int CmpNatural(const WCHAR *a, const WCHAR *b)
{
    CrashAlwaysIf(!a || !b);
    const WCHAR *aStart = a, *bStart = b;
    int diff = 0;

    for (; 0 == diff; a++, b++) {
        // ignore leading and trailing spaces, and differences in whitespace only
        if (a == aStart || !*a || !*b || IsWs(*a) && IsWs(*b)) {
            for (; IsWs(*a); a++);
            for (; IsWs(*b); b++);
        }
        // if two strings are identical when ignoring case, leading zeroes and
        // whitespace, compare them traditionally for a stable sort order
        if (!*a && !*b)
            return wcscmp(aStart, bStart);
        if (str::IsDigit(*a) && str::IsDigit(*b)) {
            // ignore leading zeroes
            for (; '0' == *a; a++);
            for (; '0' == *b; b++);
            // compare the two numbers as (positive) integers
            for (diff = 0; str::IsDigit(*a) || str::IsDigit(*b); a++, b++) {
                // if either *a or *b isn't a number, they differ in magnitude
                if (!str::IsDigit(*a))
                    return -1;
                if (!str::IsDigit(*b))
                    return 1;
                // remember the difference for when the numbers are of the same magnitude
                if (0 == diff)
                    diff = *a - *b;
            }
            // neither *a nor *b is a digit, so continue with them (unless diff != 0)
            a--;
            b--;
        }
        // sort letters case-insensitively
        else if (iswalnum(*a) && iswalnum(*b))
            diff = towlower(*a) - towlower(*b);
        // sort special characters before text and numbers
        else if (iswalnum(*a))
            return 1;
        else if (iswalnum(*b))
            return -1;
        // sort special characters by ASCII code
        else
            diff = *a - *b;
    }

    return diff;
}

template <typename T>
static T *ExtractUntil(const T *pos, T c, const T **endOut)
{
    *endOut = FindChar(pos, c);
    if (!*endOut)
        return NULL;
    return str::DupN(pos, *endOut - pos);
}

static const char *ParseLimitedNumber(const char *str, const char *format,
                                      const char **endOut, void *valueOut)
{
    UINT width;
    char f2[] = "% ";
    const char *endF = Parse(format, "%u%c", &width, &f2[1]);
    if (endF && FindChar("udx", f2[1]) && width <= Len(str)) {
        char limited[16]; // 32-bit integers are at most 11 characters long
        str::BufSet(limited, min(width + 1, dimof(limited)), str);
        const char *end = Parse(limited, f2, valueOut);
        if (end && !*end)
            *endOut = str + width;
    }
    return endF;
}

static const WCHAR *ParseLimitedNumber(const WCHAR *str, const WCHAR *format,
                                       const WCHAR **endOut, void *valueOut)
{
    UINT width;
    WCHAR f2[] = L"% ";
    const WCHAR *endF = Parse(format, L"%u%c", &width, &f2[1]);
    if (endF && FindChar(L"udx", f2[1]) && width <= Len(str)) {
        WCHAR limited[16]; // 32-bit integers are at most 11 characters long
        str::BufSet(limited, min(width + 1, dimof(limited)), str);
        const WCHAR *end = Parse(limited, f2, valueOut);
        if (end && !*end)
            *endOut = str + width;
    }
    return endF;
}

/* Parses a string into several variables sscanf-style (i.e. pass in pointers
   to where the parsed values are to be stored). Returns a pointer to the first
   character that's not been parsed when successful and NULL otherwise.

   Supported formats:
     %u - parses an unsigned int
     %d - parses a signed int
     %x - parses an unsigned hex-int
     %f - parses a float
     %c - parses a single WCHAR
     %s - parses a string (pass in a WCHAR**, free after use - also on failure!)
     %S - parses a string into a ScopedMem<WCHAR>
     %? - makes the next single character optional (e.g. "x%?,y" parses both "xy" and "x,y")
     %$ - causes the parsing to fail if it's encountered when not at the end of the string
     %  - skips a single whitespace character
     %_ - skips one or multiple whitespace characters (or none at all)
     %% - matches a single '%'

   %u, %d and %x accept an optional width argument, indicating exactly how many
   characters must be read for parsing the number (e.g. "%4d" parses -123 out of "-12345"
   and doesn't parse "123" at all).
*/
static const char *ParseV(const char *str, const char *format, va_list args)
{
    for (const char *f = format; *f; f++) {
        if (*f != '%') {
            if (*f != *str)
                return NULL;
            str++;
            continue;
        }
        f++;

        const char *end = NULL;
        if ('u' == *f)
            *va_arg(args, unsigned int *) = strtoul(str, (char **)&end, 10);
        else if ('d' == *f)
            *va_arg(args, int *) = strtol(str, (char **)&end, 10);
        else if ('x' == *f)
            *va_arg(args, unsigned int *) = strtoul(str, (char **)&end, 16);
        else if ('f' == *f)
            *va_arg(args, float *) = (float)strtod(str, (char **)&end);
        else if ('c' == *f)
            *va_arg(args, char *) = *str, end = str + 1;
        else if ('s' == *f)
            *va_arg(args, char **) = ExtractUntil(str, *(f + 1), &end);
        else if ('S' == *f)
            va_arg(args, ScopedMem<char> *)->Set(ExtractUntil(str, *(f + 1), &end));
        else if ('$' == *f && !*str)
            continue; // don't fail, if we're indeed at the end of the string
        else if ('%' == *f && *f == *str)
            end = str + 1;
        else if (' ' == *f && str::IsWs(*str))
            end = str + 1;
        else if ('_' == *f) {
            if (!str::IsWs(*str))
                continue; // don't fail, if there's no whitespace at all
            for (end = str + 1; str::IsWs(*end); end++);
        }
        else if ('?' == *f && *(f + 1)) {
            // skip the next format character, advance the string,
            // if it the optional character is the next character to parse
            if (*str != *++f)
                continue;
            end = (char *)str + 1;
        }
        else if (str::IsDigit(*f))
            f = ParseLimitedNumber(str, f, &end, va_arg(args, void *)) - 1;
        if (!end || end == str)
            return NULL;
        str = end;
    }
    return str;
}

const char *Parse(const char *str, const char *fmt, ...)
{
    if (!str || !fmt)
        return NULL;

    va_list args;
    va_start(args, fmt);
    const char *res = ParseV(str, fmt, args);
    va_end(args);
    return res;
}

// TODO: could optimize it by making the main Parse() implementation
// work with explicit length and not rely on zero-termination
const char *Parse(const char *str, size_t len, const char *fmt, ...)
{
    char buf[128] = { 0 };
    char *s = buf;

    if (!str || !fmt)
        return NULL;

    if (len < dimof(buf))
        memcpy(buf, str, len);
    else
        s = DupN(str, len);

    va_list args;
    va_start(args, fmt);
    const char *res = ParseV(s, fmt, args);
    va_end(args);

    if (s != buf)
        free(s);
    return res ? str + (res - s) : NULL;
}

const WCHAR *Parse(const WCHAR *str, const WCHAR *format, ...)
{
    if (!str)
        return NULL;
    va_list args;
    va_start(args, format);
    for (const WCHAR *f = format; *f; f++) {
        if (*f != '%') {
            if (*f != *str)
                goto Failure;
            str++;
            continue;
        }
        f++;

        const WCHAR *end = NULL;
        if ('u' == *f)
            *va_arg(args, unsigned int *) = wcstoul(str, (WCHAR **)&end, 10);
        else if ('d' == *f)
            *va_arg(args, int *) = wcstol(str, (WCHAR **)&end, 10);
        else if ('x' == *f)
            *va_arg(args, unsigned int *) = wcstoul(str, (WCHAR **)&end, 16);
        else if ('f' == *f)
            *va_arg(args, float *) = (float)wcstod(str, (WCHAR **)&end);
        else if ('c' == *f)
            *va_arg(args, WCHAR *) = *str, end = str + 1;
        else if ('s' == *f)
            *va_arg(args, WCHAR **) = ExtractUntil(str, *(f + 1), &end);
        else if ('S' == *f)
            va_arg(args, ScopedMem<WCHAR> *)->Set(ExtractUntil(str, *(f + 1), &end));
        else if ('$' == *f && !*str)
            continue; // don't fail, if we're indeed at the end of the string
        else if ('%' == *f && *f == *str)
            end = str + 1;
        else if (' ' == *f && str::IsWs(*str))
            end = str + 1;
        else if ('_' == *f) {
            if (!str::IsWs(*str))
                continue; // don't fail, if there's no whitespace at all
            for (end = str + 1; str::IsWs(*end); end++);
        }
        else if ('?' == *f && *(f + 1)) {
            // skip the next format character, advance the string,
            // if it the optional character is the next character to parse
            if (*str != *++f)
                continue;
            end = str + 1;
        }
        else if (str::IsDigit(*f))
            f = ParseLimitedNumber(str, f, &end, va_arg(args, void *)) - 1;
        if (!end || end == str)
            goto Failure;
        str = end;
    }
    va_end(args);
    return str;

Failure:
    va_end(args);
    return NULL;
}

size_t Utf8ToWcharBuf(const char *s, size_t sLen, WCHAR *bufOut, size_t bufOutMax)
{
    if (0 == bufOutMax)
        return 0;
    int sLenConverted = MultiByteToWideChar(CP_UTF8, 0, s, (int)sLen, NULL, 0);
    if ((size_t)sLenConverted >= bufOutMax)
        sLenConverted = (int)bufOutMax - 1;
    MultiByteToWideChar(CP_UTF8, 0, s, (int)sLen, bufOut, sLenConverted);
    bufOut[sLenConverted] = '\0';
    return sLenConverted;
}

void UrlDecodeInPlace(char *url)
{
    for (char *src = url; *src; src++, url++) {
        int val;
        if (*src == '%' && str::Parse(src, "%%%2x", &val)) {
            *url = (char)val;
            src += 2;
        } else {
            *url = *src;
        }
    }
    *url = '\0';
}

void UrlDecodeInPlace(WCHAR *url)
{
    for (WCHAR *src = url; *src; src++, url++) {
        int val;
        if (*src == '%' && str::Parse(src, L"%%%2x", &val)) {
            *url = (WCHAR)val;
            src += 2;
        } else {
            *url = *src;
        }
    }
    *url = '\0';
}

WCHAR *ToPlainUrl(const WCHAR *url)
{
    WCHAR *plainUrl = str::Dup(url);
    str::TransChars(plainUrl, L"#?", L"\0\0");
    UrlDecodeInPlace(plainUrl);
    return plainUrl;
}

namespace conv {

// not exactly a conversion, if it's ANSI, we just copy it verbatim
size_t ToCodePageBuf(char *buf, size_t cbBufSize, const char *s, UINT cp)
{
    BufSet(buf, cbBufSize, s);
    return Len(buf);
}
size_t FromCodePageBuf(char *buf, size_t cchBufSize, const char *s, UINT cp)
{
    BufSet(buf, cchBufSize, s);
    return Len(buf);
}

size_t ToCodePageBuf(char *buf, size_t cbBufSize, const WCHAR *s, UINT cp)
{
    return WideCharToMultiByte(cp, 0, s, -1, buf, cbBufSize, NULL, NULL);
}
size_t FromCodePageBuf(WCHAR *buf, size_t cchBufSize, const char *s, UINT cp)
{
    return MultiByteToWideChar(cp, 0, s, -1, buf, cchBufSize);
}

} // namespace str::conv

} // namespace str

namespace seqstrings {

// Returns NULL if s is the same as toFind
// If they are not equal, returns end of s + 1
static inline const char *StrEqWeird(const char *s, const char *toFind)
{
    char c;
    for (;;) {
        c = *s++;
        if (0 == c) {
            if (0 == *toFind)
                return NULL;
            return s;
        }
        if (c != *toFind++) {
            while (*s) {
                s++;
            }
            return s + 1;
        }
        // were equal, check another char
    }
}

// conceptually strings is an array of strings where strings are laid
// out sequentially in memory
// Returns index of toFind string in sttings array of size max
// Returns -1 if string doesn't exist
// TODO: unit test
int GetStrIdx(const char *strings, const char *toFind, int max)
{
    const char *s = strings;
    for (int idx = 0; idx < max; idx++) {
        s = StrEqWeird(s, toFind);
        if (NULL == s)
            return idx;
    }
    return -1;
}

// Given an index in the "array" of sequentially laid out strings,
// returns a strings at that index.
// TODO: unit tests
const char *GetByIdx(const char *strings, int idx)
{
    const char *s = strings;
    while (idx > 0) {
        while (*s) {
            s++;
        }
        s++;
        --idx;
    }
    return s;
}

} // namespace seqstrings
