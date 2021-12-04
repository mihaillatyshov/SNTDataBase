#include "TabCsvSelector.h"

#include <imgui.h>
#include "TabCsvElectricity.h"

namespace LM
{

	TabCsvSelector::TabCsvSelector(Ref<DataBase> _DataBase)
		: m_DataBase(_DataBase)
	{
	}

	void TabCsvSelector::Open(std::string_view _FileName)
	{
		m_FileName = _FileName;
		m_DataType = DataType::ELECTRICITY;
		m_Tab = nullptr;
		m_IsOpen = true;
	}

	void TabCsvSelector::Draw()
	{
		DrawOpen();

		if (m_Tab)
		{
			m_Tab->Draw();
		}
	}

	void TabCsvSelector::DrawOpen()
	{
		if (ImGui::BeginTabItem(u8"Загрузка файла", &m_IsOpen, ImGuiTabItemFlags_SetSelected))
		{
			ImGui::Text(u8"Загружаемый файл: %s", m_FileName.data());

			ImGui::Combo(u8"Загружаемые данные", (int*)(&m_DataType), s_DataTypes.data(), (int)s_DataTypes.size());

			if (ImGui::Button(u8"Открыть"))
			{
				switch (m_DataType)
				{
				case DataType::ELECTRICITY:
					m_Tab = CreateRef<TabCsvElectricity>(m_FileName, m_DataBase);
					break;
				case DataType::HOMESTEAD:

					break;
				default:
					break;
				}
				Close();
			}
			ImGui::SameLine();
			if (ImGui::Button(u8"Отмена"))
			{
				Close();
			}

			ImGui::EndTabItem();
		}
	}

}