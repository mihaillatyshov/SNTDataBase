#pragma once

#include <iostream>
#include <vector>
#include <string>

#include "imgui.h"


namespace LM
{

    ////////////////////////////////////////////////////////////////////////////////
    // Date ////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////

    struct Date
    {
        static inline std::vector<int> DaysInMonth { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

        bool DrawDateEdit();
        inline void DrawDate() { ImGui::Text("%02d.%02d.%d", Day, Month, Year); }
        const std::string GetStringDate() const
        {
            char date[20]{0};
            sprintf(date, u8"%02d.%02d.%d", Day, Month, Year);
            return date;
        }

        void FixDate();

        int Year = 2019;
        int Month = 1;
        int Day = 1;

        friend bool operator<(const Date& left, const Date& right);
        friend bool operator>(const Date& left, const Date& right);
        friend bool operator<=(const Date& left, const Date& right);
        friend bool operator>=(const Date& left, const Date& right);
        friend bool operator==(const Date& left, const Date& right);
    };


    ////////////////////////////////////////////////////////////////////////////////
    // Money ///////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////

    struct Money
    {
        Money() = default;
        Money(long long Rub, long long Cop = 0)
        {
            char lable[50]{ 0 };
            sprintf(lable, "%lld.%02lld", Rub, abs(Cop));
            long long rub; long long cop;
            sscanf(lable, "%lld.%02lld", &rub, &cop);
            m_Amount = rub * 100 + abs(cop);
        }

        void Draw() { ImGui::Text(u8"%lld.%02lld руб", m_Amount / 100, abs(m_Amount % 100)); }
        void DrawAbs() { ImGui::Text(u8"%lld.%02lld руб", abs(m_Amount / 100), abs(m_Amount % 100)); }        
        bool DrawEdit(const std::string &fieldName);
        inline long long GetRubs() { return abs(m_Amount / 100); }
        inline long long GetCops() { return abs(m_Amount % 100); }
        friend const Money operator+(const Money& left, const Money& right);
        friend const Money operator-(const Money& left, const Money& right);

        friend bool operator<(const Money& left, int right);

        friend std::ostream& operator<< (std::ostream& out, const Money money);


        friend Money& operator+=(Money& left, const Money& right);
        friend Money& operator-=(Money& left, const Money& right);


        long long m_Amount = 0;

    };

    
    ////////////////////////////////////////////////////////////////////////////////
    // Payment /////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////

    class Payment
    {
    public:
        static inline std::vector<char*> s_FormOfPaymentString{ u8"Р/С",  u8"Касса" };

        Money m_Amount;
        Date m_Date;
        int m_FormOfPayment;
        std::string m_DocumentNumber;
        //bool m_IsActive = true;
    public:
        Payment()
            : m_FormOfPayment(0), m_DocumentNumber("") { }

        bool DrawPaymentBegin() { return ImGui::TreeNode(u8"Платеж", u8"Платеж %9s, %7s, %02d.%02d.%d, %12lld.%02lld руб", GetFormOfPayment().data(), m_DocumentNumber.c_str(), m_Date.Day, m_Date.Month, m_Date.Year, m_Amount.m_Amount / 100, abs(m_Amount.m_Amount % 100)); }

        void DrawPaymentEnd() { ImGui::TreePop(); }

        void DrawPayment(bool IsEdit);

        bool DrawDeleteButton();

        bool DrawRestoreButton();

        //inline void SetActive(bool isActive) { m_IsActive = isActive; }

        inline const Date &GetDate() const { return m_Date; }
        inline std::string_view GetFormOfPayment() { return s_FormOfPaymentString[m_FormOfPayment]; }

        inline Money& GetAmountRef() { return m_Amount; }
        //inline bool IsActive() const { return m_IsActive; }

    protected:
        void DrawDocumentNumberEdit();

        void DrawDocumentNumber() { ImGui::Text(u8"Номер документа: %s", m_DocumentNumber.c_str()); }

    };

}
