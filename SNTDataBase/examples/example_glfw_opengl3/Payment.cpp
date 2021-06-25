#include "Payment.h"

namespace LM
{

    ////////////////////////////////////////////////////////////////////////////////
    // Date ////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////

    bool Date::DrawDateEdit()
    {
        bool IsChange = false;
        ImGui::PushItemWidth(100);
        ImGui::Text(u8"Год"); ImGui::SameLine();
        if (ImGui::InputInt(u8"##Год", &Year))
        {
            FixDate();
            IsChange = true;
        }
        ImGui::SameLine();
        ImGui::PopItemWidth();

        ImGui::PushItemWidth(80);
        ImGui::Text(u8"Месяц"); ImGui::SameLine();
        if (ImGui::InputInt(u8"##Месяц", &Month))
        {
            FixDate();
            IsChange = true;
        }
        ImGui::SameLine();

        ImGui::Text(u8"День"); ImGui::SameLine();
        if (ImGui::InputInt(u8"##День", &Day))
        {
            FixDate();
            IsChange = true;
        }
        ImGui::PopItemWidth();

        return IsChange;

    }


    void Date::FixDate()
    {
        int MinYear = 0;
            if (Year < MinYear)
                Year = MinYear;

        int MinMonth = 1; int MaxMonth = 12;
        if (Month < MinMonth)
            Month = MinMonth;
        else if (Month > MaxMonth)
            Month = MaxMonth;

        int MinDay = 1; int MaxDay = DaysInMonth[Month - 1];
        if (Year % 4 == 0) MaxDay++;
        if (Day < MinDay)
            Day = MinDay;
        else if (Day > MaxDay)
            Day = MaxDay;
    }


    
    bool operator<(const Date& left, const Date& right)
    {
        if (left.Year < right.Year)
            return true;
        else if (left.Year > right.Year)
            return false;

        if (left.Month < right.Month)
            return true;
        else if (left.Month > right.Month)
            return false;

        if (left.Day < right.Day)
            return true;
         //else if (left.Day > right.Day)
         //   return false;

        return false;
    }


    bool operator>(const Date& left, const Date& right)
    {
        if (left.Year > right.Year)
            return true;
        else if (left.Year < right.Year)
            return false;

        if (left.Month > right.Month)
            return true;
        else if (left.Month < right.Month)
            return false;

        if (left.Day > right.Day)
            return true;
         //else if (left.Day < right.Day)
         //   return false;

        return false;
    }


    bool operator==(const Date& left, const Date& right)
    {
        if (left.Year == right.Year && left.Month == right.Month && left.Day == right.Day)
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
        //char label[100];
        //sprintf(label, "%lld.%02lld", m_Amount / 100, abs(m_Amount % 100));
        //
        //ImGui::PushItemWidth(250);
        //if (ImGui::InputText(u8"Сумма платежа", label, 100, ImGuiInputTextFlags_CharsDecimal))
        //{
        //    long long Rub = 0, Cop = 0;
        //    sscanf(label, "%lld.%02lld", &Rub, &Cop);
        //
        //    m_Amount = abs(Rub * 100) + abs(Cop);
        //    if (Rub < 0) m_Amount = -m_Amount;
        //}
        //ImGui::PopItemWidth();
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

    void Payment::DrawPayment(bool IsEdit)
    {
        if (IsEdit)
        {
            m_Date.DrawDateEdit();
            m_Amount.DrawEdit(u8"Сумма платежа");
            ImGui::PushItemWidth(250);
            ImGui::Combo(u8"Форма платежа", &m_FormOfPayment, s_FormOfPaymentString.data(), s_FormOfPaymentString.size());
            ImGui::PopItemWidth();
            DrawDocumentNumberEdit();

        }
        else
        {
            m_Date.DrawDate();
            m_Amount.Draw();
            ImGui::Text(u8"Форма платежа: %s", s_FormOfPaymentString[m_FormOfPayment]);
            DrawDocumentNumber();
        }

    }


    bool Payment::DrawDeleteButton()
    {
        if (ImGui::Button(u8"Удалить платеж"))
            return true;
        return false;
    }


    bool Payment::DrawRestoreButton()
    {
        if (ImGui::Button(u8"Восстановить платеж"))
            return true;
        return false;
    }


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


}
