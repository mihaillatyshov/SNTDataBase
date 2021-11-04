#pragma once

#include <nlohmann/json.hpp>

#include "OtherElectricityAccrualCost.h"
#include "../Money/Money.h"

namespace LM
{

	class ElectricityAccrualCosts
	{
	public:
		ElectricityAccrualCosts() = default;
		ElectricityAccrualCosts(const Money& _Day, const Money& _Night, const std::vector<OtherElectricityAccrualCost> _Constants)
			: m_Day(_Day), m_Night(_Night), m_Others(_Constants) { }

		nlohmann::basic_json<> GetJson() const;
		void SetJson(nlohmann::basic_json<> _JS);

	public:
		Money m_Day	  = Money(6, 57);
		Money m_Night = Money(4, 13);

		// Sum of standart constants = 141
		std::vector<OtherElectricityAccrualCost> m_Others
		{
			{ u8"������������� �����������",	Money(60) },
			{ u8"������",						Money(25) },
			{ u8"��������� � ��������",			Money(15) },
			{ u8"8���",							Money(40, 50) },
		};
	};

}
