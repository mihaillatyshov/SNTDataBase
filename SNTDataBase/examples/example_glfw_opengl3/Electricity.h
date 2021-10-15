#pragma once

#include <iostream>
#include <vector>
#include "Payment.h"

namespace LM
{

    ////////////////////////////////////////////////////////////////////////////////
    // kilowatt ////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////

    struct kilowatt
    {
    public:
        kilowatt(long long kilowatts = 0, long long watts = 0)
        {
            m_Watt = kilowatts * 1000 + watts;
        }

        bool DrawEdit(const std::string &fieldName);

        nlohmann::basic_json<> GetJson() const;
        void SetJson(nlohmann::basic_json<> js);

    public:
        friend const kilowatt operator+(const kilowatt& left, const kilowatt& right);
        friend const kilowatt operator-(const kilowatt& left, const kilowatt& right);
        friend std::ostream& operator<< (std::ostream& out, const kilowatt kw);

    public:
        long long m_Watt = 0;
    };


    ////////////////////////////////////////////////////////////////////////////////
    // OtherElectricityAccuralCost /////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////

    class OtherElectricityAccuralCost
    {
    public:
        OtherElectricityAccuralCost() = default;
        OtherElectricityAccuralCost(std::string_view _Name, const Money& _Money)
            : m_Name(_Name), m_Money(_Money) { }

        nlohmann::basic_json<> GetJson() const;
        void SetJson(nlohmann::basic_json<> js);

    public:
        std::string m_Name = "<NONE>";
        Money m_Money;
    };


    ////////////////////////////////////////////////////////////////////////////////
    // ElectricityAccuralCosts /////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////

    class ElectricityAccuralCosts
    {
    public:
        ElectricityAccuralCosts() = default;
        ElectricityAccuralCosts(const Money& day, const Money& night, const std::vector<OtherElectricityAccuralCost> constants)
            : m_Day(day), m_Night(night), m_Others(constants) { }

        nlohmann::basic_json<> GetJson() const;
        void SetJson(nlohmann::basic_json<> js);

    public:
        Money m_Day   = Money(6, 57);
        Money m_Night = Money(4, 13);

        // Sum of standart constants = 141
        std::vector<OtherElectricityAccuralCost> m_Others
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
        nlohmann::basic_json<> GetJson() const;
        void SetJson(nlohmann::basic_json<> js);
        inline kilowatt GetAllMonth() const { return m_Day + m_Night; }
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

        static Money CalcMonthMoney(long long KWatt, const Money& Cost);
        static Money CalcLosses(const Money& day, const Money& night);
        static Money CalcWithBenefits(const Money& day, const Money& night, bool hasBenefits);
        Money CalcAccuralsToDate(const Date& date, bool HasBenefits); // Include this Date

        nlohmann::basic_json<> GetJson() const;
        void SetJson(nlohmann::basic_json<> js);
    protected:
        void Sort();
    public:
        Money m_All;
        Money m_OpeningBalance;
        std::vector<Payment> m_Payments;
        std::vector<ElectricityAccural> m_Accurals;
    };
}
