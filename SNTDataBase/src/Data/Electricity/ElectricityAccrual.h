#pragma once

#include <nlohmann/json.hpp>

#include "KiloWatt.h"
#include "ElectricityAccrualCosts.h"
#include "../Date/Date.h"
#include "UI/Table/TableElement.h"

namespace LM
{

	class ElectricityAccrual : public TableElement
	{
	public:
		inline KiloWatt GetAllMonth() const { return m_Day + m_Night; }

		virtual std::vector<std::function<void(void)>> GetDrawableColumns() const override;

		nlohmann::basic_json<> GetJson() const;
		void SetJson(nlohmann::basic_json<> _JS);
	public:
		Date					m_Date;
		KiloWatt				m_Day;
		KiloWatt				m_Night;
		ElectricityAccrualCosts m_Costs;
	};

}
