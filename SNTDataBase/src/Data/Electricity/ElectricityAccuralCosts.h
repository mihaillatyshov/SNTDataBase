#pragma once

#include <nlohmann/json.hpp>

#include "OtherElectricityAccuralCost.h"
#include "../Money/Money.h"

namespace LM
{

	class ElectricityAccuralCosts
	{
	public:
		ElectricityAccuralCosts() = default;
		ElectricityAccuralCosts(const Money& _Day, const Money& _Night, const std::vector<OtherElectricityAccuralCost> _Constants)
			: m_Day(_Day), m_Night(_Night), m_Others(_Constants) { }

		nlohmann::basic_json<> GetJson() const;
		void SetJson(nlohmann::basic_json<> _JS);

	public:
		Money m_Day	  = Money(6, 57);
		Money m_Night = Money(4, 13);

		// Sum of standart constants = 141
		std::vector<OtherElectricityAccuralCost> m_Others
		{
			{ u8"������������� �����������",	Money(60) },
			{ u8"������",						Money(25) },
			{ u8"��������� � ��������",			Money(15) },
			{ u8"8���",							Money(40, 50) },
		};
	};

}
