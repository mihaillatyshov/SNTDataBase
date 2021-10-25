#include "Date.h"

#include <nlohmann/JsonUtils.h>
#include <imgui.h>

namespace LM
{

	void Date::Draw() const
	{
		ImGui::Text("%02d.%02d.%d", m_Day, m_Month, m_Year);
	}

	bool Date::DrawEdit()
	{
		Date Start = *this;

		ImGui::PushItemWidth(100);

		ImGui::Text(u8"Год"); ImGui::SameLine();
		if (ImGui::InputInt(u8"##Год", &m_Year))
		{
			FixDate();
		}
		ImGui::SameLine();

		ImGui::PopItemWidth();
		ImGui::PushItemWidth(80);

		ImGui::Text(u8"Месяц"); ImGui::SameLine();
		if (ImGui::InputInt(u8"##Месяц", &m_Month))
		{
			FixDate();
		}

		ImGui::SameLine();
		ImGui::Text(u8"День"); ImGui::SameLine();
		if (ImGui::InputInt(u8"##День", &m_Day))
		{
			FixDate();
		}

		ImGui::PopItemWidth();

		return Start != *this;

	}

	int Date::GetDaysInMonth(int _Month, int _Year) const
	{
		const int Days[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
		if ((_Month == 2) && (_Year % 4 == 0))
			return 29;
		return Days[_Month - 1];
	}

	const std::string Date::GetString() const
	{
		char date[20]{ 0 };
		sprintf(date, u8"%02d.%02d.%d", m_Day, m_Month, m_Year);
		return date;
	}

	void Date::FixDate()
	{
		int MinYear = 0;
		if (m_Year < MinYear)
			m_Year = MinYear;

		int MinMonth = 1; int MaxMonth = 12;
		if (m_Month < MinMonth)
			m_Month = MinMonth;
		else if (m_Month > MaxMonth)
			m_Month = MaxMonth;

		int MinDay = 1; int MaxDay = GetDaysInMonth(m_Month, m_Year);
		if (m_Year % 4 == 0) MaxDay++;
		if (m_Day < MinDay)
			m_Day = MinDay;
		else if (m_Day > MaxDay)
			m_Day = MaxDay;
	}

	nlohmann::basic_json<> Date::GetJson() const
	{
		nlohmann::basic_json<> result;
		result["Year"] = m_Year;
		result["Month"] = m_Month;
		result["Day"] = m_Day;

		return result;
	}

	void Date::SetJson(nlohmann::basic_json<> js)
	{
		if (!js.is_object())
			return;

		nlohmann::SetValue(m_Year, js, "Year");
		nlohmann::SetValue(m_Month, js, "Month");
		nlohmann::SetValue(m_Day, js, "Day");
	}

	bool Date::operator< (const Date& _Other) const
	{
		if (m_Year < _Other.m_Year)
			return true;
		else if (m_Year > _Other.m_Year)
			return false;

		if (m_Month < _Other.m_Month)
			return true;
		else if (m_Month > _Other.m_Month)
			return false;

		if (m_Day < _Other.m_Day)
			return true;

		return false;
	}

	bool Date::operator> (const Date& _Other) const
	{
		if (m_Year > _Other.m_Year)
			return true;
		else if (m_Year < _Other.m_Year)
			return false;

		if (m_Month > _Other.m_Month)
			return true;
		else if (m_Month < _Other.m_Month)
			return false;

		if (m_Day > _Other.m_Day)
			return true;

		return false;
	}

	bool Date::operator<=(const Date& _Other) const
	{
		return !(*this > _Other);
	}

	bool Date::operator>=(const Date& _Other) const
	{
		return !(*this > _Other);
	}

	bool Date::operator==(const Date& _Other) const
	{
		if (m_Year == _Other.m_Year && m_Month == _Other.m_Month && m_Day == _Other.m_Day)
			return true;

		return false;
	}

	bool Date::operator!=(const Date& _Other) const
	{
		return !(*this == _Other);
	}

}
