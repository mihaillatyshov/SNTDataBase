#include "HomesteadTabDS.h"

#include <imgui.h>

namespace LM
{

	void HomesteadTabDS::Draw()
	{
		IsDirtyDecorator(InputTextString(u8"����� �������", m_Data.Number, 100));
		
		IsDirtyDecorator(InputTextString(u8"�������", m_Data.Surname, 500));
		
		IsDirtyDecorator(InputTextString(u8"���", m_Data.Forename, 500));
		
		IsDirtyDecorator(InputTextString(u8"��������", m_Data.Patronymic, 500));
		
		IsDirtyDecorator(InputTextString(u8"�������", m_Data.PhoneNumber, 200, ImGuiInputTextFlags_CharsDecimal));
		
		IsDirtyDecorator(InputTextString(u8"����������", m_Data.Note, 700));
		
		IsDirtyDecorator(ImGui::Checkbox(u8"������ �� ��������������", &m_Data.ElectricityPrivilege.HasPrivilege));
		if (m_Data.ElectricityPrivilege.HasPrivilege)
		{
			ImGui::SameLine();
			ImGui::TextUnformatted(u8"        ������ ������:   ");
			ImGui::SameLine();
			m_Data.ElectricityPrivilege.Start.DrawEdit();
		}
		
		IsDirtyDecorator(ImGui::Checkbox(u8"������ �� �������� �������", &m_Data.MembershipFeePrivilege.HasPrivilege));
		if (m_Data.MembershipFeePrivilege.HasPrivilege)
		{
			ImGui::SameLine();
			ImGui::TextUnformatted(u8"        ������ ������:   ");
			ImGui::SameLine();
			m_Data.MembershipFeePrivilege.Start.DrawEdit();
		}

	}

}