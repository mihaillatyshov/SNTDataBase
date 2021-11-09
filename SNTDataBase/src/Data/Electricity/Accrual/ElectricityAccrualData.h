#pragma once

#include "../KiloWatt.h"
#include "Data/Date/Date.h"
#include "ElectricityAccrualCosts.h"

namespace LM
{
	struct ElectricityAccrualData
	{
		Date					Date;
		KiloWatt				Day;
		KiloWatt				Night;
		ElectricityAccrualCosts Costs;
	};

}