#include "ElectricityAccrualCosts.h"

#include <imgui.h>

#include "Utils/JsonUtils.h"
#include "Utils/ImGuiUtils.h"

namespace LM
{

	bool ElectricityAccrualCosts::DrawEdit()
	{
		ImGuiDirtyDecorator DirtyDecorator;
		DirtyDecorator(m_Day.DrawEdit(u8"Дневной тариф"));
		DirtyDecorator(m_Night.DrawEdit(u8"Ночной тариф"));
		for (int i = 0; i < m_Others.size(); i++)
		{
			ImGui::PushID(i);
			ImGui::Separator();
			
			DirtyDecorator(m_Others[i].DrawEdit());

			if (ImGui::Button(u8"Удалить константку"))
			{
				m_Others.erase(m_Others.begin() + i--);
				DirtyDecorator(true);
			}
			ImGui::PopID();
		}

		ImGui::Separator();
		if (ImGui::Button(u8"Добавить константу"))
		{
			m_Others.push_back({ "<NONE>", Money() });
			DirtyDecorator(true);
		}

		return DirtyDecorator;
	}

	const Money& ElectricityAccrualCosts::GetOthersSum() const
	{
		Money Res(0);
		for (const auto& Other : m_Others)
			Res += Other.GetMoney();

		return Res;
	}

	nlohmann::basic_json<> ElectricityAccrualCosts::GetJson() const
	{
		nlohmann::basic_json<> Result;
		Result["Day"]	= m_Day.GetJson();
		Result["Night"] = m_Night.GetJson();

		Result["Others"] = nlohmann::GetVector(m_Others);

		return Result;
	}

	void ElectricityAccrualCosts::SetJson(nlohmann::basic_json<> _JS)
	{
		if (!_JS.is_object())
			return;

		m_Day.SetJson(	_JS["Day"]);
		m_Night.SetJson(_JS["Night"]);

		nlohmann::SetVector(m_Others, _JS, "Others");
	}

}
