#include "Money.h"

#include <imgui.h>

#include "Utils/JsonUtils.h"

namespace LM
{

	Money::Money(int64_t _Rub, int64_t _Cop)
	{
		if (_Cop >= 100)
			__debugbreak();

		m_Amount = (_Rub < 0 ? -1 : 1) * (abs(_Rub) * 100 + abs(_Cop));
	}

	void Money::Draw() const
	{
		ImGui::Text(u8"%lld.%02lld руб", m_Amount / 100, abs(m_Amount % 100));
	}

	void Money::DrawAbs() const
	{
		ImGui::Text(u8"%lld.%02lld руб", abs(m_Amount / 100), abs(m_Amount % 100));
	}

	bool Money::DrawEdit(std::string_view _FieldName, float _ItemWidth)
	{
		bool isEdit = false;
		char label[128];
		sprintf(label, "%lld.%02lld", m_Amount / 100, abs(m_Amount % 100));
		ImGui::PushItemWidth(_ItemWidth);
		if (ImGui::InputText(_FieldName.data(), label, 128, ImGuiInputTextFlags_CharsDecimal))
		{
			int64_t Rub = 0, Cop = 0, Cop1 = 0, Cop2 = 0;
			sscanf(label, "%lld.%1lld%1lld", &Rub, &Cop1, &Cop2);
			Cop = 10 * Cop1 + Cop2;

			m_Amount = abs(Rub * 100) + abs(Cop);
			if (Rub < 0)
				m_Amount = -m_Amount;

			isEdit = true;
		}
		ImGui::PopItemWidth();

		return isEdit;
	}

	nlohmann::basic_json<> Money::GetJson() const
	{
		nlohmann::basic_json<> Result;
		Result["Amount"] = m_Amount;
		Money(4, 3);
		return Result;
	}

	void Money::SetJson(nlohmann::basic_json<> _JS)
	{
		if (!_JS.is_object())
			return;

		nlohmann::SetValue(m_Amount, _JS, "Amount");
	}

	bool Money::operator<(int _Val) const
	{
		return m_Amount < _Val;
	}

	const Money Money::operator+(const Money& _Other) const
	{
		Money Res;
		Res.m_Amount = m_Amount + _Other.m_Amount;
		return Res;
	}

	const Money Money::operator-(const Money& _Other) const
	{
		Money Val;
		Val.m_Amount = m_Amount - _Other.m_Amount;
		return Val;
	}

	Money& Money::operator+=(const Money& _Other)
	{
		m_Amount += _Other.m_Amount;
		return *this;
	}

	Money& Money::operator-=(const Money& _Other)
	{
		m_Amount -= _Other.m_Amount;
		return *this;
	}

	std::ostream& operator<< (std::ostream& _Out, const Money _Money)
	{
		_Out << _Money.m_Amount / 100 << "." << abs(_Money.m_Amount % 100);
		return _Out;
	}

}
