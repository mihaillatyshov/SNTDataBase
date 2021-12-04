#include "HomesteadTabDS.h"

#include <imgui.h>

#include <Utils/ImGuiUtils.h>

namespace LM
{

	void HomesteadTabDS::Draw()
	{
		IsDirtyDecorator(ImGuiInputTextString(u8"Номер участка", m_Data.Number, 100));
		
		IsDirtyDecorator(ImGuiInputTextString(u8"Фамилия", m_Data.Surname, 500));
		
		IsDirtyDecorator(ImGuiInputTextString(u8"Имя", m_Data.Forename, 500));
		
		IsDirtyDecorator(ImGuiInputTextString(u8"Отчество", m_Data.Patronymic, 500));
		
		IsDirtyDecorator(ImGuiInputTextString(u8"Телефон", m_Data.PhoneNumber, 200, ImGuiInputTextFlags_CharsDecimal));
		
		IsDirtyDecorator(ImGuiInputTextString(u8"Примечание", m_Data.Note, 700));
		
		IsDirtyDecorator(m_Data.ElectricityPrivilege.DrawEdit(u8"Льготы по электроэнергии"));
		
		IsDirtyDecorator(m_Data.MembershipFeePrivilege.DrawEdit(u8"Льготы по членским взносам"));
	}

}