#include "Accural.h"
#include <nlohmann/JsonUtils.h>

namespace LM
{

    nlohmann::basic_json<> Accural::GetJson() const
    {
        nlohmann::basic_json<> result;
        result["Date"]  = m_Date.GetJson();
        result["Money"] = m_Money.GetJson();

        return result;
    }

    void Accural::SetJson(nlohmann::basic_json<> js)
    {
        if (!js.is_object())
            return;

        m_Date.SetJson(js["Date"]);
        m_Money.SetJson(js["Money"]);
    }

}
