#pragma once

#include "UI/Tabs/TabDataStruct/TabDataStruct.h"
#include "ElectricityAccrualData.h"

namespace LM
{

	class ElectricityAccrualTabDS : public TabDataStruct
	{
	public:
		ElectricityAccrualTabDS(const ElectricityAccrualData& _Data = ElectricityAccrualData())
			: m_Data(_Data) { }

		inline const ElectricityAccrualData& GetData() const { return m_Data; }

		void Draw() override;
	protected:
		ElectricityAccrualData m_Data;
	};

}