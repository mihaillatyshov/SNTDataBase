#include "ElectricityAccrual.h"

namespace LM
{

	ElectricityAccrual::ElectricityAccrual(Ref<const TabDataStruct> _TabDS)
	{
		m_Data = std::static_pointer_cast<const ElectricityAccrualTabDS>(_TabDS)->GetData();
	}

	ElectricityAccrual::ElectricityAccrual(ElectricityAccrualData _Data)
		: m_Data(_Data)
	{

	}

	void ElectricityAccrual::Edit(Ref<const TabDataStruct> _TabDS)
	{
		m_Data = std::static_pointer_cast<const ElectricityAccrualTabDS>(_TabDS)->GetData();
	}

	std::vector<std::function<void(void)>> ElectricityAccrual::GetDrawableColumns() const
	{
		return
		{
			[=]()
			{
				m_Data.When.Draw();
			},
			[=]()
			{
				m_Data.Day.Draw();
			},
			[=]()
			{
				m_Data.Night.Draw();
			},
			[=]()
			{
				GetAllMonth().Draw();
			}
		};
	}

	void ElectricityAccrual::FillDataStruct(Ref<TabDataStruct>& _TabDS) const
	{
		_TabDS = CreateRef<ElectricityAccrualTabDS>(m_Data);
	}

	nlohmann::basic_json<> ElectricityAccrual::GetJson() const
	{
		nlohmann::basic_json<> Result;
		Result["Date"]	= m_Data.When.GetJson();
		Result["Day"]	= m_Data.Day.GetJson();
		Result["Night"] = m_Data.Night.GetJson();
		Result["Costs"] = m_Data.Costs.GetJson();

		return Result;
	}

	void ElectricityAccrual::SetJson(nlohmann::basic_json<> _JS)
	{
		if (!_JS.is_object())
			return;

		m_Data.When.SetJson(	_JS["Date"]);
		m_Data.Day.SetJson(		_JS["Day"]);
		m_Data.Night.SetJson(	_JS["Night"]);
		m_Data.Costs.SetJson(	_JS["Costs"]);
	}

}
