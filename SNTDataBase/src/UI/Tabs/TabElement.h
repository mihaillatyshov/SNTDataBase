#pragma once

#include "Core/Base.h"
#include "TabDataStruct/TabDataStruct.h"

namespace LM 
{
	
	class TabElement
	{
	public:
		virtual void FillDataStruct(Ref<TabDataStruct>& _TabDS) const = 0;
	};

}