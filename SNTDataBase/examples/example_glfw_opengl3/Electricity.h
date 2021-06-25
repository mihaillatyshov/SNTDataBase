#pragma once

#include <iostream>
#include <vector>
#include "Payment.h"

namespace LM
{

    struct kilowatt
    {
        long long m_Watt = 0;
        kilowatt(long long kilowatts = 0, long long watts = 0)
        {
            m_Watt = kilowatts * 1000 + watts;
        }

        bool DrawEdit(const std::string &fieldName);

        
        friend const kilowatt operator+(const kilowatt& left, const kilowatt& right);
        friend const kilowatt operator-(const kilowatt& left, const kilowatt& right);

        friend std::ostream& operator<< (std::ostream& out, const kilowatt kw);
    };


    struct ElectricityAccuralCosts
    {
        ElectricityAccuralCosts(const Money& day, const Money& night, const std::vector<std::pair<std::string, Money>> constants)
            : m_DayCost(day), m_NightCost(night), m_ConstantAccurals(constants) { }

        ElectricityAccuralCosts() = default;

        Money m_DayCost = Money(6, 57);
        Money m_NightCost = Money(4, 13);

        // Sum of standart constants = 141
        std::vector<std::pair<std::string, Money>> m_ConstantAccurals
        {
            { u8"Обслуживающая организация", Money(60) },
            { u8"Сервер", Money(25) },
            { u8"Освещение и шлагбаум", Money(15) },
            { u8"8кВт", Money(40, 50) },
        };
    };


    class ElectricityAccural
    {
    public:
        Date m_Date;
        kilowatt m_Day;
        kilowatt m_Night;
        kilowatt m_AllMonth;
        ElectricityAccuralCosts m_Costs;
    };



    class Electricity
    {
    public:
        Electricity() = default;

        void Recalculate(bool HasBenefits);
        void SortPayments();
    protected:
        void Sort();
    public:
        static Money CalcMonthMoney(long long KWatt, const Money& Cost);
        static Money CalcLosses(const Money& day, const Money& night);
        static Money CalcWithBenefits(const Money& day, const Money& night, bool hasBenefits);
        Money CalcAccuralsToDate(const Date& date, bool HasBenefits); // Include this Date
    public:
        Money m_All;
        Money m_OpeningBalance;
        std::vector<Payment> m_ElectricityPayment;
        std::vector<ElectricityAccural> m_ElectricityAccural;
    };
}
