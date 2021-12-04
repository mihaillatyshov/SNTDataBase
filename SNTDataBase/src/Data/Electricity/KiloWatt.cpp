#include "KiloWatt.h"

#include <iostream>

#include <imgui.h>

#include "Utils/JsonUtils.h"
#include "Utils/ImGuiUtils.h"

namespace LM
{

	KiloWatt::KiloWatt(int64_t _KiloWatts, int64_t _Watts)
	{
		m_Watt = _KiloWatts * 1000 + _Watts;
	}

	KiloWatt::KiloWatt(std::string _Str)
	{
		int64_t KiloWatts;
		int64_t Watts;

		for (auto It = _Str.find(' '); It != std::string::npos; It = _Str.find(' '))
		{
			_Str.erase(It, 1);
		}

		auto It = _Str.find(',');
		if (It != std::string::npos)
			_Str[It] = '.';

		sscanf(_Str.data(), "%lld.%03lld", &KiloWatts, &Watts);
		m_Watt = KiloWatts * 1000 + Watts;
	}

	void KiloWatt::Draw() const
	{
		ImGui::Text("%lld.%03lld", m_Watt / 1000, m_Watt % 1000);
	}

	bool KiloWatt::DrawEdit(std::string_view _FieldName)
	{
		ImGuiDirtyDecorator DirtyDecorator;
		char label[100];
		sprintf(label, "%lld.%03lld", m_Watt / 1000, abs(m_Watt % 1000));
		ImGui::PushItemWidth(250);
		if (ImGui::InputText(_FieldName.data(), label, 100, ImGuiInputTextFlags_CharsDecimal))
		{
			int64_t Kwatt = 0, Watt = 0, Watt1 = 0, Watt2 = 0, Watt3 = 0;
			sscanf(label, "%lld.%1lld%1lld%1lld", &Kwatt, &Watt1, &Watt2, &Watt3);
			Watt = 100 * abs(Watt1) + 10 * abs(Watt2) + abs(Watt3);

			m_Watt = abs(Kwatt * 1000) + abs(Watt);
			if (Kwatt < 0)
				m_Watt = -m_Watt;
			DirtyDecorator(true);
		}
		ImGui::PopItemWidth();

		return DirtyDecorator;
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
		_Out << _KiloWatt.m_Watt / 1000 << "." << _KiloWatt.m_Watt % 1000;
		return _Out;
	}

	//const KiloWatt operator+(const KiloWatt& left, const KiloWatt& right)
	//{
	//    KiloWatt res;
	//    res.m_Watt = left.m_Watt + right.m_Watt;
	//    return res;
	//}
	//
	//const KiloWatt operator-(const KiloWatt& left, const KiloWatt& right)
	//{
	//    KiloWatt res;
	//    res.m_Watt = left.m_Watt - right.m_Watt;
	//    return res;
	//}

}
