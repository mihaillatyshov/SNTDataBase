#pragma once

#include <iostream>
#include <vector>
#include <string>

#include "imgui.h"
#include <nlohmann/json.hpp>


namespace LM
{

    ////////////////////////////////////////////////////////////////////////////////
    // Date ////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////

    struct Date
    {
        static inline std::vector<int> DaysInMonth { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

        bool DrawEdit();
        inline void Draw() { ImGui::Text("%02d.%02d.%d", m_Day, m_Month, m_Year); }

        const std::string GetString() const;

        void FixDate();

        nlohmann::basic_json<> GetJson() const;
        void SetJson(nlohmann::basic_json<> js);

        int m_Year = 2019;
        int m_Month = 1;
        int m_Day = 1;

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

        nlohmann::basic_json<> GetJson() const;
        void SetJson(nlohmann::basic_json<> js);

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
    public:
        Payment()
            : m_FormOfPayment(0), m_DocumentNumber("") { }

        inline const Date &GetDate() const { return m_Date; }
        inline std::string_view GetFormOfPayment() { return s_FormOfPaymentString[m_FormOfPayment]; }

        inline Money& GetAmountRef() { return m_Amount; }

        nlohmann::basic_json<> GetJson() const;
        void SetJson(nlohmann::basic_json<> js);
    protected:
        void DrawDocumentNumberEdit();

        void DrawDocumentNumber() { ImGui::Text(u8"Номер документа: %s", m_DocumentNumber.c_str()); }

    };

}
