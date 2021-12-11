#include "MembershipFee.h"

#include <algorithm>
#include <iostream>

#include "Data/Constants/Constants.h"
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
				return _First.GetDate() < _Second.GetDate();
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
				return _Accrual.GetDate() == _Date;
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
		int Size = (NowYear - OpeningBalance::GetDate().GetYear()) * 12 - OpeningBalance::GetDate().GetMonth() + NowMonth;
		Date CheckDate(OpeningBalance::GetDate().GetYear(), OpeningBalance::GetDate().GetMonth());
		for (int i = 0; i < Size; i++)
		{
			CheckDate.IncMonth();
			if (std::find_if(s_Accruals.begin(), s_Accruals.end(),
				[=](const MembershipFeeAccrual& _Acc) { return _Acc.GetDate() == CheckDate; }) == s_Accruals.end())
			{
				_AccrualsToAdd.push_back(MembershipFeeAccrual(CheckDate, Constants::Get()->GetAccrualMF()));
			}
		}
	}

	void MembershipFee::GetAccrualsToDelete(std::vector<Date>& _AccrualsToDelete)
	{
		_AccrualsToDelete.clear();
		for (int i = 0; i < s_Accruals.size(); i++)
		{
			std::cout << "Accrual check " << i << ": " << s_Accruals[i].GetDate().GetString() << " " << OpeningBalance::GetDate().GetString();
			if (s_Accruals[i].GetDate() <= OpeningBalance::GetDate())
			{
				std::cout << " need to delete";
				_AccrualsToDelete.push_back(s_Accruals[i].GetDate());
			}
			std::cout << std::endl;
		}
	}

	void MembershipFee::Recalculate(const Privilege& _Privilege)
	{
		SortPayments();
		m_Debt = m_OpeningBalance.GetMoney();
		for (const auto& Accrual : s_Accruals)
		{
			if (!_Privilege.GetHasPrivilege(Accrual.GetDate()))
				m_Debt += Accrual.GetMoney();
		}

		for (const auto& Pay : m_Payments)
		{
			m_Debt -= Pay->GetAmount();
		}
	}

	nlohmann::basic_json<> MembershipFee::GetJson() const
	{
		nlohmann::basic_json<> Result;
		Result["Debt"]				= m_Debt.GetJson();
		Result["OpeningBalance"]	= m_OpeningBalance.GetJson();

		Result["Payments"] = nlohmann::GetVector(m_Payments);

		return Result;
	}

	void MembershipFee::SetJson(nlohmann::basic_json<> _JS)
	{
		if (!_JS.is_object())
			return;

		m_Debt.SetJson(			 _JS["Debt"]);
		m_OpeningBalance.SetJson(_JS["OpeningBalance"]);

		nlohmann::SetVector(m_Payments, _JS, "Payments");
	}

	nlohmann::basic_json<> MembershipFee::GetAccrualsJson()
	{
		return nlohmann::GetVector(s_Accruals);
	}

	void MembershipFee::SetAccrualsJson(nlohmann::basic_json<> _JS, std::string_view _Name)
	{
		if (!_JS.is_object())
			return;

		nlohmann::SetVector(s_Accruals, _JS, _Name);

	}
}
