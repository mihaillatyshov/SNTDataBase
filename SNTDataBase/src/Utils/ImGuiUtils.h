#pragma once

#include <string>
#include <imgui.h>

namespace LM
{

	void ImGuiPrepareCenteredPopup(float _X, float _Y);
	bool ImGuiInputTextString(std::string_view _Name, std::string& _Data, const float _ItemWidth = 0.0f, const ImGuiInputTextFlags_ _Flag = ImGuiInputTextFlags_None);
	int64_t GetIntFromSplitStr(std::string_view _Str, uint32_t _Precision);


	class ImGuiDirtyDecorator
	{
	public:
		void operator()(bool _Field) { m_IsDirty = _Field || m_IsDirty; }
		operator bool() const { return m_IsDirty; }
	protected:
		bool m_IsDirty = false;
	};

}