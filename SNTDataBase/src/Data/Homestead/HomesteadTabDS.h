#pragma once

#include <string>

#include "UI/Tabs/TabDataStruct/TabDataStruct.h"
#include "HomesteadData.h"

namespace LM 
{

	class HomesteadTabDS : public TabDataStruct 
	{
	public:
		HomesteadTabDS(const HomesteadData& _Data = HomesteadData())
			: m_Data(_Data) { } 

		inline const HomesteadData& GetData() const { return m_Data; }

		void Draw() override;
	protected:
		HomesteadData m_Data;
	};

}