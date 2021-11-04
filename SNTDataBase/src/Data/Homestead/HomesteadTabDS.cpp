#include "HomesteadTabDS.h"

#include <imgui.h>

namespace LM
{

	void HomesteadTabDS::Draw()
	{
		IsDirtyDecorator(InputTextString(u8"����� �������", m_Data.m_Number, 100));
		
		IsDirtyDecorator(InputTextString(u8"�������", m_Data.m_Surname, 500));
		
		IsDirtyDecorator(InputTextString(u8"���", m_Data.m_Forename, 500));
		
		IsDirtyDecorator(InputTextString(u8"��������", m_Data.m_Patronymic, 500));
		
		IsDirtyDecorator(InputTextString(u8"�������", m_Data.m_PhoneNumber, 200, ImGuiInputTextFlags_CharsDecimal));
		
		IsDirtyDecorator(InputTextString(u8"����������", m_Data.m_Note, 700));
		
		IsDirtyDecorator(ImGui::Checkbox(u8"������ �� ��������������", &m_Data.m_HasBenefits));
		
		IsDirtyDecorator(ImGui::Checkbox(u8"��������� �������� ������?", &m_Data.m_AddMembershipFees));
	}


}