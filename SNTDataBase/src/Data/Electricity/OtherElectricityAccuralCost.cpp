#include "OtherElectricityAccuralCost.h"

#include <nlohmann/JsonUtils.h>

namespace LM
{

    nlohmann::basic_json<> OtherElectricityAccuralCost::GetJson() const
    {
        nlohmann::basic_json<> Result;
        Result["Name"]  = m_Name;
        Result["Money"] = m_Money.GetJson();

        return Result;
    }

    void OtherElectricityAccuralCost::SetJson(nlohmann::basic_json<> _JS)
    {
        nlohmann::SetValue(m_Name, _JS, "Name");
        m_Money.SetJson(_JS["Money"]);
    }

}
