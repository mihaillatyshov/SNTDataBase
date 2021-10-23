#include "Electricity.h"

#include <algorithm>
#include <nlohmann/JsonUtils.h>

namespace LM
{

    void Electricity::Recalculate(bool _HasBenefits)
    {
        Sort();
        m_All = m_OpeningBalance;
        for (int i = 1; i < m_Accurals.size(); i++)
        {
            //int cost = 6.57 * 1'254.789; // = 8'243.963'73
            // 657 * 1'254'789 = 824'396'373 //    / 1.000
            int64_t DayWatt = m_Accurals[i].m_Day.m_Watt - m_Accurals[i - 1].m_Day.m_Watt;
            int64_t NightWatt = m_Accurals[i].m_Night.m_Watt - m_Accurals[i - 1].m_Night.m_Watt;
            Money Day = CalcMonthMoney(DayWatt, m_Accurals[i].m_Costs.m_Day);
            Money Night = CalcMonthMoney(NightWatt, m_Accurals[i].m_Costs.m_Night);
            Money Losses = CalcLosses(Day, Night);
            std::cout << Day << std::endl;
            m_All += Losses;
            m_All += CalcWithBenefits(Day, Night, _HasBenefits);
            for (auto& [Name, Amount] : m_Accurals[i].m_Costs.m_Others)
            {
                m_All += Amount;
            }
        }
        for (auto& Pay: m_Payments)
        {
            m_All -= Pay.m_Amount;
        }
    }

    void Electricity::Sort()
    {
        std::sort(m_Accurals.begin(), m_Accurals.end(), [](const ElectricityAccural &First, const ElectricityAccural &Second)
            {
                return First.m_Date < Second.m_Date;
            });
    }

    void Electricity::SortPayments()
    {
        std::sort(m_Payments.begin(), m_Payments.end(), [](const Payment &First, const Payment &Second)
            {
                return First.m_Date > Second.m_Date;
            });
    }

    Money Electricity::CalcMonthMoney(int64_t _Watt, const Money& _Cost)
    {
        // TODO: Formula here
        int64_t MonthCost = _Watt * _Cost.m_Amount;
        int64_t Rub = MonthCost / 100'000;
        int64_t Cop = (MonthCost / 1'000) % 100;
        Cop += (MonthCost % 1'000) / 100 >= 5 ? 1 : 0;
        return Money(Rub, Cop);
    }

    Money Electricity::CalcPercent(Money _Money, int _Percent)
    {
        _Money.m_Amount *= _Percent;
        _Money.m_Amount = _Money.m_Amount / 100 + ((_Money.m_Amount % 100) / 10 >= 5 ? 1 : 0);
        return _Money;
    }

    Money Electricity::CalcLosses(const Money& _Day, const Money& _Night)
    {
        // 101.21       * 3
        // 303.63       / 100
        //   3.04

        Money AllMonth = _Day + _Night;
        std::cout << AllMonth.m_Amount << " ";
        //AllMonth.m_Amount *= 3;
        //AllMonth.m_Amount = AllMonth.m_Amount / 100 + ((AllMonth.m_Amount % 100) / 10 >= 5 ? 1 : 0);
        AllMonth = CalcPercent(AllMonth, 3);
        std::cout << AllMonth.m_Amount / 100 << "." << AllMonth.m_Amount % 100 << std::endl;
        return AllMonth;
    }

    Money Electricity::CalcWithBenefits(const Money& _Day, const Money& _Night, bool _HasBenefits)
    {
        //Money AllMonth = day + night;
        Money DayMoney = _Day;
        Money NightMoney = _Night;
        if (_HasBenefits)
        {
            DayMoney = CalcPercent(DayMoney, 70);
            NightMoney = CalcPercent(NightMoney, 70);
        }
        Money AllMonth = DayMoney + NightMoney;
        std::cout << AllMonth.m_Amount / 100 << "." << AllMonth.m_Amount % 100 << std::endl;
        return AllMonth;
    
    }

    Money Electricity::CalcAccuralsToDate(const Date& _Date, bool _HasBenefits)
    {
        Money Res = m_OpeningBalance;
        for (int i = 1; i < m_Accurals.size(); i++)
        {
            if (m_Accurals[i].m_Date > _Date)
                break;

            //int cost = 6.57 * 1'254.789; // = 8'243.963'73
            // 657 * 1'254'789 = 824'396'373 //    / 1.000
            int64_t DayKWatt = m_Accurals[i].m_Day.m_Watt - m_Accurals[i - 1].m_Day.m_Watt;
            int64_t NightKWatt = m_Accurals[i].m_Night.m_Watt - m_Accurals[i - 1].m_Night.m_Watt;
            Money Day = CalcMonthMoney(DayKWatt, m_Accurals[i].m_Costs.m_Day);
            Money Night = CalcMonthMoney(NightKWatt, m_Accurals[i].m_Costs.m_Night);
            Money Losses = CalcLosses(Day, Night);
            std::cout << Day << std::endl;
            Res += Losses;
            Res += CalcWithBenefits(Day, Night, _HasBenefits);
            for (auto& [Name, Amount] : m_Accurals[i].m_Costs.m_Others)
            {
                Res += Amount;
                std::cout << "A: " << Amount << std::endl;
            }
        }
        std::cout << "R1: " << Res << std::endl;
        for (auto& Pay: m_Payments)
        {
            if (Pay.m_Date > _Date)
                break;

            Res -= Pay.m_Amount;
            std::cout << "P: " << Pay.m_Amount << std::endl;

        }

        std::cout << "R2: " << Res << std::endl;
        return Res;
    }

    nlohmann::basic_json<> Electricity::GetJson() const
    {
        nlohmann::basic_json<> Result;
        Result["All"]               = m_All.GetJson();
        Result["OpeningBalance"]    = m_OpeningBalance.GetJson();

        Result["Payments"] = nlohmann::GetVector(m_Payments);
        Result["Accurals"] = nlohmann::GetVector(m_Accurals);

        return Result;
    }

    void Electricity::SetJson(nlohmann::basic_json<> _JS)
    {
        if (!_JS.is_object())
            return;

        m_All.SetJson(           _JS["All"]);
        m_OpeningBalance.SetJson(_JS["OpeningBalance"]);

        nlohmann::SetVector(m_Payments, _JS, "Payments");
        nlohmann::SetVector(m_Accurals, _JS, "Accurals");
    }

}
