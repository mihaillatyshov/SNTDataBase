#pragma once

#include <iostream>
#include <vector>
#include <nlohmann/json.hpp>

#include "Accrual/ElectricityAccrual.h"
#include "Data/Payment/Payment.h"
#include "Core/Base.h"

namespace LM
{

	class Electricity
	{
	public:
		Electricity() = default;

		void Recalculate(bool _HasBenefits);
		void SortAccruals();
		void SortPayments();
		void Sort();

		static Money CalcMonthMoney(int64_t _Watt, const Money& _Cost);
		static Money CalcPercent(Money _Money, int _Percent);
		static Money CalcLosses(const Money& _Day, const Money& _Night);
		static Money CalcWithBenefits(const Money& _Day, const Money& _Night, bool _HasBenefits);
		Money CalcAccrualsToDate(const Date& _Date, bool _HasBenefits); // Include this Date

		inline const Ref<const ElectricityAccrual> GetAccrual(size_t _Id) const { return m_Accruals[_Id]; }
		size_t GetAccrualsCount() const { return m_Accruals.size(); }
		void AddAccrual(Ref<const TabDataStruct> _TabDS);
		void EditAccrual(size_t _AccId, Ref<const TabDataStruct> _TabDS);
		void DeleteAccrual(size_t _AccId);

		void AddPayment(Ref<const TabDataStruct> _TabDS);
		void EditPayment(size_t _PayId, Ref<const TabDataStruct> _TabDS);
		void DeletePayment(size_t _PayId);

		inline const Ref<const Payment> GetPayment(size_t _Id) const { return m_Payments[_Id]; }
		size_t GetPaymentsCount() const { return m_Payments.size(); }

		inline const Money& GetAll() const { return m_All; }

		nlohmann::basic_json<> GetJson() const;
		void SetJson(nlohmann::basic_json<> _JS);
	public:
		Money							m_All;
		Money							m_OpeningBalance;
		VectorRef<Payment>				m_Payments;
		VectorRef<ElectricityAccrual>	m_Accruals;
	};
}
