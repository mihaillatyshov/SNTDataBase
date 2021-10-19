#include "ElectricityAccural.h"

namespace LM
{

    nlohmann::basic_json<> ElectricityAccural::GetJson() const
    {
        nlohmann::basic_json<> Result;
        Result["Date"]  = m_Date.GetJson();
        Result["Day"]   = m_Day.GetJson();
        Result["Night"] = m_Night.GetJson();
        Result["Costs"] = m_Costs.GetJson();

        return Result;
    }

    void ElectricityAccural::SetJson(nlohmann::basic_json<> _JS)
    {
        if (!_JS.is_object())
            return;

        m_Date.SetJson( _JS["Date"]);
        m_Day.SetJson(  _JS["Day"]);
        m_Night.SetJson(_JS["Night"]);
        m_Costs.SetJson(_JS["Costs"]);
    }

}
