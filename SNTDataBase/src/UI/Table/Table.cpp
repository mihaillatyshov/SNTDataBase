#include "Table.h"

#include <imgui.h>
#include "Data/Homestead/Homestead.h"

namespace LM
{
	Table::Table(const std::vector<std::string>& _Names, const GE_F& _GetElement, const GEC_F& _GetElementsCount)
		: m_Names(_Names), m_GetElement(_GetElement), m_GetElementsCount(_GetElementsCount)
	{

	}


	void Table::Draw()
	{
		ImGuiTableFlags Flags =
			ImGuiTableFlags_Resizable
			| ImGuiTableFlags_Sortable | ImGuiTableFlags_SortMulti
			| ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders
			| ImGuiTableFlags_ScrollY
			| ImGuiTableFlags_SizingFixedFit;

		ImVec2 RegionAvail = ImGui::GetContentRegionAvail();
		if (ImGui::BeginTable("table_advanced", m_Names.size(), Flags, ImVec2(0.0f, RegionAvail.y - 50)))
		{
			ImGui::TableSetupScrollFreeze(0, 1);
			for (size_t i = 0; i < m_Names.size(); i++)
			{
				ImGui::TableSetupColumn(m_Names[i].data(), i == (m_Names.size() - 1) ? ImGuiTableColumnFlags_WidthStretch : 0, 0.0f, i);
			}
			ImGui::TableHeadersRow();

			ImGuiListClipper clipper;
			clipper.Begin(m_GetElementsCount());
			while (clipper.Step())
			{
				for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
				{
					ImGui::PushID(i);
					ImGui::TableNextRow();

					auto ElementDrawableColumns = m_GetElement(i)->GetDrawableColumns();

					if (ImGui::TableSetColumnIndex(0))
					{
						ElementDrawableColumns[0]();
						ImGui::SameLine();
						const ImGuiSelectableFlags SelectableFlags = ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowItemOverlap;
						const bool IsSelected = m_SelectedId == i;
						if (ImGui::Selectable("", IsSelected, SelectableFlags))
						{
							if (IsSelected)
							{
								m_SelectedId = -1;
							}
							else
							{
								m_SelectedId = i;
							}
						}
					}

					for (size_t i = 1; i < ElementDrawableColumns.size(); i++)
					{
						if (ImGui::TableSetColumnIndex(i))
						{
							ElementDrawableColumns[i]();
						}
					}

					ImGui::PopID();
				}
			}
			ImGui::EndTable();
		}
	}

}