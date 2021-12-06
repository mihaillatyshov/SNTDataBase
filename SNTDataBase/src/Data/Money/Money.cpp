#include "Money.h"

#include <iostream>
#include <sstream>

#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>

#include "Utils/JsonUtils.h"
#include "Utils/ImGuiUtils.h"
#include "Utils/StreamUtils.h"

namespace LM
{

	Money::Money(int64_t _Rub, int64_t _Cop)
	{
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
		ImGuiDirtyDecorator DirtyDecorator;
		
		std::string Buffer = GetString();
		
		ImGui::PushItemWidth(_ItemWidth);
		if (ImGui::InputText(_FieldName.data(), &Buffer, ImGuiInputTextFlags_CharsDecimal))
		{
			m_Amount = GetIntFromSplitStr(Buffer, 2u);
			DirtyDecorator(true);
		}
		ImGui::PopItemWidth();

		return DirtyDecorator;
	}

	std::string Money::GetString() const
	{
		std::ostringstream Res;
		Res << m_Amount / 100 << '.' << Stream::Fill('0', 2) << abs(m_Amount % 100);
		return Res.str();
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

	bool Money::operator==(const Money& _Other) const
	{
		return m_Amount == _Other.m_Amount;
	}

	bool Money::operator!=(const Money& _Other) const
	{
		return !(*this == _Other);
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

	const Money Money::operator*(const KiloWatt& _Other) const
	{
		Money Res;
		Res.m_Amount = m_Amount * _Other.Get();
		Res.m_Amount = Res.m_Amount / 1000 + ((Res.m_Amount % 1'000) / 100 >= 5 ? 1 : 0);
		return Res;
	}

	const Money Money::operator*(const Percent& _Other) const
	{
		Money Res;
		Res.m_Amount = m_Amount * _Other.Get();
		Res.m_Amount = Res.m_Amount / 100 + ((Res.m_Amount % 100) / 10 >= 5 ? 1 : 0);
		return Res;
	}

	std::ostream& operator<<(std::ostream& _Out, const Money& _Money)
	{
		_Out << _Money.GetString();
		return _Out;
	}

}
