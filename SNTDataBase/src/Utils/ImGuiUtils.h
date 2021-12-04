#pragma once

#include <string>
#include <imgui.h>

namespace LM
{

	void ImGuiPrepareCenteredPopup(float _X, float _Y);
	bool ImGuiInputTextString(std::string_view _Name, std::string& _Data, const float _ItemWidth = 0.0f, const ImGuiInputTextFlags_ _Flag = ImGuiInputTextFlags_None);

	class ImGuiDirtyDecorator
	{
	public:
		void operator()(bool _Field) { m_IsDirty = _Field || m_IsDirty; }
		operator bool() const { return m_IsDirty; }
	protected:
		bool m_IsDirty = false;
	};

}