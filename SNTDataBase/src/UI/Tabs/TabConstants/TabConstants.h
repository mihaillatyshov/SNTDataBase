#pragma once

#include "Data/DataBase/DataBase.h"

namespace LM
{

	class TabConstants
	{
	public:
		TabConstants(Ref<DataBase> _DataBase);
		void Draw();
		inline bool GetIsOpen() const { return m_IsOpen; }
		inline void SwitchOpen() { m_IsOpen = !m_IsOpen; }
	protected:
		bool m_IsOpen = false;
	};

}