#pragma once

#include <nlohmann/json.hpp>

#include "KiloWatt.h"
#include "ElectricityAccuralCosts.h"
#include "../Date/Date.h"

namespace LM
{

	class ElectricityAccural
	{
	public:
		nlohmann::basic_json<> GetJson() const;
		void SetJson(nlohmann::basic_json<> _JS);
		inline KiloWatt GetAllMonth() const { return m_Day + m_Night; }
	public:
		Date					m_Date;
		KiloWatt				m_Day;
		KiloWatt				m_Night;
		KiloWatt				m_AllMonth;
		ElectricityAccuralCosts m_Costs;
	};

}
