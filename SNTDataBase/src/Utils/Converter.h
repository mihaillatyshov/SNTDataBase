#pragma once

#include <string>
#include <locale>

namespace LM
{

	std::string Utf8ToStr(std::string_view _Utf8Str,	const std::locale& _Loc = std::locale(".1251"));
	std::string StrToUtf8(std::string_view _Str,		const std::locale& _Loc = std::locale(".1251"));

}