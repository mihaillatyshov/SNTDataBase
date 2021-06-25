#pragma once

#include <vector>
#include "Payment.h"
#include "OpeningBalance.h"
#include "Accural.h"

namespace LM
{

    class MembershipFee
    {
    public:
        Money m_Debt;
        OpeningBalance m_OpeningBalance;

        std::vector<Payment> m_Payments;
        static inline std::vector<Accural> s_Accural;


        int IdToDetele = -1;

    public:
        MembershipFee()
        {
        }

        void DrawMembershipFee(bool IsEdit);

        void SortPayments();

        void DrawActivePayment(Payment &payment, int id, bool isEdit);
        void DrawInactivePayment(Payment &payment, int id);
        void DrawDeletePayment();
    };

}
