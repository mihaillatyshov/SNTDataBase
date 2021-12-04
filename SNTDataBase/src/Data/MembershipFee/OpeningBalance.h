#pragma once

#include "../Payment/Payment.h"

namespace LM
{
	class OpeningBalance
	{
	public:
		static inline void SetDate(const Date& _Date) { s_Date = _Date; }
		static inline const Date& GetDate() { return s_Date; }

		inline void SetMoney(const Money& _Money) { m_Money = _Money; }
		inline const Money& GetMoney() const { return m_Money; }

		nlohmann::basic_json<> GetJson() const;
		void SetJson(nlohmann::basic_json<> _JS);

		static nlohmann::basic_json<> GetDateJson();
		static void SetDateJson(nlohmann::basic_json<> _JS);
	protected:
		static inline Date s_Date;
	protected:
		Money m_Money;
	};
}
