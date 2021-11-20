#pragma once

#include <iostream>
#include <vector>
#include <string>

#include <imgui.h>
#include <nlohmann/json.hpp>

#include "../Date/Date.h"
#include "../Money/Money.h"
#include "UI/Table/TableElement.h"
#include "UI/Tabs/TabElement.h"
#include "PaymentData.h"
#include "PaymentTabDS.h"

namespace LM
{

	////////////////////////////////////////////////////////////////////////////////
	// Payment /////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////

	class Payment : public TableElement, public TabElement
	{
	public:
		Payment() = default;
		Payment(Ref<const TabDataStruct> _TabDS);

		void Edit(Ref<const TabDataStruct> _TabDS);

		inline const Date& GetDate() const { return m_Data.Date; }
		inline const Money& GetAmount() const { return m_Data.Amount; }
		inline std::string_view GetFormOfPayment() const { return PaymentData::s_FormOfPaymentString[m_Data.FormOfPayment]; }

		virtual std::vector<std::function<void(void)>> GetDrawableColumns() const override;

		nlohmann::basic_json<> GetJson() const;
		void SetJson(nlohmann::basic_json<> _JS);

		void FillDataStruct(Ref<TabDataStruct>& _TabDS) const override;
	protected:
		void DrawDocumentNumberEdit();

		void DrawDocumentNumber() { ImGui::Text(u8"Номер документа: %s", m_Data.DocumentNumber.c_str()); }
	public:
		PaymentData m_Data;
	};

}
