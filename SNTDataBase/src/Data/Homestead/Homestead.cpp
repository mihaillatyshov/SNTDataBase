#include "Homestead.h"

#include <imgui.h>

#include "Utils/JsonUtils.h"

namespace LM
{

	Homestead::Homestead(Ref<const TabDataStruct> _TabDS)
	{
		m_Data = std::static_pointer_cast<const HomesteadTabDS>(_TabDS)->GetData();
		if (m_Data.AddMembershipFees)
		{
			for (int i = 0; i < MembershipFee::s_Accrual.size(); i++)
			{
				m_MembershipFee.m_Debt += MembershipFee::s_Accrual[i].m_Money;
			}
		}
	}

	void Homestead::Edit(Ref<const TabDataStruct> _TabDS)
	{
		auto NewData = std::static_pointer_cast<const HomesteadTabDS>(_TabDS)->GetData();
		if (m_Data.ElectricityPrivilege.HasPrivilege != NewData.ElectricityPrivilege.HasPrivilege)
		{
			m_MembershipFee.m_Debt = m_MembershipFee.m_OpeningBalance.m_Money;
			if (NewData.AddMembershipFees)
			{
				for (int i = 0; i < MembershipFee::s_Accrual.size(); i++)
				{
					m_MembershipFee.m_Debt += MembershipFee::s_Accrual[i].m_Money;
				}
			}
		}
		m_Data = NewData;
	}

	void Homestead::AddMembershipFeePayment(Ref<const TabDataStruct> _TabDS)
	{
		m_MembershipFee.AddPayment(_TabDS);

	}

	void Homestead::EditMembershipFeePayment(size_t _PayId, Ref<const TabDataStruct> _TabDS)
	{
		m_MembershipFee.EditPayment(_PayId, _TabDS);
	}

	void Homestead::DeleteMembershipFeePayment(size_t _PayId)
	{
		m_MembershipFee.DeletePayment(_PayId);
	}

	void Homestead::AddElectricityAccrual(Ref<const TabDataStruct> _TabDS)
	{
		m_Electricity.AddAccrual(_TabDS);
		m_Electricity.Recalculate(m_Data.ElectricityPrivilege.HasPrivilege);
	}

	void Homestead::EditElectricityAccrual(size_t _AccId, Ref<const TabDataStruct> _TabDS)
	{
		m_Electricity.EditAccrual(_AccId, _TabDS);
		m_Electricity.Recalculate(m_Data.ElectricityPrivilege.HasPrivilege);
	}

	void Homestead::DeleteElectricityAccrual(size_t _AccId)
	{
		m_Electricity.DeleteAccrual(_AccId);
		m_Electricity.Recalculate(m_Data.ElectricityPrivilege.HasPrivilege);
	}

	void Homestead::AddElectricityPayment(Ref<const TabDataStruct> _TabDS)
	{
		m_Electricity.AddPayment(_TabDS);
		m_Electricity.Recalculate(m_Data.ElectricityPrivilege.HasPrivilege);
	}

	void Homestead::EditElectricityPayment(size_t _PayId, Ref<const TabDataStruct> _TabDS)
	{
		m_Electricity.EditPayment(_PayId, _TabDS);
		m_Electricity.Recalculate(m_Data.ElectricityPrivilege.HasPrivilege);
	}

	void Homestead::DeleteElectricityPayment(size_t _PayId)
	{
		m_Electricity.DeletePayment(_PayId);
		m_Electricity.Recalculate(m_Data.ElectricityPrivilege.HasPrivilege);
	}

	std::vector<std::function<void(void)>> Homestead::GetDrawableColumns() const
	{
		return
		{
			[=]()
			{
				ImGui::TextUnformatted(m_Data.Number.data());
			},
			[=]()
			{
				ImGui::Text(u8"%s %s %s", m_Data.Surname.data(), m_Data.Forename.data(), m_Data.Patronymic.data());
			},
			[=]()
			{
				m_MembershipFee.GetDebt().Draw();
			},
			[=]()
			{
				m_Electricity.GetAll().Draw();
			}
		};
	}

	void Homestead::FillDataStruct(Ref<TabDataStruct>& _TabDS) const
	{
		_TabDS = CreateRef<HomesteadTabDS>(m_Data);
	}

	nlohmann::basic_json<> Homestead::GetJson() const
	{
		nlohmann::basic_json<> Result;
		Result["Number"]					= m_Data.Number;
		Result["Surname"]					= m_Data.Surname;
		Result["Forename"]					= m_Data.Forename;
		Result["Patronymic"]				= m_Data.Patronymic;
		Result["PhoneNumber"]				= m_Data.PhoneNumber;
		Result["Note"]						= m_Data.Note;

		Result["ElectricityPrivilege"]		= m_Data.ElectricityPrivilege.GetJson();
		Result["AddMembershipFees"]			= m_Data.AddMembershipFees;

		Result["MembershipFee"]				= m_MembershipFee.GetJson();
		Result["Electricity"]				= m_Electricity.GetJson();

		return Result;
	}

	void Homestead::SetJson(nlohmann::basic_json<> _JS)
	{
		if (!_JS.is_object())
			return;

		nlohmann::SetValue(m_Data.Number,				_JS, "Number");
		nlohmann::SetValue(m_Data.Surname,				_JS, "Surname");
		nlohmann::SetValue(m_Data.Forename,				_JS, "Forename");
		nlohmann::SetValue(m_Data.Patronymic,			_JS, "Patronymic");
		nlohmann::SetValue(m_Data.PhoneNumber,			_JS, "PhoneNumber");
		nlohmann::SetValue(m_Data.Note,					_JS, "Note");

		m_Data.ElectricityPrivilege.SetJson(_JS["ElectricityPrivilege"]);
		nlohmann::SetValue(m_Data.AddMembershipFees,	_JS, "AddMembershipFees");

		m_MembershipFee.SetJson(_JS["MembershipFee"]);
		m_Electricity.SetJson(  _JS["Electricity"]);
	}

}
