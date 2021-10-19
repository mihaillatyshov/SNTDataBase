#pragma once

#include <vector>
#include <string>

#include <nlohmann/json.hpp>

#include "TabColumn.h"

namespace LM
{

    class TabColumns
    {
    public:
        TabColumns(std::string_view _Title, const std::vector<std::string>& _Names, int WindowWidth);

        void DrawNames();
        inline void SetNeedUpdate(bool _NeedUpdate = true) { m_NeedUpdate = _NeedUpdate; }
        inline std::string_view GetTitle() const { return m_Title; }
        void Print() const;

        nlohmann::basic_json<> GetJson() const;
        void SetJson(nlohmann::basic_json<> _JS);
    protected:
        std::string m_Title;
        std::vector<TabColumn> m_Columns;

        bool m_NeedUpdate = true;
    };

}
