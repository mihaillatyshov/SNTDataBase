#pragma once

#include <nlohmann/json.hpp>

#include "OtherElectricityAccrualCost.h"
#include "Data/Money/Money.h"

namespace LM
{

	class ElectricityAccrualCosts
	{
	public:
		ElectricityAccrualCosts() = default;
		ElectricityAccrualCosts(const Money& _Day, const Money& _Night, const std::vector<OtherElectricityAccrualCost> _Constants)
			: m_Day(_Day), m_Night(_Night), m_Others(_Constants) { }

		bool DrawEdit();
		
		inline const Money&										GetDay()	const { return m_Day; }
		inline const Money&										GetNight()	const { return m_Night; }
		inline const std::vector<OtherElectricityAccrualCost>&	GetOthres() const { return m_Others; }
		
		const Money& GetOthersSum() const;

		nlohmann::basic_json<> GetJson() const;
		void SetJson(nlohmann::basic_json<> _JS);

	protected:
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
