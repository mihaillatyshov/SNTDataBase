#pragma once

#include <string>

#include "UI/Tabs/TabDataStruct/TabDataStruct.h"
#include "PaymentData.h"

namespace LM
{

	class PaymentTabDS : public TabDataStruct
	{
	public:
		PaymentTabDS(const PaymentData& _Data = PaymentData())
			: m_Data(_Data) { }

		inline const PaymentData& GetData() const { return m_Data; }

		void Draw() override;
	protected:
		PaymentData m_Data;
	};

}