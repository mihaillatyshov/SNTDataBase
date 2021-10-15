#include "Electricity.h"

#include <algorithm>
#include <nlohmann/JsonUtils.h>

namespace LM
{

    ////////////////////////////////////////////////////////////////////////////////
    // kilowatt ////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////

    bool kilowatt::DrawEdit(const std::string& fieldName)
    {
        bool isEdit = false;
        char label[100];
        sprintf(label, "%lld.%03lld", m_Watt / 1000, abs(m_Watt % 1000));
        ImGui::PushItemWidth(250);
        if (ImGui::InputText(fieldName.c_str(), label, 100, ImGuiInputTextFlags_CharsDecimal))
        {
            long long Kwatt = 0, Watt = 0, Watt1 = 0, Watt2 = 0, Watt3 = 0;
            sscanf(label, "%lld.%1lld%1lld%1lld", &Kwatt, &Watt1, &Watt2, &Watt3);
            Watt = 100 * Watt1 + 10 * Watt2 + Watt3;

            m_Watt = abs(Kwatt * 1000) + abs(Watt);
            if (Kwatt < 0)
                m_Watt = -m_Watt;
            isEdit = true;
        }
        ImGui::PopItemWidth();

        return isEdit;
    }

    nlohmann::basic_json<> kilowatt::GetJson() const
    {
        nlohmann::basic_json<> result;
        result["Watt"] = m_Watt;

        return result;
    }

    void kilowatt::SetJson(nlohmann::basic_json<> js)
    {
        nlohmann::SetValue(m_Watt, js, "Watt");
    }

    std::ostream& operator<< (std::ostream& out, const kilowatt kw)
    {
        out << kw.m_Watt / 1000 << "." << kw.m_Watt % 1000;
        return out;
    }

    const kilowatt operator+(const kilowatt& left, const kilowatt& right)
    {
        kilowatt res;
        res.m_Watt = left.m_Watt + right.m_Watt;
        return res;
    }

    const kilowatt operator-(const kilowatt& left, const kilowatt& right)
    {
        kilowatt res;
        res.m_Watt = left.m_Watt - right.m_Watt;
        return res;
    }


    ////////////////////////////////////////////////////////////////////////////////
    // OtherElectricityAccuralCost /////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////

    nlohmann::basic_json<> OtherElectricityAccuralCost::GetJson() const
    {
        nlohmann::basic_json<> result;
        result["Name"] = m_Name;
        result["Money"] = m_Money.GetJson();

        return result;
    }

    void OtherElectricityAccuralCost::SetJson(nlohmann::basic_json<> js)
    {
        nlohmann::SetValue(m_Name, js, "Name");
        m_Money.SetJson(js["Money"]);
    }


    ////////////////////////////////////////////////////////////////////////////////
    // ElectricityAccuralCosts /////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////

    nlohmann::basic_json<> ElectricityAccuralCosts::GetJson() const
    {
        nlohmann::basic_json<> result;
        result["Day"]   = m_Day.GetJson();
        result["Night"] = m_Night.GetJson();

        result["Others"] = nlohmann::GetVector(m_Others);

        return result;
    }

    void ElectricityAccuralCosts::SetJson(nlohmann::basic_json<> js)
    {
        if (!js.is_object())
            return;

        m_Day.SetJson(  js["Day"]);
        m_Night.SetJson(js["Night"]);

        nlohmann::SetVector(m_Others, js, "Others");
    }


    ////////////////////////////////////////////////////////////////////////////////
    // ElectricityAccural //////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////

    nlohmann::basic_json<> ElectricityAccural::GetJson() const
    {
        nlohmann::basic_json<> result;
        result["Date"]  = m_Date.GetJson();
        result["Day"]   = m_Day.GetJson();
        result["Night"] = m_Night.GetJson();
        result["Costs"] = m_Costs.GetJson();

        return result;
    }

    void ElectricityAccural::SetJson(nlohmann::basic_json<> js)
    {
        if (!js.is_object())
            return;

        m_Date.SetJson( js["Date"]);
        m_Day.SetJson(  js["Day"]);
        m_Night.SetJson(js["Night"]);
        m_Costs.SetJson(js["Costs"]);
    }


    ////////////////////////////////////////////////////////////////////////////////
    // Electricity /////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////

    void Electricity::Recalculate(bool hasBenefits)
    {
        Sort();
        m_All = m_OpeningBalance;
        for (int i = 1; i < m_Accurals.size(); i++)
        {
            //int cost = 6.57 * 1'254.789; // = 8'243.963'73
            // 657 * 1'254'789 = 824'396'373 //    / 1.000
            long long DayKWatt = m_Accurals[i].m_Day.m_Watt - m_Accurals[i - 1].m_Day.m_Watt;
            long long NightKWatt = m_Accurals[i].m_Night.m_Watt - m_Accurals[i - 1].m_Night.m_Watt;
            Money day = CalcMonthMoney(DayKWatt, m_Accurals[i].m_Costs.m_Day);
            Money night = CalcMonthMoney(NightKWatt, m_Accurals[i].m_Costs.m_Night);
            Money Losses = CalcLosses(day, night);
            std::cout << day.m_Amount / 100 << "." << day.m_Amount % 100 << std::endl;
            m_All += Losses;
            m_All += CalcWithBenefits(day, night, hasBenefits);
            for (auto& [name, amount] : m_Accurals[i].m_Costs.m_Others)
            {
                m_All += amount;
            }
        }
        for (auto& payment : m_Payments)
        {
            m_All -= payment.m_Amount;
        }
    }

