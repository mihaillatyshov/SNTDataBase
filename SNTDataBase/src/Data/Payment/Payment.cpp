#include "Payment.h"

#include "Utils/JsonUtils.h"

namespace LM
{

	////////////////////////////////////////////////////////////////////////////////
	// Payment /////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////

	void Payment::DrawDocumentNumberEdit()
	{
		int BufSize = 20;
		//std::string InputId = "##DocumentNumber"; //+ std::to_string(id);
		char* NameBuf = new char[BufSize] { 0 };
		memcpy(NameBuf, m_DocumentNumber.c_str(), m_DocumentNumber.size());
		ImGui::PushItemWidth(250);
		if (ImGui::InputText(u8"Номер платежа", NameBuf, BufSize))
			m_DocumentNumber = NameBuf;
		delete[] NameBuf;
		ImGui::PopItemWidth();
	}

	std::vector<std::function<void(void)>> Payment::GetDrawableColumns() const
	{
		return
		{
			[=]()
			{
				m_Date.Draw();
			},
			[=]()
			{
				m_Amount.Draw();
			},
			[=]()
			{
				ImGui::TextUnformatted(GetFormOfPayment().data());
			},
			[=]()
			{
				ImGui::TextUnformatted(m_DocumentNumber.c_str());
			}
		};
	}

	nlohmann::basic_json<> Payment::GetJson() const
	{
		nlohmann::basic_json<> Result;
		Result["Amount"]			= m_Amount.GetJson();
		Result["Date"]				= m_Date.GetJson();
		Result["FormOfPayment"]		= m_FormOfPayment;
		Result["DocumentNumber"]	= m_DocumentNumber;

		return Result;
	}

	void Payment::SetJson(nlohmann::basic_json<> _JS)
	{
		if (!_JS.is_object())
			return;

		m_Amount.SetJson(_JS["Amount"]);
		m_Date.SetJson(_JS["Date"]);
		nlohmann::SetValue(m_FormOfPayment,  _JS, "FormOfPayment");
		nlohmann::SetValue(m_DocumentNumber, _JS, "DocumentNumber");
	}


}
