#include "OtherElectricityAccrualCost.h"

#include "Utils/JsonUtils.h"

namespace LM
{

	nlohmann::basic_json<> OtherElectricityAccrualCost::GetJson() const
	{
		nlohmann::basic_json<> Result;
		Result["Name"]	= m_Name;
		Result["Money"] = m_Money.GetJson();

		return Result;
	}

	void OtherElectricityAccrualCost::SetJson(nlohmann::basic_json<> _JS)
	{
		nlohmann::SetValue(m_Name, _JS, "Name");
		m_Money.SetJson(_JS["Money"]);
	}

}
