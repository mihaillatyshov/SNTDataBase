#pragma once

#include <nlohmann/json.hpp>

#include "Data/Money/Money.h"
#include "Data/Date/Date.h"

namespace LM
{

	class MembershipFeeAccrual
	{
	public:
		MembershipFeeAccrual(Date _Date = Date(), Money _Money = Money());

		inline const Date& GetDate() const { return m_Date; }
		inline const Money& GetMoney() const { return m_Money; }
		static inline const Money& GetConstantAmount() { return s_ConstantAmount; }

		bool DrawEdit();

		nlohmann::basic_json<> GetJson() const;
		void SetJson(nlohmann::basic_json<> _JS);
	protected:
		static inline Money s_ConstantAmount = Money(500);
	protected:
		Date m_Date;
		Money m_Money;
	};

}
