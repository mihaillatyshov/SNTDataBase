#pragma once

#include <vector>
#include <functional>

#include <imgui.h>

namespace LM
{

	class TabDataStruct
	{
	public:
		virtual void Draw() = 0;
		inline bool IsDirty() { return m_IsDirty; }
	protected: 
		inline void IsDirtyDecorator(bool _Field) { m_IsDirty = _Field || m_IsDirty; }
		bool InputTextString(std::string_view _Name, std::string& _Data, const float _ItemWidth = 0.0f, const ImGuiInputTextFlags_ _Flag = ImGuiInputTextFlags_None);
	protected:
		bool m_IsDirty = false;
	};

}