#include "PaymentTabDS.h"

namespace LM
{

	void PaymentTabDS::Draw()
	{

		IsDirtyDecorator(m_Data.Date.DrawEdit());

		IsDirtyDecorator(m_Data.Amount.DrawEdit(u8"Сумма платежа"));

		ImGui::PushItemWidth(250);
		IsDirtyDecorator(ImGui::Combo(u8"Форма платежа", &m_Data.FormOfPayment, PaymentData::s_FormOfPaymentString.data(), (int)PaymentData::s_FormOfPaymentString.size()));
		ImGui::PopItemWidth();

		IsDirtyDecorator(InputTextString(u8"Номер платежа", m_Data.DocumentNumber, 250));
	}

}