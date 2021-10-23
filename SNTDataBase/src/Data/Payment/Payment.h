#pragma once

#include <iostream>
#include <vector>
#include <string>

#include <imgui.h>
#include <nlohmann/json.hpp>

#include "../Date/Date.h"
#include "../Money/Money.h"

namespace LM
{
    
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
