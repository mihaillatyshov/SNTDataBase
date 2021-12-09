#include "CsvTable.h"

#include <iostream>
#include <algorithm>

namespace LM
{

	CsvTable::CsvTable(const std::vector<std::string>& _Names, Ref<CsvReader> _Reader)
		: ITable(_Names), m_Reader(_Reader)
	{
		GenColumnsIds();
	}

	void CsvTable::Draw()
	{
		ImVec2 RegionAvail = ImGui::GetContentRegionAvail();
		if (ImGui::BeginTable("table_advanced", (int)m_Reader->GetColumnsCount(), s_Flags, ImVec2(0.0f, RegionAvail.y - 50)))
		{
			ImGui::TableSetupScrollFreeze(0, 1);
			for (size_t i = 0; i < m_ColumnsIds.size(); i++)
			{
				ImGui::TableSetupColumn(std::to_string(i).data(), i == (m_ColumnsIds.size() - 1) ? ImGuiTableColumnFlags_WidthStretch : 0, 0.0f, (ImGuiID)i);
			}
			ImGui::TableNextRow();
			for (size_t i = 0; i < m_ColumnsIds.size(); i++)
			{
				ImGui::PushID(i);
				if (ImGui::TableSetColumnIndex((int)i))
				{
					if (ImGui::Button(m_ColumnsIds[i] < m_Names.size() ? m_Names[m_ColumnsIds[i]].data() : std::to_string(m_ColumnsIds[i]).data(), ImVec2(-1, 0)))
					{
						std::cout << m_ColumnsIds[i] << std::endl;
						if (m_ColumnsIds[i] >= m_Names.size())
							ImGui::OpenPopup("ColumnMenu");
					}
					if (ImGui::BeginPopup("ColumnMenu"))
					{
						if (ImGui::Button(u8"Удалить данные столбца"))
						{
							m_ColumnsIds.erase(m_ColumnsIds.begin() + i);
							m_Reader->RemoveColumn(i);
							ImGui::CloseCurrentPopup();
						}
						ImGui::EndPopup();
					}
					if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
					{
						ImGui::SetDragDropPayload("SwapIds", &i, sizeof(int));
						ImGui::Text("Swap");
						ImGui::EndDragDropSource();
					}
					if (ImGui::BeginDragDropTarget())
					{
						if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SwapIds"))
						{
							int payload_i = *(const int*)payload->Data;
							std::swap(m_ColumnsIds[i], m_ColumnsIds[payload_i]);
						}
						ImGui::EndDragDropTarget();
					}
				}
				ImGui::PopID();
			}

			ImGuiListClipper clipper;
			clipper.Begin((int)m_Reader->GetRowsCount());
			while (clipper.Step())
			{
				for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
				{
					ImGui::PushID(i);
					ImGui::TableNextRow();

					for (size_t j = 0; j < m_Reader->GetColumnsCount(); j++)
					{
						if (ImGui::TableSetColumnIndex((int)j))
						{
							ImGui::TextUnformatted((*m_Reader)[i][j].data());
						}
					}

					ImGui::PopID();
				}
			}
			ImGui::EndTable();
		}
	}

	void CsvTable::GenColumnsIds()
	{
		m_ColumnsIds.resize(m_Reader->GetColumnsCount());
		std::generate(m_ColumnsIds.begin(), m_ColumnsIds.end(), [n = 0]() mutable { return n++; });
	}

	size_t CsvTable::GetColId(size_t _ColName) const
	{
		return std::find(m_ColumnsIds.begin(), m_ColumnsIds.end(), _ColName) - m_ColumnsIds.begin();
	}


}