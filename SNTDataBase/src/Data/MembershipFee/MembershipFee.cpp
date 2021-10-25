#include "MembershipFee.h"
#include <time.h>

#include <nlohmann/JsonUtils.h>
#include <algorithm>
#include <iostream>

namespace LM
{

	void MembershipFee::SortPayments()
	{
		std::sort(m_Payments.begin(), m_Payments.end(), [](std::shared_ptr<Payment> first, std::shared_ptr<Payment> second)
			{
				return first->GetDate() > second->GetDate();
			});
	}

	nlohmann::basic_json<> MembershipFee::GetJson() const
	{
		nlohmann::basic_json<> result;
		result["Debt"]				= m_Debt.GetJson();
		result["OpeningBalance"]	= m_OpeningBalance.m_Money.GetJson();

		result["Payments"] = nlohmann::GetVectorSptr(m_Payments);

		return result;
	}

	void MembershipFee::SetJson(nlohmann::basic_json<> js)
	{
		if (!js.is_object())
			return;

		m_Debt.SetJson(					 js["Debt"]);
		m_OpeningBalance.m_Money.SetJson(js["OpeningBalance"]);

		nlohmann::SetVectorSptr(m_Payments, js, "Payments");
	}

}
