#include "TabCsvElectricity.h"

#include <iostream>
#include <string>
#include <algorithm>

#include <imgui.h>

#include "Utils/ImGuiUtils.h"

using namespace std::string_literals;

namespace LM
{

	TabCsvElectricity::TabCsvElectricity(std::string_view _FileName, Ref<DataBase> _DataBase)
		: TabCsv(_FileName, _DataBase), m_Names({ u8"Дата", u8"Номер участка", u8"День", u8"Ночь", u8"Общее" })
	{
		
	}

	TabCsvElectricity::~TabCsvElectricity()
	{

	}

	void TabCsvElectricity::Draw()
	{
		if (ImGui::BeginTabItem(u8"Загрузка файла Электроэнергии", &m_IsOpen, ImGuiTabItemFlags_SetSelected))
		{
			if (!m_Reader.IsFileOk())
			{
				ImGui::Text(u8"Не получилось открыть файл: %s\nПопробуйте открыть заново!", m_Reader.GetFileName().data());
				ImGui::EndTabItem();
				return;
			}

			DrawChangeCosts();

			DrawAdd();
			ImGui::SameLine();
			if (ImGui::Button(u8"Отмена"))
			{
				Close();
			}

			ImGuiTableFlags Flags =
				ImGuiTableFlags_Resizable
				| ImGuiTableFlags_Sortable | ImGuiTableFlags_SortMulti
				| ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders
				| ImGuiTableFlags_ScrollY
				| ImGuiTableFlags_SizingFixedFit;

			ImVec2 RegionAvail = ImGui::GetContentRegionAvail();
			if (ImGui::BeginTable("table_advanced", (int)m_Reader.GetColumnsCount(), Flags, ImVec2(0.0f, RegionAvail.y - 50)))
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
								m_Reader.RemoveColumn(i);
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
				clipper.Begin((int)m_Reader.GetRowsCount());
				while (clipper.Step())
				{
					for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
					{
						ImGui::PushID(i);
						ImGui::TableNextRow();

						for (size_t j = 0; j < m_Reader.GetColumnsCount(); j++)
						{
							if (ImGui::TableSetColumnIndex((int)j))
							{
								ImGui::TextUnformatted(m_Reader[i][j].data());
							}
						}

						ImGui::PopID();
					}
				}
				ImGui::EndTable();
			}

			ImGui::EndTabItem();
		}
	}

	void TabCsvElectricity::DrawChangeCosts()
	{
		if (ImGui::Button(u8"Изменить стоимость начислений"))
		{
			ImGui::OpenPopup(u8"Изменить стоимость начислений");

		}
		ImGuiPrepareCenteredPopup(800, 500);
		if (ImGui::BeginPopupModal(u8"Изменить стоимость начислений", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDocking))
		{
			if (ImGui::Button(u8"Закрыть"))
			{
				ImGui::CloseCurrentPopup();
			}
			m_Costs.DrawEdit();

			ImGui::EndPopup();
		}
	}

	void TabCsvElectricity::DrawAdd()
	{
		size_t NumberColId = GetColId(ColumnName::NUMBER);
		if (ImGui::Button(u8"Добавить"))
		{
			// Проверка совпадения на участки 
			m_CheckData = TabCsvElectricityCheckData(m_DataBase->GetHomesteadsCount(), m_Reader.GetRowsCount());
			//std::cout << "NumberColId: " << NumberColId << std::endl;
			for (size_t i = 0; i < m_DataBase->GetHomesteadsCount(); i++)
			{
				for (int j = 0; j < m_Reader.GetRowsCount(); j++)
				{
					if (m_Reader.GetCell(j, NumberColId) == m_DataBase->GetHomestead(i)->GetNumber())
					{
						m_CheckData.CheckDataBase[i] = true;
						m_CheckData.CheckCsv[j] = true;
						break;
					}
				}
			}
			ImGui::OpenPopup(u8"Добавить начисления");
		}
		ImGuiPrepareCenteredPopup(800.0f, 500.0f);
		if (ImGui::BeginPopupModal(u8"Добавить начисления", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDocking))
		{
			ImGui::BeginChild("ChildCheckDataBase", ImVec2(ImGui::GetWindowContentRegionWidth() * 0.5f, 400), true, ImGuiWindowFlags_MenuBar);
			DrawChildName(u8"Несовпадения в базе");
			for (size_t i = 0; i < m_CheckData.CheckDataBase.size(); i++)
			{
				if (m_CheckData.CheckDataBase[i])
					continue;

				ImGui::PushID(i);
				auto t = m_CheckData.FixPair.find(i);
				std::string ButtonName = m_DataBase->GetHomestead(i)->GetNumber().data() +
					(m_CheckData.FixPair.find(i) == m_CheckData.FixPair.end() ? "" : " -> "s + m_Reader.GetCell(m_CheckData.FixPair[i], NumberColId).data());
				if (ImGui::Button(ButtonName.data()))
				{
					ImGui::OpenPopup("Выбор участка для фикса");
				}
				if (ImGui::BeginPopup("Выбор участка для фикса"))
				{
					for (int j = 0; j < m_Reader.GetRowsCount(); j++)
					{
						if (ImGui::Selectable(m_Reader.GetCell(j, NumberColId).data()))
						{
							m_CheckData.FixPair[i] = j;
							ImGui::CloseCurrentPopup();
						}
					}
					ImGui::EndPopup();
				}
				ImGui::PopID();
			}
			ImGui::EndChild();
			
			ImGui::SameLine();
			ImGui::BeginChild("ChildCheckCsv", ImVec2(0, 400), true, ImGuiWindowFlags_MenuBar);
			DrawChildName(u8"Несовпадения в файле");
			for (int i = 0; i < m_CheckData.CheckCsv.size(); i++)
			{
				if (m_CheckData.CheckCsv[i])
					continue;

				ImGui::PushID(i);
				ImGui::Text("  %s", m_Reader.GetCell(i, NumberColId).data(), std::to_string(m_CheckData.CheckCsv[i]).data());
				ImGui::PopID();
			}
			ImGui::EndChild();

			if (ImGui::Button(u8"Добавить", ImVec2(120, 0)))
			{
				Add();
				ImGui::CloseCurrentPopup();
				Close();
			}
			ImGui::SameLine();
			if (ImGui::Button(u8"Закрыть", ImVec2(120, 0)))
			{
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
		
		// Найденые несоответствия: 
		//    1) Нехватка данных в файле
		//    2) В файле есть участки, которых нет в базе
		// Возможность устранить конфликты?
		// Выбор действия:
		//    1) Все равно добавить(если есть несоответствия) / Добавить(Все ок)
		//    2) Отмена
		
	}

	void TabCsvElectricity::Add()
	{
		size_t NumberColId = std::find(m_ColumnsIds.begin(), m_ColumnsIds.end(), (size_t)ColumnName::NUMBER) - m_ColumnsIds.begin();
		for (size_t i = 0; i < m_CheckData.CheckCsv.size(); i++)
		{
			if (m_CheckData.CheckCsv[i])
			{
				//Date DateEA(m_Reader.GetCell(i, GetColId(ColumnName::DATE)));
				//KiloWatt DayEA(m_Reader.GetCell(i, GetColId(ColumnName::DAY)).data());
				//KiloWatt NightEA(m_Reader.GetCell(i, GetColId(ColumnName::NIGHT)).data());
				//
				//ElectricityAccrual EA({ DateEA, DayEA, NightEA, m_Costs });
				m_DataBase->AddElectricityAccrual(GetIdByNumber(m_Reader.GetCell(i, GetColId(ColumnName::NUMBER))), CreateAccrual(i));
			}
		}
		for (auto [HsId, CsvId] : m_CheckData.FixPair)
		{
			m_DataBase->AddElectricityAccrual(HsId, CreateAccrual(CsvId));
		}
	}

	ElectricityAccrual TabCsvElectricity::CreateAccrual(size_t _CsvId)
	{
		Date DateEA(m_Reader.GetCell(_CsvId, GetColId(ColumnName::DATE)));
		KiloWatt DayEA(m_Reader.GetCell(_CsvId, GetColId(ColumnName::DAY)).data());
		KiloWatt NightEA(m_Reader.GetCell(_CsvId, GetColId(ColumnName::NIGHT)).data());

		return ElectricityAccrual({ DateEA, DayEA, NightEA, m_Costs });
	}

	size_t TabCsvElectricity::GetIdByNumber(std::string_view _Number)
	{
		for (size_t i = 0; i < m_DataBase->GetHomesteadsCount(); i++)
		{
			if (m_DataBase->GetHomestead(i)->GetNumber() == _Number)
			{
				return i;
			}
		}
	}

	size_t TabCsvElectricity::GetColId(ColumnName _ColName)
	{
		return std::find(m_ColumnsIds.begin(), m_ColumnsIds.end(), (size_t)_ColName) - m_ColumnsIds.begin();

	}

	void TabCsvElectricity::DrawChildName(std::string_view _Name)
	{
		if (ImGui::BeginMenuBar())
		{
			ImGui::TextUnformatted(_Name.data());
			ImGui::EndMenuBar();
		}
	}

}