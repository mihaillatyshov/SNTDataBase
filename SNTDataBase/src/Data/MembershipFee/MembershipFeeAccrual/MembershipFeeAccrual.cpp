#include "MembershipFeeAccrual.h"

#include "Utils/JsonUtils.h"

namespace LM
{

	nlohmann::basic_json<> MembershipFeeAccrual::GetJson() const
	{
		nlohmann::basic_json<> Result;
		Result["Date"]	= m_Date.GetJson();
		Result["Money"] = m_Money.GetJson();

		return Result;
	}

	void MembershipFeeAccrual::SetJson(nlohmann::basic_json<> _JS)
	{
		if (!_JS.is_object())
			return;

		m_Date.SetJson(_JS["Date"]);
		m_Money.SetJson(_JS["Money"]);
	}

}
