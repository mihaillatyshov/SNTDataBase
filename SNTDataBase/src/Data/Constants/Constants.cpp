#include "Constants.h"

#include <imgui.h>

namespace LM
{

	Ref<Constants> Constants::Get() 
	{ 
		if (!s_Instance)
			s_Instance = CreateRef<Constants>();

		return s_Instance; 
	}


	void Constants::DrawEdit()
	{
		m_Costs.DrawEdit();											
		Sep();

		m_AccrualMF.DrawEdit(u8"Сумма начислений членских взносов");
		Sep();
		
		if (m_ElBenefit.DrawEdit(u8"Процент льготы электроэнергии"))
		{
			m_DataBase->RecalculateElectricity();
		}
		Sep();
		
		if (m_ElLosses.DrawEdit( u8"Процент потери электроэнергии"))
		{
			m_DataBase->RecalculateElectricity();
		}
		Sep();
	}

	void Constants::Sep()
	{
		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Separator();
	}

	nlohmann::basic_json<> Constants::GetJson() const
	{
		nlohmann::basic_json<> Result;
		Result["Costs"]		= m_Costs.GetJson();
		Result["AccrualMF"] = m_AccrualMF.GetJson();
		Result["ElBenefit"] = m_ElBenefit.GetJson();
		Result["ElLosses"]	= m_ElLosses.GetJson();
		return Result;
	}

	void Constants::SetJson(nlohmann::basic_json<> _JS)
	{
		m_Costs.SetJson(	_JS["Costs"]);
		m_AccrualMF.SetJson(_JS["AccrualMF"]);
		m_ElBenefit.SetJson(_JS["ElBenefit"]);
		m_ElLosses.SetJson(	_JS["ElLosses"]);
	}

}