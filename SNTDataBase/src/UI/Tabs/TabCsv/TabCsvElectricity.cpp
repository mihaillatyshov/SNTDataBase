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
		: TabCsv(_FileName, { u8"����", u8"����� �������", u8"����", u8"����", u8"�����" }, _DataBase)
	{
	}

	TabCsvElectricity::~TabCsvElectricity()
	{

	}

	void TabCsvElectricity::Draw()
	{
		if (ImGui::BeginTabItem(u8"�������� ����� ��������������", &m_IsOpen, ImGuiTabItemFlags_SetSelected))
		{
			if (m_Reader->IsFileOk())
				DrawOk();
			else
				DrawFailed();

			ImGui::EndTabItem();
		}
	}

	void TabCsvElectricity::DrawOk()
	{
		DrawChangeCosts();
		DrawAdd();
		ImGui::SameLine();
		if (ImGui::Button(u8"������"))
		{
			Close();
		}

		m_Table->Draw();
	}

	void TabCsvElectricity::DrawFailed()
	{
		ImGui::Text(u8"�� ���������� ������� ����: %s\n���������� ������� ������!", m_Reader->GetFileName().data());
		if (ImGui::Button(u8"�������"))
		{
			Close();
		}
	}

	void TabCsvElectricity::DrawChangeCosts()
	{
		if (ImGui::Button(u8"�������� ��������� ����������"))
		{
			ImGui::OpenPopup(u8"�������� ��������� ����������");

		}
		ImGuiPrepareCenteredPopup(800, 500);
		if (ImGui::BeginPopupModal(u8"�������� ��������� ����������", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDocking))
		{
			if (ImGui::Button(u8"�������"))
			{
				ImGui::CloseCurrentPopup();
			}
			m_Costs.DrawEdit();

			ImGui::EndPopup();
		}
	}

	void TabCsvElectricity::DrawAdd()
	{
		size_t NumberColId = m_Table->GetColId((size_t)ColumnName::NUMBER);
		if (ImGui::Button(u8"��������"))
		{
			// �������� ���������� �� ������� 
			m_CheckData = TabCsvElectricityCheckData(m_DataBase->GetHomesteadsCount(), m_Reader->GetRowsCount());
			//std::cout << "NumberColId: " << NumberColId << std::endl;
			for (size_t i = 0; i < m_DataBase->GetHomesteadsCount(); i++)
			{
				for (int j = 0; j < m_Reader->GetRowsCount(); j++)
				{
					if (m_Reader->GetCell(j, NumberColId) == m_DataBase->GetHomestead(i)->GetNumber())
					{
						m_CheckData.CheckDataBase[i] = true;
						m_CheckData.CheckCsv[j] = true;
						break;
					}
				}
			}
			ImGui::OpenPopup(u8"�������� ����������");
		}
		ImGuiPrepareCenteredPopup(800.0f, 500.0f);
		if (ImGui::BeginPopupModal(u8"�������� ����������", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDocking))
		{
			ImGui::BeginChild("ChildCheckDataBase", ImVec2(ImGui::GetWindowContentRegionWidth() * 0.5f, 400), true, ImGuiWindowFlags_MenuBar);
			DrawChildName(u8"������������ � ����");
			for (size_t i = 0; i < m_CheckData.CheckDataBase.size(); i++)
			{
				if (m_CheckData.CheckDataBase[i])
					continue;

				ImGui::PushID(i);
				auto t = m_CheckData.FixPair.find(i);
				std::string ButtonName = m_DataBase->GetHomestead(i)->GetNumber().data() +
					(m_CheckData.FixPair.find(i) == m_CheckData.FixPair.end() ? "" : " -> "s + m_Reader->GetCell(m_CheckData.FixPair[i], NumberColId).data());
				if (ImGui::Button(ButtonName.data()))
				{
					ImGui::OpenPopup("����� ������� ��� �����");
				}
				if (ImGui::BeginPopup("����� ������� ��� �����"))
				{
					for (int j = 0; j < m_Reader->GetRowsCount(); j++)
					{
						if (ImGui::Selectable(m_Reader->GetCell(j, NumberColId).data()))
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
			DrawChildName(u8"������������ � �����");
			for (int i = 0; i < m_CheckData.CheckCsv.size(); i++)
			{
				if (m_CheckData.CheckCsv[i])
					continue;

				ImGui::PushID(i);
				ImGui::Text("  %s", m_Reader->GetCell(i, NumberColId).data(), std::to_string(m_CheckData.CheckCsv[i]).data());
				ImGui::PopID();
			}
			ImGui::EndChild();

			if (ImGui::Button(u8"��������", ImVec2(120, 0)))
			{
				Add();
				ImGui::CloseCurrentPopup();
				Close();
			}
			ImGui::SameLine();
			if (ImGui::Button(u8"�������", ImVec2(120, 0)))
			{
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
		
		// �������� ��������������: 
		//    1) �������� ������ � �����
		//    2) � ����� ���� �������, ������� ��� � ����
		// ����������� ��������� ���������?
		// ����� ��������:
		//    1) ��� ����� ��������(���� ���� ��������������) / ��������(��� ��)
		//    2) ������
		
	}

	void TabCsvElectricity::Add()
	{
		for (size_t i = 0; i < m_CheckData.CheckCsv.size(); i++)
		{
			if (m_CheckData.CheckCsv[i])
			{
				//Date DateEA(m_Reader->GetCell(i, GetColId(ColumnName::DATE)));
				//KiloWatt DayEA(m_Reader->GetCell(i, GetColId(ColumnName::DAY)).data());
				//KiloWatt NightEA(m_Reader->GetCell(i, GetColId(ColumnName::NIGHT)).data());
				//
				//ElectricityAccrual EA({ DateEA, DayEA, NightEA, m_Costs });
				m_DataBase->AddElectricityAccrual(GetIdByNumber(m_Reader->GetCell(i, m_Table->GetColId((size_t)ColumnName::NUMBER))), CreateAccrual(i));
			}
		}
		for (auto [HsId, CsvId] : m_CheckData.FixPair)
		{
			m_DataBase->AddElectricityAccrual(HsId, CreateAccrual(CsvId));
		}
	}

	ElectricityAccrual TabCsvElectricity::CreateAccrual(size_t _CsvId)
	{
		Date		DateEA(	m_Reader->GetCell(_CsvId, m_Table->GetColId((size_t)ColumnName::DATE)));
		KiloWatt	DayEA(	m_Reader->GetCell(_CsvId, m_Table->GetColId((size_t)ColumnName::DAY)).data());
		KiloWatt	NightEA(m_Reader->GetCell(_CsvId, m_Table->GetColId((size_t)ColumnName::NIGHT)).data());

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

	void TabCsvElectricity::DrawChildName(std::string_view _Name)
	{
		if (ImGui::BeginMenuBar())
		{
			ImGui::TextUnformatted(_Name.data());
			ImGui::EndMenuBar();
		}
	}

}