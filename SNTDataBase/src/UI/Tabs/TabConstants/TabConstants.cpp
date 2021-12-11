#include "TabConstants.h"

#include "Data/Constants/Constants.h"

namespace LM
{

	TabConstants::TabConstants(Ref<DataBase> _DataBase)
	{
		Constants::Get()->SetDataBase(_DataBase);
	}

	void TabConstants::Draw()
	{
		if (ImGui::BeginTabItem(u8"Загрузка файла Электроэнергии", &m_IsOpen, ImGuiTabItemFlags_SetSelected))
		{
			Constants::Get()->DrawEdit();

			ImGui::EndTabItem();
		}
	}

}