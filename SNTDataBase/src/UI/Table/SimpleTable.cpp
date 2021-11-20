#include "SimpleTable.h"

#include <imgui.h>
#include "Data/Homestead/Homestead.h"

namespace LM
{
	SimpleTable::SimpleTable(const std::vector<std::string>& _Names, const GEDF_V& _GetElementDrawFunc, const GEC_F& _GetElementsCount)
		: m_Names(_Names), m_GetElementDraw(_GetElementDrawFunc), m_GetElementsCount(_GetElementsCount)
	{

	}

	void SimpleTable::Draw()
	{
		ImGuiTableFlags Flags =
			ImGuiTableFlags_Resizable
			| ImGuiTableFlags_Sortable | ImGuiTableFlags_SortMulti
			| ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders
			| ImGuiTableFlags_ScrollY
			| ImGuiTableFlags_SizingFixedFit;

		ImVec2 RegionAvail = ImGui::GetContentRegionAvail();
		if (ImGui::BeginTable("table_advanced", (int)m_Names.size(), Flags, ImVec2(0.0f, RegionAvail.y - 50)))
		{
			ImGui::TableSetupScrollFreeze(0, 1);
			for (size_t i = 0; i < m_Names.size(); i++)
			{
				ImGui::TableSetupColumn(m_Names[i].data(), i == (m_Names.size() - 1) ? ImGuiTableColumnFlags_WidthStretch : 0, 0.0f, (ImGuiID)i);
			}
			ImGui::TableHeadersRow();

			ImGuiListClipper clipper;
			clipper.Begin((int)m_GetElementsCount());
			while (clipper.Step())
			{
				for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
				{
					ImGui::PushID(i);
					ImGui::TableNextRow();

					for (size_t j = 0; j < m_GetElementDraw.size(); j++)
					{
						if (ImGui::TableSetColumnIndex((int)j))
						{
							m_GetElementDraw[j](i);
						}
					}

					ImGui::PopID();
				}
			}
			ImGui::EndTable();
		}
	}

}