#pragma once

#include "../Payment/Payment.h"
#include <nlohmann/json.hpp>

namespace LM
{

	class Accrual
	{
	public:
		static inline Money MembershipFeeAmount = Money(500);
		nlohmann::basic_json<> GetJson() const;
		void SetJson(nlohmann::basic_json<> js);
	public:
		Date m_Date;
		Money m_Money;
	};

}
