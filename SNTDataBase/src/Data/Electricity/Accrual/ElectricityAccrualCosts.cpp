#include "ElectricityAccrualCosts.h"

#include "Utils/JsonUtils.h"

namespace LM
{

	nlohmann::basic_json<> ElectricityAccrualCosts::GetJson() const
	{
		nlohmann::basic_json<> Result;
		Result["Day"]	= m_Day.GetJson();
		Result["Night"] = m_Night.GetJson();

		Result["Others"] = nlohmann::GetVector(m_Others);

		return Result;
	}

	void ElectricityAccrualCosts::SetJson(nlohmann::basic_json<> _JS)
	{
		if (!_JS.is_object())
			return;

		m_Day.SetJson(	_JS["Day"]);
		m_Night.SetJson(_JS["Night"]);

		nlohmann::SetVector(m_Others, _JS, "Others");
	}

}