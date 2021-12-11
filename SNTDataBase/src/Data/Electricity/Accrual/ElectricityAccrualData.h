#pragma once

#include "Data/Date/Date.h"
#include "Data/Electricity/KiloWatt.h"
#include "ElectricityAccrualCosts.h"

namespace LM
{
	
	struct ElectricityAccrualData
	{
		ElectricityAccrualData();
		ElectricityAccrualData(const Date& _When, const KiloWatt& _Day, const KiloWatt& _Night, const ElectricityAccrualCosts& _Costs);

		Date					When;
		KiloWatt				Day;
		KiloWatt				Night;
		ElectricityAccrualCosts Costs;
	};

}