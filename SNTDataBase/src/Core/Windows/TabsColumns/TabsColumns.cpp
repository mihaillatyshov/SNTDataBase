#include "TabsColumns.h"

#include <algorithm>

namespace LM
{

    TabsColumns::TabsColumns()
    {

    }

    TabsColumns::~TabsColumns()
    {

    }

    void TabsColumns::Load(const std::vector<std::pair<std::string, std::vector<std::string>>>& _Tabs, int WindowWidth)
    {
        for (const auto& [Title, Names] : _Tabs)
        {
            m_Tabs.emplace_back(Title, Names, WindowWidth);
        }
    }

    void TabsColumns::DrawNames(std::string_view _Name)
    {
        auto& Columns = std::find_if(m_Tabs.begin(), m_Tabs.end(), [=](const TabColumns& Now) { return Now.GetTitle() == _Name; });
    }

    void TabsColumns::SetNeedUpdate(bool _NeedUpdate)
    {
        for (auto& Tab : m_Tabs)
        {
            Tab.SetNeedUpdate(_NeedUpdate);
        }
    }

    void TabsColumns::Print() const
    {
        for (const auto& Tab : m_Tabs)
        {
            Tab.Print();
        }
    }

    nlohmann::basic_json<> TabsColumns::GetJson() const
    {
        nlohmann::basic_json<> Result;

        return Result;
    }

    void TabsColumns::SetJson(nlohmann::basic_json<> _JS)
    {

    }

}
