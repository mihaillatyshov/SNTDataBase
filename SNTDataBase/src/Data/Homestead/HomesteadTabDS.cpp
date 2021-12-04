#include "HomesteadTabDS.h"

#include <imgui.h>

#include <Utils/ImGuiUtils.h>

namespace LM
{

	void HomesteadTabDS::Draw()
	{
		IsDirtyDecorator(ImGuiInputTextString(u8"����� �������", m_Data.Number, 100));
		
		IsDirtyDecorator(ImGuiInputTextString(u8"�������", m_Data.Surname, 500));
		
		IsDirtyDecorator(ImGuiInputTextString(u8"���", m_Data.Forename, 500));
		
		IsDirtyDecorator(ImGuiInputTextString(u8"��������", m_Data.Patronymic, 500));
		
		IsDirtyDecorator(ImGuiInputTextString(u8"�������", m_Data.PhoneNumber, 200, ImGuiInputTextFlags_CharsDecimal));
		
		IsDirtyDecorator(ImGuiInputTextString(u8"����������", m_Data.Note, 700));
		
		IsDirtyDecorator(m_Data.ElectricityPrivilege.DrawEdit(u8"������ �� ��������������"));
		
		IsDirtyDecorator(m_Data.MembershipFeePrivilege.DrawEdit(u8"������ �� �������� �������"));
	}

}