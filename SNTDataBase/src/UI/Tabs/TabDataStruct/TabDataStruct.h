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
	protected:
		bool m_IsDirty = false;
	};

}