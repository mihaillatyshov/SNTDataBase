#include "MembershipFee.h"

#include <algorithm>
#include <iostream>

#include "Utils/JsonUtils.h"

namespace LM
{

	void MembershipFee::AddPayment(Ref<const TabDataStruct> _TabDS)
	{
		m_Payments.push_back(CreateRef<Payment>(_TabDS));
		m_Debt -= std::static_pointer_cast<const PaymentTabDS>(_TabDS)->GetData().Amount;
		SortPayments();
	}

	void MembershipFee::EditPayment(size_t _PayId, Ref<const TabDataStruct> _TabDS)
	{
		auto NewData = std::static_pointer_cast<const PaymentTabDS>(_TabDS)->GetData();
		m_Debt += m_Payments[_PayId]->m_Data.Amount;
		m_Payments[_PayId]->Edit(_TabDS);
		m_Debt -= NewData.Amount;
		SortPayments();
	}

	void MembershipFee::DeletePayment(size_t _PayId)
	{
		m_Debt += m_Payments[_PayId]->m_Data.Amount;
		m_Payments.erase(m_Payments.begin() + _PayId);
	}

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
