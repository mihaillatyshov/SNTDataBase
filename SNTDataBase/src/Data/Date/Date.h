#pragma once

#include <vector>

#include <nlohmann/json.hpp>

namespace LM
{

	class Date
	{
	public:
		Date(int _Year = 2019, int _Month = 1, int _Day = 1);
		Date(std::string_view _Str);

		void Draw() const;
		bool DrawEdit();

		const std::string GetString() const;
		const uint32_t GetInt() const;
		inline int GetDay()		const { return m_Day; }
		inline int GetMonth()	const { return m_Month; }
		inline int GetYear()	const { return m_Year; }

		void IncMonth();

		void FixDate();

		static uint32_t GetDaysInMonth(uint32_t _Month, uint32_t _Year);

		nlohmann::basic_json<> GetJson() const;
		void SetJson(nlohmann::basic_json<> _JS);

		bool operator< (const Date& _Other) const;
		bool operator> (const Date& _Other) const;
		bool operator<=(const Date& _Other) const;
		bool operator>=(const Date& _Other) const;
		bool operator==(const Date& _Other) const;
		bool operator!=(const Date& _Other) const;
		friend std::ostream& operator<<(std::ostream& _Out, const Date& _Date);
	protected:
		uint32_t m_Year = 2019;
		uint32_t m_Month = 1;
		uint32_t m_Day = 1;
	};

}
