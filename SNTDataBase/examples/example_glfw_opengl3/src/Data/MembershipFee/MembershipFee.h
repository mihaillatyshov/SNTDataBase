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
        Money m_Debt;
        OpeningBalance m_OpeningBalance;

        std::vector<Payment> m_Payments;
        static inline std::vector<Accural> s_Accural;


        int IdToDetele = -1;

    public:
        MembershipFee()
        {
        }

        void SortPayments();

        nlohmann::basic_json<> GetJson() const; 
        void SetJson(nlohmann::basic_json<> js);
    };

}
