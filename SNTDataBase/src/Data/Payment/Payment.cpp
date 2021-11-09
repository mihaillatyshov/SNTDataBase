#include "Payment.h"

#include "Utils/JsonUtils.h"

namespace LM
{

	Payment::Payment(Ref<const TabDataStruct> _TabDS)
	{
		m_Data = std::static_pointer_cast<const PaymentTabDS>(_TabDS)->GetData();
	}

	void Payment::Edit(Ref<const TabDataStruct> _TabDS)
	{
		m_Data = std::static_pointer_cast<const PaymentTabDS>(_TabDS)->GetData();
	}

	void Payment::FillDataStruct(Ref<TabDataStruct>& _TabDS) const
	{
		_TabDS = CreateRef<PaymentTabDS>(m_Data);
	}

	void Payment::DrawDocumentNumberEdit()
	{
		int BufSize = 20;
		//std::string InputId = "##DocumentNumber"; //+ std::to_string(id);
		char* NameBuf = new char[BufSize] { 0 };
		memcpy(NameBuf, m_Data.DocumentNumber.c_str(), m_Data.DocumentNumber.size());
		ImGui::PushItemWidth(250);
		if (ImGui::InputText(u8"Номер платежа", NameBuf, BufSize))
			m_Data.DocumentNumber = NameBuf;
		delete[] NameBuf;
		ImGui::PopItemWidth();
	}

	std::vector<std::function<void(void)>> Payment::GetDrawableColumns() const
	{
		return
		{
			[=]()
			{
				m_Data.Date.Draw();
			},
			[=]()
			{
				m_Data.Amount.Draw();
			},
			[=]()
			{
				ImGui::TextUnformatted(GetFormOfPayment().data());
			},
			[=]()
			{
				ImGui::TextUnformatted(m_Data.DocumentNumber.c_str());
			}
		};
	}

	nlohmann::basic_json<> Payment::GetJson() const
	{
		nlohmann::basic_json<> Result;
		Result["Amount"]			= m_Data.Amount.GetJson();
		Result["Date"]				= m_Data.Date.GetJson();
		Result["FormOfPayment"]		= m_Data.FormOfPayment;
		Result["DocumentNumber"]	= m_Data.DocumentNumber;

		return Result;
	}

	void Payment::SetJson(nlohmann::basic_json<> _JS)
	{
		if (!_JS.is_object())
			return;

		m_Data.Amount.SetJson(_JS["Amount"]);
		m_Data.Date.SetJson(  _JS["Date"]);
		nlohmann::SetValue(m_Data.FormOfPayment,  _JS, "FormOfPayment");
		nlohmann::SetValue(m_Data.DocumentNumber, _JS, "DocumentNumber");
	}


}
