#pragma once

#include <vector>
#include "../Payment/Payment.h"
#include "OpeningBalance.h"
#include "MembershipFeeAccrual/MembershipFeeAccrual.h"
#include <nlohmann/json.hpp>
#include "Core/Base.h"

namespace LM
{

	class MembershipFee
	{
	public:
		MembershipFee() = default;

		inline const Money& GetDebt() const { return m_Debt; }

		inline const Ref<const Payment> GetPayment(size_t _Id) const { return m_Payments[_Id]; }
		inline size_t GetPaymentsCount() const { return m_Payments.size(); }
		void AddPayment(Ref<const TabDataStruct> _TabDS);
		void EditPayment(size_t _PayId, Ref<const TabDataStruct> _TabDS);
		void DeletePayment(size_t _PayId);
		void SortPayments();

		//void Recalculate();

		nlohmann::basic_json<> GetJson() const;
		void SetJson(nlohmann::basic_json<> js);
	public:
		static inline std::vector<MembershipFeeAccrual> s_Accrual;
	public:
		Money m_Debt;
		OpeningBalance m_OpeningBalance;

		VectorRef<Payment> m_Payments;
	};

}
