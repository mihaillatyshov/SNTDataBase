#include "Electricity.h"

#include <algorithm>

namespace LM
{

    void Electricity::Recalculate(bool hasBenefits)
    {
        Sort();
        m_All = m_OpeningBalance;
        for (int i = 1; i < m_ElectricityAccural.size(); i++)
        {
            //int cost = 6.57 * 1'254.789; // = 8'243.963'73
            // 657 * 1'254'789 = 824'396'373 //    / 1.000
            long long DayKWatt = m_ElectricityAccural[i].m_Day.m_Watt - m_ElectricityAccural[i - 1].m_Day.m_Watt;
            long long NightKWatt = m_ElectricityAccural[i].m_Night.m_Watt - m_ElectricityAccural[i - 1].m_Night.m_Watt;
            Money day = CalcMonthMoney(DayKWatt, m_ElectricityAccural[i].m_Costs.m_DayCost);
            Money night = CalcMonthMoney(NightKWatt, m_ElectricityAccural[i].m_Costs.m_NightCost);
            Money Losses = CalcLosses(day, night);
            std::cout << day.m_Amount / 100 << "." << day.m_Amount % 100 << std::endl;
            m_All += Losses;
            m_All += CalcWithBenefits(day, night, hasBenefits);
            for (auto& [name, amount] : m_ElectricityAccural[i].m_Costs.m_ConstantAccurals)
            {
                m_All += amount;
            }
        }
        for (auto& payment : m_ElectricityPayment)
        {
            m_All -= payment.m_Amount;
        }
    }


    void Electricity::Sort()
    {
        std::sort(m_ElectricityAccural.begin(), m_ElectricityAccural.end(), [](const ElectricityAccural &first, const ElectricityAccural &second)
            {
                //long long DateFirst, DateSecond;
                //char lable1[20] { 0 };
                //sprintf(lable1, "%d%02d%02d", first.m_Date.Year, first.m_Date.Month, first.m_Date.Day);
                //sscanf(lable1, "%lld", &DateFirst);
                ////std::cout << DateFirst << " ";
                //char lable2[20] { 0 };
                //sprintf(lable2, "%d%02d%02d", second.m_Date.Year, second.m_Date.Month, second.m_Date.Day);
                //sscanf(lable2, "%lld", &DateSecond);
                ////std::cout << DateSecond << std::endl;
                //return DateFirst < DateSecond;
                return first.m_Date < second.m_Date;
            });
    }


    void Electricity::SortPayments()
    {
        std::sort(m_ElectricityPayment.begin(), m_ElectricityPayment.end(), [](const Payment &first, const Payment &second)
            {
                //long long DateFirst, DateSecond;
                //char lable1[20] { 0 };
                //sprintf(lable1, "%d%02d%02d", first.GetDate().Year, first.GetDate().Month, first.GetDate().Day);
                //sscanf(lable1, "%lld", &DateFirst);
                ////std::cout << DateFirst << " ";
                //char lable2[20] { 0 };
                //sprintf(lable2, "%d%02d%02d", second.GetDate().Year, second.GetDate().Month, second.GetDate().Day);
                //sscanf(lable2, "%lld", &DateSecond);
                ////std::cout << DateSecond << std::endl;
                //return DateFirst > DateSecond;
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
        for (int i = 1; i < m_ElectricityAccural.size(); i++)
        {
            if (m_ElectricityAccural[i].m_Date > date)
                break;

            //int cost = 6.57 * 1'254.789; // = 8'243.963'73
            // 657 * 1'254'789 = 824'396'373 //    / 1.000
            long long DayKWatt = m_ElectricityAccural[i].m_Day.m_Watt - m_ElectricityAccural[i - 1].m_Day.m_Watt;
            long long NightKWatt = m_ElectricityAccural[i].m_Night.m_Watt - m_ElectricityAccural[i - 1].m_Night.m_Watt;
            Money day = CalcMonthMoney(DayKWatt, m_ElectricityAccural[i].m_Costs.m_DayCost);
            Money night = CalcMonthMoney(NightKWatt, m_ElectricityAccural[i].m_Costs.m_NightCost);
            Money Losses = CalcLosses(day, night);
            std::cout << day.m_Amount / 100 << "." << day.m_Amount % 100 << std::endl;
            res += Losses;
            res += CalcWithBenefits(day, night, hasBenefits);
            for (auto& [name, amount] : m_ElectricityAccural[i].m_Costs.m_ConstantAccurals)
            {
                res += amount;
                std::cout << "A: " << amount << std::endl;
            }
        }
        std::cout << "R1: " << res << std::endl;
        for (auto& payment : m_ElectricityPayment)
        {
            if (payment.m_Date > date)
                break;

            res -= payment.m_Amount;
            std::cout << "P: " << payment.m_Amount << std::endl;

        }

        std::cout << "R2: " << res << std::endl;
        return res;
    }


    bool kilowatt::DrawEdit(const std::string &fieldName)
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


}
