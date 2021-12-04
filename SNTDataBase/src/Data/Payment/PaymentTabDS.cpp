#include "PaymentTabDS.h"

#include "Utils/ImGuiUtils.h"

namespace LM
{

	void PaymentTabDS::Draw()
	{

		IsDirtyDecorator(m_Data.Date.DrawEdit());

		IsDirtyDecorator(m_Data.Amount.DrawEdit(u8"����� �������"));

		ImGui::PushItemWidth(250);
		IsDirtyDecorator(ImGui::Combo(u8"����� �������", &m_Data.FormOfPayment, PaymentData::s_FormOfPaymentString.data(), (int)PaymentData::s_FormOfPaymentString.size()));
		ImGui::PopItemWidth();

		IsDirtyDecorator(ImGuiInputTextString(u8"����� �������", m_Data.DocumentNumber, 250));
	}

}