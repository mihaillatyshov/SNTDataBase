#include "Date.h"

#include <iostream>
#include <sstream>
#include <iomanip>

#include <imgui.h>

#include "Utils/JsonUtils.h"
#include "Utils/StreamUtils.h"

namespace LM
{
	Date::Date(int _Year, int _Month, int _Day)
		: m_Year(_Year), m_Month(_Month), m_Day(_Day)
	{
		FixDate();
	}

	Date::Date(std::string_view _Str)
	{
		std::istringstream In(_Str.data());
		In >> m_Day;
		In.ignore(1, '.');
		In >> m_Month;
		In.ignore(1, '.');
		In >> m_Year;
		std::cout << *this << std::endl;
	}

	void Date::Draw() const
	{
		ImGui::Text("%02d.%02d.%d", m_Day, m_Month, m_Year);
	}

	bool Date::DrawEdit()
	{
		Date Start = *this;
		uint32_t Step = 1;

		ImGui::PushItemWidth(100);

		ImGui::Text(u8"Год"); ImGui::SameLine();
		ImGui::InputScalar(u8"##Год", ImGuiDataType_U32, &m_Year, &Step);
		
		ImGui::PopItemWidth();
		ImGui::PushItemWidth(80);

		ImGui::SameLine();
		ImGui::Text(u8"Месяц"); ImGui::SameLine();
		ImGui::InputScalar(u8"##Месяц", ImGuiDataType_U32, &m_Month, &Step);

		ImGui::SameLine();
		ImGui::Text(u8"День"); ImGui::SameLine();
		ImGui::InputScalar(u8"##День", ImGuiDataType_U32, &m_Day, &Step);

		ImGui::PopItemWidth();

		FixDate();

		return Start != *this;
	}

	uint32_t Date::GetDaysInMonth(uint32_t _Month, uint32_t _Year)
	{
		const int Days[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
		if ((_Month == 2) && (_Year % 4 == 0))
			return 29;
		return Days[_Month - 1];
	}

	const std::string Date::GetString() const
	{
		std::ostringstream Res;
		Res << Stream::Fill('0', 2) << m_Day << "." << Stream::Fill('0', 2) << m_Month << "." << m_Year;
		return Res.str();
	}

	const uint32_t Date::GetInt() const
	{
		return m_Year * 1000 + m_Month * 100 + m_Day;
	}

	void Date::IncMonth()
	{
		if (++m_Month > 12)
		{
			m_Month -= 12;
			m_Year++;
		}
		FixDate();
	}
	
	void Date::FixDate()
	{
		m_Year	= std::max(		m_Year,		0u);
		m_Month = std::clamp(	m_Month,	1u, 12u);
		m_Day	= std::clamp(	m_Day,		1u, GetDaysInMonth(m_Month, m_Year));
	}

	nlohmann::basic_json<> Date::GetJson() const
	{
		nlohmann::basic_json<> Result;
		Result["Year"]	= m_Year;
		Result["Month"] = m_Month;
		Result["Day"]	= m_Day;

		return Result;
	}

	void Date::SetJson(nlohmann::basic_json<> _JS)
	{
		if (!_JS.is_object())
			return;

		nlohmann::SetValue(m_Year,	_JS, "Year");
		nlohmann::SetValue(m_Month, _JS, "Month");
		nlohmann::SetValue(m_Day,	_JS, "Day");
	}

	bool Date::operator<(const Date& _Other) const
	{
		return GetInt() < _Other.GetInt();
	}

	bool Date::operator>(const Date& _Other) const
	{
		return GetInt() > _Other.GetInt();
	}

	bool Date::operator<=(const Date& _Other) const
	{
		return !(*this > _Other);
	}

	bool Date::operator>=(const Date& _Other) const
	{
		return !(*this < _Other);
	}

	bool Date::operator==(const Date& _Other) const
	{
		return GetInt() == _Other.GetInt();
	}

	bool Date::operator!=(const Date& _Other) const
	{
		return !(*this == _Other);
	}

	std::ostream& operator<<(std::ostream& _Out, const Date& _Date)
	{
		_Out << Stream::Fill('0', 2) << _Date.m_Day << "." << Stream::Fill('0', 2) << _Date.m_Month << "." << _Date.m_Year;
		return _Out;
	}

}
