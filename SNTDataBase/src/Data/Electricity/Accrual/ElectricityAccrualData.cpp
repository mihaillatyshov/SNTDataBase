#include "ElectricityAccrualData.h"

#include "Data/Constants/Constants.h"

namespace LM
{
	ElectricityAccrualData::ElectricityAccrualData()
	{
		Costs = Constants::Get()->GetCosts();
	}

	ElectricityAccrualData::ElectricityAccrualData(const Date& _When, const KiloWatt& _Day, const KiloWatt& _Night, const ElectricityAccrualCosts& _Costs)
		: When(_When), Day(_Day), Night(_Night), Costs(_Costs)
	{

	}


}