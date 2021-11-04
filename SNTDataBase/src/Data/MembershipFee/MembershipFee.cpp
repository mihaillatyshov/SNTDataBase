#include "MembershipFee.h"

#include <algorithm>
#include <iostream>

#include "Utils/JsonUtils.h"

namespace LM
{

	void MembershipFee::SortPayments()
	{
		std::sort(m_Payments.begin(), m_Payments.end(), [](const Ref<const Payment> first, const Ref<const Payment> second)
			{
				return first->GetDate() > second->GetDate();
			});
	}

	nlohmann::basic_json<> MembershipFee::GetJson() const
	{
		nlohmann::basic_json<> result;
		result["Debt"]				= m_Debt.GetJson();
		result["OpeningBalance"]	= m_OpeningBalance.m_Money.GetJson();

		result["Payments"] = nlohmann::GetVector(m_Payments);

		return result;
	}

	void MembershipFee::SetJson(nlohmann::basic_json<> js)
	{
		if (!js.is_object())
			return;

		m_Debt.SetJson(					 js["Debt"]);
		m_OpeningBalance.m_Money.SetJson(js["OpeningBalance"]);

		nlohmann::SetVector(m_Payments, js, "Payments");
	}

}
