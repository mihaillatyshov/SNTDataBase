#pragma once

#include "Core/Base.h"
#include "Data/DataBase/DataBase.h"
#include "Data/Money/Money.h"
#include "Data/Electricity/Accrual/ElectricityAccrualCosts.h"

namespace LM
{

	class Constants
	{
	public:
		Constants() = default;
		static Ref<Constants> Get();

		void DrawEdit();
		inline void SetDataBase(Ref<DataBase> _DataBase) { m_DataBase = _DataBase; }

		inline const ElectricityAccrualCosts&	GetCosts()		const { return m_Costs; }
		inline const Money&						GetAccrualMF()	const { return m_AccrualMF; }
		inline const Percent&					GetElBenefit()	const { return m_ElBenefit; }
		inline const Percent&					GetElLosses()	const { return m_ElLosses; }

		nlohmann::basic_json<> GetJson() const;
		void SetJson(nlohmann::basic_json<> _JS);
	protected:
		void Sep();
	protected:
		inline static Ref<Constants> s_Instance;

		Ref<DataBase> m_DataBase;

		// Data:
		ElectricityAccrualCosts m_Costs;
		Money	m_AccrualMF	= Money(500);
		Percent	m_ElBenefit = Percent(70);
		Percent	m_ElLosses	= Percent(3);

	};

}