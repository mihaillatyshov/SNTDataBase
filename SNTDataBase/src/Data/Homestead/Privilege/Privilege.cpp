#include "Privilege.h"

#include "Utils/JsonUtils.h"
#include "Utils/ImGuiUtils.h"

namespace LM
{

	bool Privilege::DrawEdit(std::string_view _Name)
	{
		ImGuiDirtyDecorator DirtyDecorator;
		DirtyDecorator(ImGui::Checkbox(_Name.data(), &m_HasPrivilege));
		if (m_HasPrivilege)
		{
			ImGui::SameLine();
			ImGui::TextUnformatted(u8"        Начало льготы:   ");
			ImGui::SameLine();
			DirtyDecorator(m_Start.DrawEdit());
		}
		return DirtyDecorator;
	}

	bool Privilege::GetHasPrivilege(const Date& _Date) const
	{
		return m_HasPrivilege && (_Date >= m_Start);
	}

	nlohmann::basic_json<> Privilege::GetJson() const
	{
		nlohmann::basic_json<> Result;
		Result["HasPrivilege"] = m_HasPrivilege;
		Result["Start"] = m_Start.GetJson();

		return Result;
	}

	void Privilege::SetJson(nlohmann::basic_json<> _JS)
	{
		if (!_JS.is_object())
			return;

		nlohmann::SetValue(m_HasPrivilege, _JS, "HasPrivilege");
		m_Start.SetJson(_JS["Start"]);
	}

}