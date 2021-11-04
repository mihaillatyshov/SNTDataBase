#pragma once

#include <vector>
#include "../Payment/Payment.h"
#include "OpeningBalance.h"
#include "../Accrual/Accrual.h"
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

		void SortPayments();

		nlohmann::basic_json<> GetJson() const;
		void SetJson(nlohmann::basic_json<> js);
	public:
		Money m_Debt;
		OpeningBalance m_OpeningBalance;

		VectorRef<Payment> m_Payments;
		static inline std::vector<Accrual> s_Accrual;
	};

}
