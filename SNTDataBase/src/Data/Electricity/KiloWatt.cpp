#include "KiloWatt.h"

#include <iostream>
#include <sstream>

#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>

#include "Utils/JsonUtils.h"
#include "Utils/ImGuiUtils.h"
#include "Utils/StreamUtils.h"

namespace LM
{

	KiloWatt::KiloWatt(int64_t _KiloWatts, int64_t _Watts)
	{
		m_Watt = _KiloWatts * 1000 + _Watts;
	}

	KiloWatt::KiloWatt(std::string _Str)
	{
		for (auto It = _Str.find(' '); It != std::string::npos; It = _Str.find(' '))
			_Str.erase(It, 1);

		auto It = _Str.find(',');
		if (It != std::string::npos)
			_Str[It] = '.';

		m_Watt = GetIntFromSplitStr(_Str, 3);
	}

	void KiloWatt::Draw() const
	{
		ImGui::Text("%lld.%03lld", m_Watt / 1000, m_Watt % 1000);
	}

	bool KiloWatt::DrawEdit(std::string_view _FieldName)
	{
		ImGuiDirtyDecorator DirtyDecorator;

		std::string Buffer = GetString();

		ImGui::PushItemWidth(250);
		if (ImGui::InputText(_FieldName.data(), &Buffer, ImGuiInputTextFlags_CharsDecimal))
		{
			m_Watt = GetIntFromSplitStr(Buffer, 3u);
			DirtyDecorator(true);
		}
		ImGui::PopItemWidth();

		return DirtyDecorator;
	}

	std::string KiloWatt::GetString() const
	{
		std::ostringstream Res;
		Res << m_Watt / 1000 << '.' << Stream::Fill('0', 3) << abs(m_Watt % 1000);
		return Res.str();
	}

	nlohmann::basic_json<> KiloWatt::GetJson() const
	{
		nlohmann::basic_json<> result;
		result["Watt"] = m_Watt;

		return result;
	}

	void KiloWatt::SetJson(nlohmann::basic_json<> js)
	{
		nlohmann::SetValue(m_Watt, js, "Watt");
	}

	const KiloWatt KiloWatt::operator+(const KiloWatt& _Other) const
	{
		KiloWatt Res;
		Res.m_Watt = m_Watt + _Other.m_Watt;
		return Res;
	}

	const KiloWatt KiloWatt::operator-(const KiloWatt& _Other) const
	{
		KiloWatt Res;
		Res.m_Watt = m_Watt - _Other.m_Watt;
		return Res;
	}

	std::ostream& operator<<(std::ostream& _Out, const KiloWatt& _KiloWatt)
	{
		_Out << _KiloWatt.GetString();
		return _Out;
	}

}
