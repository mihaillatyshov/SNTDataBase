#include "Percent.h"

#include <imgui.h>

#include "Utils/JsonUtils.h"

namespace LM
{

	bool Percent::DrawEdit(std::string_view _Label, const uint64_t _Min, const uint64_t _Max)
	{
		ImGui::PushItemWidth(400.0f);
		bool IsDirty = ImGui::SliderScalar(_Label.data(), ImGuiDataType_U64, &m_Val, &_Min, &_Max);
		ImGui::PopItemWidth();
		return IsDirty;
	}

	nlohmann::basic_json<> Percent::GetJson() const
	{
		nlohmann::basic_json<> Result;
		Result["Val"] = m_Val;
		return Result;
	}

	void Percent::SetJson(nlohmann::basic_json<> _JS)
	{
		if (!_JS.is_object())
			return;

		nlohmann::SetValue(m_Val, _JS, "Val");
	}

}