#pragma once

#include <iostream>
#include <vector>

#include <nlohmann/json.hpp>

#include "Core/Base.h"
#include "Accrual/ElectricityAccrual.h"
#include "Data/Payment/Payment.h"
#include "Data/Homestead/Privilege/Privilege.h"

namespace LM
{

	class Electricity
	{
	public:
		Electricity() = default;

		void Recalculate(const Privilege& _Privilege);
		void SortAccruals();
		void SortPayments();
		void Sort();

		static Money CalcLosses(const Money& _Day, const Money& _Night);
		static Money CalcWithBenefits(const Money& _Day, const Money& _Night, bool _HasBenefits);
		Money CalcAccrualsToDate(const Date& _Date, const Privilege& _Privilege) const; // Include this Date

		const Money& CalcAccrual(size_t _AccId, const Privilege& _Privilege) const;
		void GetAccrualCsv(size_t _AccId, const Privilege& _Privilege, Money* _Day, Money* _Night, Money* _Losses) const;

		inline const Ref<const ElectricityAccrual> GetAccrual(size_t _Id) const { return m_Accruals[_Id]; }
		size_t GetAccrualsCount() const { return m_Accruals.size(); }
		void AddAccrual(Ref<const TabDataStruct> _TabDS);
		void AddAccrual(const ElectricityAccrual& _Accrual);
		void EditAccrual(size_t _AccId, Ref<const TabDataStruct> _TabDS);
		void DeleteAccrual(size_t _AccId);

		void AddPayment(Ref<const TabDataStruct> _TabDS);
		void EditPayment(size_t _PayId, Ref<const TabDataStruct> _TabDS);
		void DeletePayment(size_t _PayId);

		inline const Ref<const Payment> GetPayment(size_t _Id) const { return m_Payments[_Id]; }
		size_t GetPaymentsCount() const { return m_Payments.size(); }

		const Money& GetOpeningBalance() const { return m_OpeningBalance; }
		inline void SetOpeningBalance(const Money& _Money) { m_OpeningBalance = _Money; }

		inline const Money& GetAll() const { return m_All; }

		nlohmann::basic_json<> GetJson() const;
		void SetJson(nlohmann::basic_json<> _JS);
	protected:
		static inline Percent BenefitPecent = Percent(70);
		static inline Percent LossesPecent  = Percent(3);
	protected:
		Money							m_All;
		Money							m_OpeningBalance;
		VectorRef<Payment>				m_Payments;
		VectorRef<ElectricityAccrual>	m_Accruals;
	};
}
