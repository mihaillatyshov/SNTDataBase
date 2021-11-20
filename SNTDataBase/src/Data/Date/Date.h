#pragma once

#include <vector>

#include <nlohmann/json.hpp>

namespace LM
{

	class Date
	{
	public:
		Date(int _Year = 2019, int _Month = 1, int _Day = 1);

		void Draw() const;
		bool DrawEdit();

		const std::string GetString() const;
		inline int GetYear() const { return m_Year; }
		inline int GetMonth() const { return m_Month; }

		void IncMonth();

		void FixDate();

		static int GetDaysInMonth(int _Month, int _Year);

		nlohmann::basic_json<> GetJson() const;
		void SetJson(nlohmann::basic_json<> _JS);

		bool operator< (const Date& _Other) const;
		bool operator> (const Date& _Other) const;
		bool operator<=(const Date& _Other) const;
		bool operator>=(const Date& _Other) const;
		bool operator==(const Date& _Other) const;
		bool operator!=(const Date& _Other) const;
	public:
		int m_Year = 2019;
		int m_Month = 1;
		int m_Day = 1;
	};

}
