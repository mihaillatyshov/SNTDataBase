#include "Payment.h"

#include <nlohmann/JsonUtils.h>

namespace LM
{

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
