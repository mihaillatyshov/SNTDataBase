#pragma once

#include <nlohmann/json.hpp>

#include "Data/Money/Money.h"
#include "Data/Date/Date.h"

namespace LM
{

	class MembershipFeeAccrual
	{
	public:
		static inline Money MembershipFeeAmount = Money(500);
		nlohmann::basic_json<> GetJson() const;
		void SetJson(nlohmann::basic_json<> _JS);
	public:
		Date m_Date;
		Money m_Money;
	};

}
