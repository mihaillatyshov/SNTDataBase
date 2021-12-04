#include "OpeningBalance.h"

namespace LM
{

	nlohmann::basic_json<> OpeningBalance::GetJson() const
	{
		return m_Money.GetJson();
	}

	void OpeningBalance::SetJson(nlohmann::basic_json<> _JS)
	{
		if (!_JS.is_object())
			return;

		m_Money.SetJson(_JS);
	}

	nlohmann::basic_json<> OpeningBalance::GetDateJson()
	{
		return s_Date.GetJson();
	}

	void OpeningBalance::SetDateJson(nlohmann::basic_json<> _JS)
	{
		if (!_JS.is_object())
			return;

		s_Date.SetJson(_JS);
	}
}