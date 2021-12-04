#include "Electricity.h"

#include <algorithm>

#include "Utils/JsonUtils.h"

namespace LM
{

	void Electricity::Recalculate(const Privilege& _Privilege)
	{
		Sort();
		m_All = m_OpeningBalance;
		for (size_t i = 1; i < m_Accruals.size(); i++)
		{
			//int cost = 6.57 * 1'254.789; // = 8'243.963'73
			// 657 * 1'254'789 = 824'396'373 //    / 1.000
			m_All += CalcAccrual(i, _Privilege);
		}
		for (auto& Pay : m_Payments)
		{
			m_All -= Pay->GetAmount();
		}
	}

	void Electricity::SortAccruals()
	{
		std::sort(m_Accruals.begin(), m_Accruals.end(), [](const Ref<const ElectricityAccrual> First, const Ref<const ElectricityAccrual> Second)
			{
				return First->GetDate() < Second->GetDate();
			});
	}

	void Electricity::SortPayments()
	{
		std::sort(m_Payments.begin(), m_Payments.end(), [](const Ref<const Payment> First, const Ref<const Payment> Second)
			{
				return First->GetDate() > Second->GetDate();
			});
	}

	void Electricity::Sort() 
	{
		SortAccruals(); 
		SortPayments(); 
	}

	Money Electricity::CalcLosses(const Money& _Day, const Money& _Night)
	{
		// 101.21       * 3
		// 303.63       / 100
		//   3.04
		return (_Day + _Night) * Percent(3);
	}

	Money Electricity::CalcWithBenefits(const Money& _Day, const Money& _Night, bool _HasBenefits)
	{
		Percent Pct(70);
		return _HasBenefits ? (_Day * Pct + _Night * Pct) : (_Day + _Night);
	}

	Money Electricity::CalcAccrualsToDate(const Date& _Date, const Privilege& _Privilege)
	{
		Money Res = m_OpeningBalance;
		for (size_t i = 1; i < m_Accruals.size(); i++)
		{
			if (m_Accruals[i]->GetDate() > _Date)
				break;
			Res += CalcAccrual(i, _Privilege);
		}
		for (const auto& Pay : m_Payments)
		{
			if (Pay->GetDate() > _Date)
				break;

			Res -= Pay->GetAmount();
		}

		return Res;
	}

	const Money& Electricity::CalcAccrual(size_t _AccId, const Privilege& _Privilege)
	{
		Money Res(0);

		Money Day	= m_Accruals[_AccId]->GetCosts().GetDay()	* (m_Accruals[_AccId]->GetDay()		- m_Accruals[_AccId - 1]->GetDay());
		Money Night = m_Accruals[_AccId]->GetCosts().GetNight() * (m_Accruals[_AccId]->GetNight()	- m_Accruals[_AccId - 1]->GetNight());
		Res += CalcWithBenefits(Day, Night, _Privilege.GetHasPrivilege(m_Accruals[_AccId]->GetDate()));
		Res += CalcLosses(Day, Night);
		Res += m_Accruals[_AccId]->GetCosts().GetOthersSum();

		return Res;
	}

	void Electricity::AddAccrual(Ref<const TabDataStruct> _TabDS)
	{
		m_Accruals.push_back(CreateRef<ElectricityAccrual>(_TabDS));
	}

	void Electricity::AddAccrual(const ElectricityAccrual& _Accrual)
	{
		m_Accruals.push_back(CreateRef<ElectricityAccrual>(_Accrual));
	}

	void Electricity::EditAccrual(size_t _AccId, Ref<const TabDataStruct> _TabDS)
	{
		m_Accruals[_AccId]->Edit(_TabDS);
	}

	void Electricity::DeleteAccrual(size_t _AccId)
	{
		m_Accruals.erase(m_Accruals.begin() + _AccId);
	}

	void Electricity::AddPayment(Ref<const TabDataStruct> _TabDS)
	{
		m_Payments.push_back(CreateRef<Payment>(_TabDS));
	}

	void Electricity::EditPayment(size_t _PayId, Ref<const TabDataStruct> _TabDS)
	{
		m_Payments[_PayId]->Edit(_TabDS);
	}

	void Electricity::DeletePayment(size_t _PayId)
	{
		m_Payments.erase(m_Payments.begin() + _PayId);
	}

	nlohmann::basic_json<> Electricity::GetJson() const
	{
		nlohmann::basic_json<> Result;
		Result["All"] = m_All.GetJson();
		Result["OpeningBalance"] = m_OpeningBalance.GetJson();

		Result["Payments"] = nlohmann::GetVector(m_Payments);
		Result["Accruals"] = nlohmann::GetVector(m_Accruals);

		return Result;
	}

	void Electricity::SetJson(nlohmann::basic_json<> _JS)
	{
		if (!_JS.is_object())
			return;

		m_All.SetJson(_JS["All"]);
		m_OpeningBalance.SetJson(_JS["OpeningBalance"]);

		nlohmann::SetVector(m_Payments, _JS, "Payments");
		nlohmann::SetVector(m_Accruals, _JS, "Accruals");
	}

}
