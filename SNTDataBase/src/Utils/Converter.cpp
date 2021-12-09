#include "Converter.h"

#include <vector>
#include <codecvt>

namespace LM
{

	std::string Utf8ToStr(std::string_view _Utf8Str, const std::locale& _Loc)
	{
		std::wstring_convert<std::codecvt_utf8<wchar_t>> WConv;
		std::wstring WStr = WConv.from_bytes(_Utf8Str.data());
		std::vector<char> Buf(WStr.size());
		std::use_facet<std::ctype<wchar_t>>(_Loc).narrow(WStr.data(), WStr.data() + WStr.size(), '?', Buf.data());
		return std::string(Buf.data(), Buf.size());
	}

	std::string StrToUtf8(std::string_view _Str, const std::locale& _Loc)
	{
		std::wstring_convert<std::codecvt_utf8<wchar_t>> WConv;
		std::vector<wchar_t> Buf(_Str.size());
		std::use_facet<std::ctype<wchar_t>>(_Loc).widen(_Str.data(), _Str.data() + _Str.size(), Buf.data());
		return WConv.to_bytes(std::wstring(Buf.data(), Buf.size()));
	}

}