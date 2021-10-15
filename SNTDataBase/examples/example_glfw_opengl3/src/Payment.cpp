#include "Payment.h"

#include <nlohmann/JsonUtils.h>

namespace LM
{

    ////////////////////////////////////////////////////////////////////////////////
    // Date ////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////

    bool Date::DrawEdit()
    {
        bool IsChange = false;
        ImGui::PushItemWidth(100);
        ImGui::Text(u8"Год"); ImGui::SameLine();
        if (ImGui::InputInt(u8"##Год", &m_Year))
        {
            FixDate();
            IsChange = true;
        }
        ImGui::SameLine();
        ImGui::PopItemWidth();

        ImGui::PushItemWidth(80);
        ImGui::Text(u8"Месяц"); ImGui::SameLine();
        if (ImGui::InputInt(u8"##Месяц", &m_Month))
        {
            FixDate();
            IsChange = true;
        }
        ImGui::SameLine();

        ImGui::Text(u8"День"); ImGui::SameLine();
        if (ImGui::InputInt(u8"##День", &m_Day))
        {
            FixDate();
            IsChange = true;
        }
        ImGui::PopItemWidth();

        return IsChange;

    }

    const std::string Date::GetString() const
    {
        char date[20]{ 0 };
        sprintf(date, u8"%02d.%02d.%d", m_Day, m_Month, m_Year);
        return date;
    }

    void Date::FixDate()
    {
        int MinYear = 0;
            if (m_Year < MinYear)
                m_Year = MinYear;

        int MinMonth = 1; int MaxMonth = 12;
        if (m_Month < MinMonth)
            m_Month = MinMonth;
        else if (m_Month > MaxMonth)
            m_Month = MaxMonth;

        int MinDay = 1; int MaxDay = DaysInMonth[m_Month - 1];
        if (m_Year % 4 == 0) MaxDay++;
        if (m_Day < MinDay)
            m_Day = MinDay;
        else if (m_Day > MaxDay)
            m_Day = MaxDay;
    }

    nlohmann::basic_json<> Date::GetJson() const
    {
        nlohmann::basic_json<> result;
        result["Year"]  = m_Year;
        result["Month"] = m_Month;
        result["Day"]   = m_Day;

        return result;
    }

    void Date::SetJson(nlohmann::basic_json<> js)
    {
        if (!js.is_object())
            return;

        nlohmann::SetValue(m_Year,  js, "Year");
        nlohmann::SetValue(m_Month, js, "Month");
        nlohmann::SetValue(m_Day,   js, "Day");
    }
 
    bool operator<(const Date& left, const Date& right)
    {
        if (left.m_Year < right.m_Year)
            return true;
        else if (left.m_Year > right.m_Year)
            return false;

        if (left.m_Month < right.m_Month)
            return true;
        else if (left.m_Month > right.m_Month)
            return false;

        if (left.m_Day < right.m_Day)
            return true;

        return false;
    }

    bool operator>(const Date& left, const Date& right)
    {
        if (left.m_Year > right.m_Year)
            return true;
        else if (left.m_Year < right.m_Year)
            return false;

        if (left.m_Month > right.m_Month)
            return true;
        else if (left.m_Month < right.m_Month)
            return false;

        if (left.m_Day > right.m_Day)
            return true;
         //else if (left.Day < right.Day)
         //   return false;

        return false;
    }

    bool operator==(const Date& left, const Date& right)
    {
        if (left.m_Year == right.m_Year && left.m_Month == right.m_Month && left.m_Day == right.m_Day)
            return true;

        return false;
    }
        
    bool operator<=(const Date& left, const Date& right)
    {
        return !(left > right);
    }

    bool operator>=(const Date& left, const Date& right)
    {
        return !(left < right);
    }


    ////////////////////////////////////////////////////////////////////////////////
    // Money ///////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////

    nlohmann::basic_json<> Money::GetJson() const
    {
        nlohmann::basic_json<> result;
        result["Amount"] = m_Amount;

        return result;
    }

    void Money::SetJson(nlohmann::basic_json<> js)
    {
        if (!js.is_object())
            return;

        nlohmann::SetValue(m_Amount, js, "Amount");
    }

    bool Money::DrawEdit(const std::string &fieldName)
    {
        bool isEdit = false;
        char label[100];
        sprintf(label, "%lld.%02lld", m_Amount / 100, abs(m_Amount % 100));
        ImGui::PushItemWidth(250);
        if (ImGui::InputText(fieldName.c_str(), label, 100, ImGuiInputTextFlags_CharsDecimal))
        {
            long long Rub = 0, Cop = 0, Cop1 = 0, Cop2 = 0;
            sscanf(label, "%lld.%1lld%1lld", &Rub, &Cop1, &Cop2);
            Cop = 10 * Cop1 + Cop2;

            m_Amount = abs(Rub * 100) + abs(Cop);
            if (Rub < 0)
                m_Amount = -m_Amount;

            isEdit = true;
        }
        ImGui::PopItemWidth();

        return isEdit;
    }

    const Money operator+(const Money& left, const Money& right)
    {
        Money val;
        val.m_Amount = left.m_Amount + right.m_Amount;
        return val;
    }

    const Money operator-(const Money& left, const Money& right)
    {
        Money val;
        val.m_Amount = left.m_Amount - right.m_Amount;
        return val;
    }

    bool operator<(const Money& left, int right)
    {
        return left.m_Amount < right;
    }

    Money& operator+=(Money& left, const Money& right)
    {
        left.m_Amount += right.m_Amount;
        return left;
    }

    Money& operator-=(Money& left, const Money& right)
    {
        left.m_Amount -= right.m_Amount;
        return left;
    }


    ////////////////////////////////////////////////////////////////////////////////
    // Payment /////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////

    void Payment::DrawDocumentNumberEdit()
    {
        int bufSize = 20;
        //std::string InputId = "##DocumentNumber"; //+ std::to_string(id);
        char* NameBuf = new char[bufSize] { 0 };
        memcpy(NameBuf, m_DocumentNumber.c_str(), m_DocumentNumber.size());
        ImGui::PushItemWidth(250);
        if (ImGui::InputText(u8"Номер платежа", NameBuf, bufSize))
            m_DocumentNumber = NameBuf;
        delete NameBuf;
        ImGui::PopItemWidth();
    }

    std::ostream& operator<< (std::ostream& out, const Money money)
    {
        out << money.m_Amount / 100 << "." << money.m_Amount % 100;
        return out;
    }

    nlohmann::basic_json<> Payment::GetJson() const
    {
        nlohmann::basic_json<> result;
        result["Amount"]            = m_Amount.GetJson();
        result["Date"]              = m_Date.GetJson();
        result["FormOfPayment"]     = m_FormOfPayment;
        result["DocumentNumber"]    = m_DocumentNumber;

        return result;
    }

    void Payment::SetJson(nlohmann::basic_json<> js)
    {
        if (!js.is_object())
            return;

        m_Amount.SetJson(js["Amount"]);
        m_Date.SetJson(  js["Date"]);
        nlohmann::SetValue(m_FormOfPayment,   js, "FormOfPayment");
        nlohmann::SetValue(m_DocumentNumber,  js, "DocumentNumber");
    }


}
