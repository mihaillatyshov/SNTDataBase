#include "ElectricityAccrualTabDS.h"

#include <imgui.h>

namespace LM
{

	void ElectricityAccrualTabDS::Draw()
	{
		IsDirtyDecorator(m_Data.Date.DrawEdit());

		IsDirtyDecorator(m_Data.Day.DrawEdit(u8"������� ����������"));

		IsDirtyDecorator(m_Data.Night.DrawEdit(u8"������ ����������"));

		ImGui::PushItemWidth(250);
		(m_Data.Day + m_Data.Night).Draw();
		ImGui::PopItemWidth();
		ImGui::SameLine();
		ImGui::Text(u8"����� ����������");

		IsDirtyDecorator(m_Data.Costs.DrawEdit());
	}

}