#include "Homestead.h"

#include <nlohmann/JsonUtils.h>

namespace LM
{

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
