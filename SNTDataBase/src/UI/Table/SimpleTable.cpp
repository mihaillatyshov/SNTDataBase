#include "SimpleTable.h"

#include <imgui.h>
#include "Data/Homestead/Homestead.h"

namespace LM
{
	SimpleTable::SimpleTable(const std::vector<std::string>& _Names, const GEDF_V& _GetElementDrawFunc, const GEC_F& _GetElementsCount)
		: ITable(_Names), m_GetElementDraw(_GetElementDrawFunc), m_GetElementsCount(_GetElementsCount)
	{

	}

	void SimpleTable::Draw()
	{
		ImVec2 RegionAvail = ImGui::GetContentRegionAvail();
		if (ImGui::BeginTable("table_advanced", (int)m_Names.size(), s_Flags, ImVec2(0.0f, RegionAvail.y - 50)))
		{
			ImGui::TableSetupScrollFreeze(0, 1);
			for (size_t i = 0; i < m_Names.size(); i++)
			{
				ImGui::TableSetupColumn(m_Names[i].data(), i == (m_Names.size() - 1) ? ImGuiTableColumnFlags_WidthStretch : 0, 0.0f, (ImGuiID)i);
			}
			ImGui::TableHeadersRow();

			m_Clipper.Begin((int)m_GetElementsCount());
			while (m_Clipper.Step())
			{
				for (int i = m_Clipper.DisplayStart; i < m_Clipper.DisplayEnd; i++)
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