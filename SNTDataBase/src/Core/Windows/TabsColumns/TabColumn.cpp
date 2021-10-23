#include "TabColumn.h"

#include <iostream>

#include <nlohmann/JsonUtils.h>

namespace LM
{

    TabColumn::TabColumn(std::string_view _Name, int _Width)
        : m_Name(_Name), m_Width(_Width)
    {
    }

    void TabColumn::SetWidth(int _Width)
    {
        m_Width = std::max(0, _Width);
    }

    void TabColumn::Print() const
    {
        std::cout << "\t" << m_Name << "\t" << m_Width << std::endl;
    }

    nlohmann::basic_json<> TabColumn::GetJson() const
    {
        nlohmann::basic_json<> Result;
        Result["Width"] = m_Width;

        return Result;
    }

    void TabColumn::SetJson(nlohmann::basic_json<> _JS)
    {
        if (!_JS.is_object())
            return;

        nlohmann::SetValue(m_Width, _JS, "Width");
    }

}
