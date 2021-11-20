#include "Homestead.h"

#include <imgui.h>

#include "Utils/JsonUtils.h"

namespace LM
{

	Homestead::Homestead()
	{
		RecalculateMembershipFee();
	}

	Homestead::Homestead(Ref<const TabDataStruct> _TabDS)
	{
		m_Data = std::static_pointer_cast<const HomesteadTabDS>(_TabDS)->GetData();
		RecalculateMembershipFee();
		RecalculateElectricity();
	}

	void Homestead::Edit(Ref<const TabDataStruct> _TabDS)
	{
		m_Data = std::static_pointer_cast<const HomesteadTabDS>(_TabDS)->GetData();
		RecalculateMembershipFee();
		RecalculateElectricity();
	}

	void Homestead::DrawFullName() const
	{
		ImGui::Text(u8"%s %s %s", m_Data.Surname.data(), m_Data.Forename.data(), m_Data.Patronymic.data());
	}

	void Homestead::AddMembershipFeePayment(Ref<const TabDataStruct> _TabDS)
	{
		m_MembershipFee.AddPayment(_TabDS);
		RecalculateMembershipFee();
	}

	void Homestead::EditMembershipFeePayment(size_t _PayId, Ref<const TabDataStruct> _TabDS)
	{
		m_MembershipFee.EditPayment(_PayId, _TabDS);
		RecalculateMembershipFee();
	}

	void Homestead::DeleteMembershipFeePayment(size_t _PayId)
	{
		m_MembershipFee.DeletePayment(_PayId);
		RecalculateMembershipFee();
	}

	void Homestead::SetMembershipFeeOpeningBalance(const Money& _Money)
	{
		m_MembershipFee.SetOpeningBalance(_Money);
		RecalculateMembershipFee();
	}

	void Homestead::AddElectricityAccrual(Ref<const TabDataStruct> _TabDS)
	{
		m_Electricity.AddAccrual(_TabDS);
		RecalculateElectricity();
	}

	void Homestead::EditElectricityAccrual(size_t _AccId, Ref<const TabDataStruct> _TabDS)
	{
		m_Electricity.EditAccrual(_AccId, _TabDS);
		RecalculateElectricity();
	}

	void Homestead::DeleteElectricityAccrual(size_t _AccId)
	{
		m_Electricity.DeleteAccrual(_AccId);
		RecalculateElectricity();
	}

	void Homestead::AddElectricityPayment(Ref<const TabDataStruct> _TabDS)
	{
		m_Electricity.AddPayment(_TabDS);
		RecalculateElectricity();
	}

	void Homestead::EditElectricityPayment(size_t _PayId, Ref<const TabDataStruct> _TabDS)
	{
		m_Electricity.EditPayment(_PayId, _TabDS);
		RecalculateElectricity();
	}

	void Homestead::DeleteElectricityPayment(size_t _PayId)
	{
		m_Electricity.DeletePayment(_PayId);
		RecalculateElectricity();
	}

	void Homestead::SetElectricityOpeningBalance(const Money& _Money)
	{
		m_Electricity.SetOpeningBalance(_Money);
		RecalculateElectricity();
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
				DrawFullName();
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

	void Homestead::RecalculateMembershipFee()
	{
		m_MembershipFee.Recalculate(m_Data.MembershipFeePrivilege);
	}

	void Homestead::RecalculateElectricity()
	{
		m_Electricity.Recalculate(m_Data.ElectricityPrivilege.HasPrivilege);
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
		Result["MembershipFeePrivilege"]	= m_Data.MembershipFeePrivilege.GetJson();

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

		m_Data.ElectricityPrivilege.SetJson(  _JS["ElectricityPrivilege"]);
		m_Data.MembershipFeePrivilege.SetJson(_JS["MembershipFeePrivilege"]);

		m_MembershipFee.SetJson(_JS["MembershipFee"]);
		m_Electricity.SetJson(  _JS["Electricity"]);
	}

}
