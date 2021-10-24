#include "Homestead.h"

#include <nlohmann/JsonUtils.h>
#include <imgui.h>

namespace LM
{

    std::vector<std::function<void(void)>> Homestead::GetDrawableColumns() const
    {
        return
        {
            [=]()
            {
                ImGui::TextUnformatted(m_Number.data());
            },
            [=]()
            {
                ImGui::Text(u8"%s %s %s", m_Surname.data(),
                                          m_Forename.data(),
                                          m_Patronymic.data());
            },
            [=]()
            {
                m_MembershipFee.GetDebt().Draw();
            },
            [=]()
            {
                m_Electricity.GetAll().Draw();
            }
        };
    }

    nlohmann::basic_json<> Homestead::GetJson() const
    {
        nlohmann::basic_json<> result;
        result["Number"]                = m_Number;
        result["Surname"]               = m_Surname;
        result["Forename"]              = m_Forename;
        result["Patronymic"]            = m_Patronymic;
        result["PhoneNumber"]           = m_PhoneNumber;
        result["Note"]                  = m_Note;
                                        
        result["HasBenefits"]           = m_HasBenefits;
        result["AddMembershipFees"]     = m_AddMembershipFees;

        result["MembershipFee"]         = m_MembershipFee.GetJson();
        result["Electricity"]           = m_Electricity.GetJson();

        return result;
    }

    void Homestead::SetJson(nlohmann::basic_json<> js)
    {
        if (!js.is_object())
            return;

        nlohmann::SetValue(m_Number,            js, "Number");
        nlohmann::SetValue(m_Surname,           js, "Surname");
        nlohmann::SetValue(m_Forename,          js, "Forename");
        nlohmann::SetValue(m_Patronymic,        js, "Patronymic");
        nlohmann::SetValue(m_PhoneNumber,       js, "PhoneNumber");
        nlohmann::SetValue(m_Note,              js, "Note");
                                                  
        nlohmann::SetValue(m_HasBenefits,       js, "HasBenefits");
        nlohmann::SetValue(m_AddMembershipFees, js, "AddMembershipFees");

        m_MembershipFee.SetJson(js["MembershipFee"]);
        m_Electricity.SetJson(  js["Electricity"]);
    }

}
