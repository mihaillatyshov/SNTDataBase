#include "MembershipFee.h"

#include <algorithm>
#include <iostream>

#include "Utils/JsonUtils.h"
#include "Utils/Time.h"

namespace LM
{

	void MembershipFee::AddPayment(Ref<const TabDataStruct> _TabDS)
	{
		m_Payments.push_back(CreateRef<Payment>(_TabDS));
	}

	void MembershipFee::EditPayment(size_t _PayId, Ref<const TabDataStruct> _TabDS)
	{
		m_Payments[_PayId]->Edit(std::static_pointer_cast<const PaymentTabDS>(_TabDS));
	}

	void MembershipFee::DeletePayment(size_t _PayId)
	{
		m_Payments.erase(m_Payments.begin() + _PayId);
	}

	void MembershipFee::SortPayments()
	{
		std::sort(m_Payments.begin(), m_Payments.end(), [](const Ref<const Payment> first, const Ref<const Payment> second)
			{
				return first->GetDate() > second->GetDate();
			});
	}

	void MembershipFee::SortAccruals()
	{
		std::sort(s_Accruals.begin(), s_Accruals.end(), [](const MembershipFeeAccrual& _First, const MembershipFeeAccrual& _Second)
			{
				return _First.m_Date < _Second.m_Date;
			});
	}

	void MembershipFee::AddAccrual(const MembershipFeeAccrual& _Accrual)
	{
		s_Accruals.push_back(_Accrual);
		SortAccruals();
	}

	void MembershipFee::DeleteAccrual(Date _Date)
	{
		s_Accruals.erase(std::find_if(s_Accruals.begin(), s_Accruals.end(), 
			[=](const MembershipFeeAccrual& _Accrual) 
			{
				return _Accrual.m_Date == _Date;
			})
		);
		SortAccruals();
	}

	void MembershipFee::GetAccrualsToAdd(std::vector<MembershipFeeAccrual>& _AccrualsToAdd)
	{
		_AccrualsToAdd.clear();
		auto LocalTime = Time::GetLocalTime();

		int NowYear = 1900 + LocalTime->tm_year;
		int NowMonth = 1 + LocalTime->tm_mon;
		int Size = (NowYear - OpeningBalance::s_Date.m_Year) * 12 - OpeningBalance::s_Date.m_Month + NowMonth;
		Date CheckDate(OpeningBalance::s_Date.GetYear(), OpeningBalance::s_Date.GetMonth());
		for (int i = 0; i < Size; i++)
		{
			CheckDate.IncMonth();
			if (std::find_if(s_Accruals.begin(), s_Accruals.end(),
				[=](const MembershipFeeAccrual& _Acc) { return _Acc.m_Date == CheckDate; }) == s_Accruals.end())
			{
				_AccrualsToAdd.push_back(MembershipFeeAccrual(CheckDate, MembershipFeeAccrual::MembershipFeeAmount));
			}
		}
	}

	void MembershipFee::GetAccrualsToDelete(std::vector<Date>& _AccrualsToDelete)
	{
		_AccrualsToDelete.clear();
		for (int i = 0; i < s_Accruals.size(); i++)
		{
			std::cout << "Accrual check " << i << ": " << s_Accruals[i].m_Date.GetString() << " " << OpeningBalance::s_Date.GetString();
			if (s_Accruals[i].m_Date <= OpeningBalance::s_Date)
			{
				std::cout << " need to delete";
				_AccrualsToDelete.push_back(s_Accruals[i].m_Date);
			}
			std::cout << std::endl;
		}
	}

	void MembershipFee::Recalculate(const Privilege& _Privilege)
	{
		SortPayments();
		m_Debt = m_OpeningBalance.m_Money;
		for (const auto& Accrual : s_Accruals)
		{
			if (!_Privilege.HasPrivilege || Accrual.m_Date < _Privilege.Start)
				m_Debt += Accrual.m_Money;
		}

		for (const auto& Pay : m_Payments)
		{
			m_Debt -= Pay->GetAmount();
		}
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
