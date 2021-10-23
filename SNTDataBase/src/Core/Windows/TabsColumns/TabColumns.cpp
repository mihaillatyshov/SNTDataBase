#include "TabColumns.h"

#include <iostream>

#include <imgui.h>

namespace LM
{

    TabColumns::TabColumns(std::string_view _Title, const std::vector<std::string>& _Names, int WindowWidth)
        : m_Title(_Title)
    {
        for (const auto& Name : _Names)
        {
            m_Columns.emplace_back(Name, WindowWidth / _Names.size());
        }
    }

    void TabColumns::DrawNames()
    {
        ImGui::Columns(m_Columns.size(), m_Title.c_str(), true); // 2-ways, with border
        ImGui::Separator();
        //static bool Init = true;
        if (m_NeedUpdate)
        {
            for (int i = 0; i < m_Columns.size() - 1; i++)
            {
                ImGui::SetColumnWidth(i, m_Columns[i].GetWidth());
                std::cout << m_Columns[i].GetWidth() << " ";
            }
            std::cout << m_Columns[m_Columns.size() - 1].GetWidth() << "\n";
            m_NeedUpdate = false;
        }

        for (auto& Column : m_Columns)
        {
            Column.SetWidth(ImGui::GetColumnWidth());
            ImGui::Text(Column.GetName().data());
            ImGui::NextColumn();
        }
    }

    void TabColumns::Print() const
    {
        std::cout << m_Title << std::endl;
        for (const auto& Column : m_Columns)
        {
            Column.Print();
        }
    }

    nlohmann::basic_json<> TabColumns::GetJson() const
    {
        nlohmann::basic_json<> Result;
        for (const auto& Column : m_Columns)
        {
            Result[Column.GetName().data()] = Column.GetJson();
        }

        return Result;
    }

    void TabColumns::SetJson(nlohmann::basic_json<> _JS)
    {
        if (!_JS.is_object())
            return;

        for (auto& Column : m_Columns)
            Column.SetJson(_JS[Column.GetName().data()]);
    }

}
