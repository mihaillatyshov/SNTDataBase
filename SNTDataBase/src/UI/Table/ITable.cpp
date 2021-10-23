#include "ITable.h"

#include <imgui.h>

namespace LM
{
	


	void ITable::Draw()
	{
        ImGuiTableFlags Flags =
            ImGuiTableFlags_Resizable
            | ImGuiTableFlags_Sortable | ImGuiTableFlags_SortMulti
            | ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders
            | ImGuiTableFlags_ScrollY
            | ImGuiTableFlags_SizingFixedFit;

        ImVec2 RegionAvail = ImGui::GetContentRegionAvail();
        if (ImGui::BeginTable("table_advanced", 4, Flags, ImVec2(0.0f, RegionAvail.y - 50)))
        {
            ImGui::TableSetupScrollFreeze(0, 1);
            ImGui::TableSetupColumn(u8"Номер участка", 0, 0.0f, 0);
            ImGui::TableSetupColumn(u8"ФИО", 0, 0.0f, 1);
            ImGui::TableSetupColumn(u8"Членские взносы", 0, 0.0f, 2);
            ImGui::TableSetupColumn(u8"Электросеть", ImGuiTableColumnFlags_WidthStretch, 0.0f, 3);
            ImGui::TableHeadersRow();

            ImGuiListClipper clipper;
            clipper.Begin(GetRawCount());
            while (clipper.Step())
            {
                for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
                {
                    ImGui::PushID(i);
                    ImGui::TableNextRow();

                    Homestead& NowHomestead = m_DataBase->m_Homestead[i];
                    if (ImGui::TableSetColumnIndex(0))
                    {
                        const ImGuiSelectableFlags SelectableFlags = ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowItemOverlap;
                        const bool IsHomesteadSelected = SelectedHomesteadNumber == NowHomestead.GetNumber();
                        if (ImGui::Selectable(NowHomestead.GetNumber().data(), IsHomesteadSelected, SelectableFlags))
                        {
                            if (IsHomesteadSelected)
                            {
                                SelectedHomesteadNumber = "<NONE>";
                                SelectedHomestead = -1;
                            }
                            else
                            {
                                SelectedHomesteadNumber = NowHomestead.GetNumber();
                                SelectedHomestead = i;
                            }
                        }
                    }

                    if (ImGui::TableSetColumnIndex(1))
                    {
                        ImGui::Text(u8"%s %s %s", NowHomestead.GetSurname().data(),
                            NowHomestead.GetForename().data(),
                            NowHomestead.GetPatronymic().data());
                    }

                    if (ImGui::TableSetColumnIndex(2))
                    {
                        NowHomestead.GetMembershipFee().GetDebt().Draw();
                    }

                    if (ImGui::TableSetColumnIndex(3))
                    {
                        NowHomestead.GetElectricity().GetAll().Draw();
                    }

                    ImGui::PopID();
                }
            }
            ImGui::EndTable();
        }
	}

}