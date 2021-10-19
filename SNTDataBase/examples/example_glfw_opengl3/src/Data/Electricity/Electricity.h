#pragma once

#include <iostream>
#include <vector>
#include <nlohmann/json.hpp>

#include "ElectricityAccural.h"
#include "../Payment/Payment.h"


namespace LM
{

    class Electricity
    {
    public:
        Electricity() = default;

        void Recalculate(bool _HasBenefits);
        void SortPayments();

        static Money CalcMonthMoney(int64_t _Watt, const Money& _Cost);
        static Money CalcPercent(Money _Money, int _Percent);
        static Money CalcLosses(const Money& _Day, const Money& _Night);
        static Money CalcWithBenefits(const Money& _Day, const Money& _Night, bool _HasBenefits);
        Money CalcAccuralsToDate(const Date& _Date, bool _HasBenefits); // Include this Date

        nlohmann::basic_json<> GetJson() const;
        void SetJson(nlohmann::basic_json<> _JS);
    protected:
        void Sort();
    public:
        Money                           m_All;
        Money                           m_OpeningBalance;
        std::vector<Payment>            m_Payments;
        std::vector<ElectricityAccural> m_Accurals;
    };
}
