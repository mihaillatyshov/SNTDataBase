#pragma once

#include <vector>
#include "../Payment/Payment.h"
#include "OpeningBalance.h"
#include "../Accural/Accural.h"
#include <nlohmann/json.hpp>

namespace LM
{

	class MembershipFee
	{
	public:
		MembershipFee() = default;

		inline const Money& GetDebt() const { return m_Debt; }
		inline const std::shared_ptr<const Payment> GetPayment(size_t _Id) const { return m_Payments[_Id]; }
		inline size_t GetPaymentsCount() const { return m_Payments.size(); }

		void SortPayments();

		nlohmann::basic_json<> GetJson() const;
		void SetJson(nlohmann::basic_json<> js);
	public:
		Money m_Debt;
		OpeningBalance m_OpeningBalance;

		std::vector<std::shared_ptr<Payment>> m_Payments;
		static inline std::vector<Accural> s_Accural;
	};

}