    void Electricity::Sort()
    {
        std::sort(m_Accurals.begin(), m_Accurals.end(), [](const ElectricityAccural &first, const ElectricityAccural &second)
            {
                return first.m_Date < second.m_Date;
            });
    }

    void Electricity::SortPayments()
    {
        std::sort(m_Payments.begin(), m_Payments.end(), [](const Payment &first, const Payment &second)
            {
                return first.m_Date > second.m_Date;
            });
    }

    Money Electricity::CalcMonthMoney(long long KWatt, const Money& Cost)
    {
        // TODO: Formula here
        long long MonthCost = KWatt * Cost.m_Amount;
        long long Rub = MonthCost / 100'000;
        long long Cop = (MonthCost / 1'000) % 100;
        Cop += (MonthCost % 1'000) / 100 >= 5 ? 1 : 0;
        return Money(Rub, Cop);
    }

    Money CalcPercent(Money money, int percent)
    {
        money.m_Amount *= percent;
        money.m_Amount = money.m_Amount / 100 + ((money.m_Amount % 100) / 10 >= 5 ? 1 : 0);
        return money;
    }

    Money Electricity::CalcLosses(const Money& day, const Money& night)
    {
        // 101.21       * 3
        // 303.63       / 100
        //   3.04

        Money AllMonth = day + night;
        std::cout << AllMonth.m_Amount << " ";
        //AllMonth.m_Amount *= 3;
        //AllMonth.m_Amount = AllMonth.m_Amount / 100 + ((AllMonth.m_Amount % 100) / 10 >= 5 ? 1 : 0);
        AllMonth = CalcPercent(AllMonth, 3);
        std::cout << AllMonth.m_Amount / 100 << "." << AllMonth.m_Amount % 100 << std::endl;
        return AllMonth;
    }

    Money Electricity::CalcWithBenefits(const Money& day, const Money& night, bool hasBenefits)
    {
        //Money AllMonth = day + night;
        Money dayMoney = day;
        Money nightMoney = night;
        if (hasBenefits)
        {
            dayMoney = CalcPercent(dayMoney, 70);
            nightMoney = CalcPercent(nightMoney, 70);
            //AllMonth.m_Amount *= 70;
            //AllMonth.m_Amount = dayMoney.m_Amount / 100 + ((dayMoney.m_Amount % 100) / 10 >= 5 ? 1 : 0);
        }
        Money AllMonth = dayMoney + nightMoney;
        std::cout << AllMonth.m_Amount / 100 << "." << AllMonth.m_Amount % 100 << std::endl;
        return AllMonth;
    
    }

    Money Electricity::CalcAccuralsToDate(const Date& date, bool hasBenefits)
    {
        Money res = m_OpeningBalance;
        for (int i = 1; i < m_Accurals.size(); i++)
        {
            if (m_Accurals[i].m_Date > date)
                break;

            //int cost = 6.57 * 1'254.789; // = 8'243.963'73
            // 657 * 1'254'789 = 824'396'373 //    / 1.000
            long long DayKWatt = m_Accurals[i].m_Day.m_Watt - m_Accurals[i - 1].m_Day.m_Watt;
            long long NightKWatt = m_Accurals[i].m_Night.m_Watt - m_Accurals[i - 1].m_Night.m_Watt;
            Money day = CalcMonthMoney(DayKWatt, m_Accurals[i].m_Costs.m_Day);
            Money night = CalcMonthMoney(NightKWatt, m_Accurals[i].m_Costs.m_Night);
            Money Losses = CalcLosses(day, night);
            std::cout << day.m_Amount / 100 << "." << day.m_Amount % 100 << std::endl;
            res += Losses;
            res += CalcWithBenefits(day, night, hasBenefits);
            for (auto& [name, amount] : m_Accurals[i].m_Costs.m_Others)
            {
                res += amount;
                std::cout << "A: " << amount << std::endl;
            }
        }
        std::cout << "R1: " << res << std::endl;
        for (auto& payment : m_Payments)
        {
            if (payment.m_Date > date)
                break;

            res -= payment.m_Amount;
            std::cout << "P: " << payment.m_Amount << std::endl;

        }

        std::cout << "R2: " << res << std::endl;
        return res;
    }

    nlohmann::basic_json<> Electricity::GetJson() const
    {
        nlohmann::basic_json<> result;
        result["All"]               = m_All.GetJson();
        result["OpeningBalance"]    = m_OpeningBalance.GetJson();

        result["Payments"] = nlohmann::GetVector(m_Payments);
        result["Accurals"] = nlohmann::GetVector(m_Accurals);

        return result;
    }

    void Electricity::SetJson(nlohmann::basic_json<> js)
    {
        if (!js.is_object())
            return;

        m_All.SetJson(           js["All"]);
        m_OpeningBalance.SetJson(js["OpeningBalance"]);

        nlohmann::SetVector(m_Payments, js, "Payments");
        nlohmann::SetVector(m_Accurals, js, "Accurals");
    }

}
