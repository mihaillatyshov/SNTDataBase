#pragma once

#include <vector>

#include <nlohmann/json.hpp>

#include "Core/Base.h"
#include "OpeningBalance.h"
#include "Accrual/MembershipFeeAccrual.h"
#include "Data/Payment/Payment.h"
#include "Data//Homestead/Privilege/Privilege.h"

namespace LM
{

	class MembershipFee
	{
	public:
		MembershipFee() = default;

		inline const Money& GetDebt() const { return m_Debt; }
		
		inline const Money& GetOpeningBalance() const { return m_OpeningBalance.GetMoney(); }
		inline void SetOpeningBalance(const Money& _Money) { m_OpeningBalance.SetMoney(_Money); }

		inline const Ref<const Payment> GetPayment(size_t _Id) const { return m_Payments[_Id]; }
		inline size_t GetPaymentsCount() const { return m_Payments.size(); }
		void AddPayment(Ref<const TabDataStruct> _TabDS);
		void EditPayment(size_t _PayId, Ref<const TabDataStruct> _TabDS);
		void DeletePayment(size_t _PayId);
		void SortPayments();

		static void SortAccruals();
		static void AddAccrual(const MembershipFeeAccrual& _Accrual);
		static void DeleteAccrual(Date _Date);
		static void GetAccrualsToAdd(std::vector<MembershipFeeAccrual>& _AccrualsToAdd);
		static void GetAccrualsToDelete(std::vector<Date>& _AccrualsToDelete);

		void Recalculate(const Privilege& _Privilege);

		nlohmann::basic_json<> GetJson() const;
		void SetJson(nlohmann::basic_json<> _JS);

		static nlohmann::basic_json<> GetAccrualsJson();
		static void SetAccrualsJson(nlohmann::basic_json<> _JS, std::string_view _Name);
	protected:
		Money m_Debt;
		OpeningBalance m_OpeningBalance;

		VectorRef<Payment> m_Payments;

		static inline std::vector<MembershipFeeAccrual> s_Accruals;
	};

}
