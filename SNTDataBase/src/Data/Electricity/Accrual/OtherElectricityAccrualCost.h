#pragma once

#include <nlohmann/json.hpp>

#include "Data/Money/Money.h"

namespace LM
{

	class OtherElectricityAccrualCost
	{
	public:
		OtherElectricityAccrualCost() = default;
		OtherElectricityAccrualCost(std::string_view _Name, const Money& _Money)
			: m_Name(_Name), m_Money(_Money) { }

		bool DrawEdit();

		inline const Money& GetMoney() const { return m_Money; }

		nlohmann::basic_json<> GetJson() const;
		void SetJson(nlohmann::basic_json<> _JS);
	protected:
		std::string m_Name = "<NONE>";
		Money       m_Money;
	};

}
