#pragma once

#include <nlohmann/json.hpp>

#include "../KiloWatt.h"
#include "ElectricityAccrualCosts.h"
#include "Data/Date/Date.h"
#include "UI/Table/TableElement.h"
#include "UI/Tabs/TabElement.h"
#include "ElectricityAccrualData.h"
#include "ElectricityAccrualTabDS.h"

namespace LM
{

	class ElectricityAccrual : public TableElement, public TabElement
	{
	public:
		ElectricityAccrual() = default;
		ElectricityAccrual(Ref<const TabDataStruct> _TabDS);

		void Edit(Ref<const TabDataStruct> _TabDS);

		inline KiloWatt GetAllMonth() const { return m_Data.Day + m_Data.Night; }

		virtual std::vector<std::function<void(void)>> GetDrawableColumns() const override;

		void FillDataStruct(Ref<TabDataStruct>& _TabDS) const override;

		nlohmann::basic_json<> GetJson() const;
		void SetJson(nlohmann::basic_json<> _JS);
	public:
		ElectricityAccrualData m_Data;
	};

}
