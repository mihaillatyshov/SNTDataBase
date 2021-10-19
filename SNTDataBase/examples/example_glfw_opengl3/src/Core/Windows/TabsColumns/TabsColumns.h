#pragma once

#include <unordered_map>
#include <imgui.h>

#include "TabColumns.h"

namespace LM
{

    class TabsColumns
    {
    public:
        TabsColumns();
        ~TabsColumns();

        void Load(const std::vector<std::pair<std::string, std::vector<std::string>>>& _Tabs, int WindowWidth);
        void DrawNames(std::string_view _Name);
        void SetNeedUpdate(bool _NeedUpdate = true);
        void Print() const;

        nlohmann::basic_json<> GetJson() const;
        void SetJson(nlohmann::basic_json<> _JS);
    public:
        std::vector<TabColumns> m_Tabs;

    };

}
