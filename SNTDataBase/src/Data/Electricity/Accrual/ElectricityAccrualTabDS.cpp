#include "ElectricityAccrualTabDS.h"

#include <imgui.h>

namespace LM
{

	void ElectricityAccrualTabDS::Draw()
	{
		IsDirtyDecorator(m_Data.Date.DrawEdit());

		IsDirtyDecorator(m_Data.Day.DrawEdit(u8"Дневные начисления"));

		IsDirtyDecorator(m_Data.Night.DrawEdit(u8"Ночные начисления"));

		ImGui::PushItemWidth(250);
		(m_Data.Day + m_Data.Night).Draw();
		ImGui::PopItemWidth();
		ImGui::SameLine();
		ImGui::Text(u8"Обшее начисление");

		IsDirtyDecorator(m_Data.Costs.m_Day.DrawEdit(u8"Дневной тариф"));
		IsDirtyDecorator(m_Data.Costs.m_Night.DrawEdit(u8"Ночной тариф"));
		for (int i = 0; i < m_Data.Costs.m_Others.size(); i++)
		{
			ImGui::PushID(i);
			ImGui::Separator();
			auto& [Name, Amount] = m_Data.Costs.m_Others[i];
			
			IsDirtyDecorator(Amount.DrawEdit("##Sum"));
			ImGui::SameLine();
			IsDirtyDecorator(InputTextString("##Name", Name, 250));

			if (ImGui::Button(u8"Удалить константку"))
			{
				m_Data.Costs.m_Others.erase(m_Data.Costs.m_Others.begin() + i);
				m_IsDirty = true;
				i--;
			}
			ImGui::PopID();
		}

		ImGui::Separator();
		if (ImGui::Button(u8"Добавить константу"))
		{
			m_Data.Costs.m_Others.push_back({ "<NONE>", Money() });
			m_IsDirty = true;
		}
	}

}