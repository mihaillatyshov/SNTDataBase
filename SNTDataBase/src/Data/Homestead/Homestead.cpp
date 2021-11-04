#include "Homestead.h"

#include <imgui.h>

#include "Utils/JsonUtils.h"

namespace LM
{

	Homestead::Homestead(Ref<const TabDataStruct> _TabDS)
	{
		m_Data = std::static_pointer_cast<const HomesteadTabDS>(_TabDS)->GetData();
		if (m_Data.m_AddMembershipFees)
		{
			for (int i = 0; i < MembershipFee::s_Accrual.size(); i++)
			{
				m_MembershipFee.m_Debt += MembershipFee::s_Accrual[i].m_Money;
			}
		}
	}

	std::vector<std::function<void(void)>> Homestead::GetDrawableColumns() const
	{
		return
		{
			[=]()
			{
				ImGui::TextUnformatted(m_Data.m_Number.data());
			},
			[=]()
			{
				ImGui::Text(u8"%s %s %s", m_Data.m_Surname.data(),
										  m_Data.m_Forename.data(),
										  m_Data.m_Patronymic.data());
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

	void Homestead::Edit(Ref<const TabDataStruct> _TabDS)
	{
		auto NewData = std::static_pointer_cast<const HomesteadTabDS>(_TabDS)->GetData();
		if (m_Data.m_HasBenefits != NewData.m_HasBenefits)
		{
			m_MembershipFee.m_Debt = m_MembershipFee.m_OpeningBalance.m_Money;
			if (NewData.m_AddMembershipFees)
			{
				for (int i = 0; i < MembershipFee::s_Accrual.size(); i++)
				{
					m_MembershipFee.m_Debt += MembershipFee::s_Accrual[i].m_Money;
				}
			}
		}
		m_Data = NewData;
	}

	nlohmann::basic_json<> Homestead::GetJson() const
	{
		nlohmann::basic_json<> result;
		result["Number"]			= m_Data.m_Number;
		result["Surname"]			= m_Data.m_Surname;
		result["Forename"]			= m_Data.m_Forename;
		result["Patronymic"]		= m_Data.m_Patronymic;
		result["PhoneNumber"]		= m_Data.m_PhoneNumber;
		result["Note"]				= m_Data.m_Note;

		result["HasBenefits"]		= m_Data.m_HasBenefits;
		result["AddMembershipFees"] = m_Data.m_AddMembershipFees;

		result["MembershipFee"]		= m_MembershipFee.GetJson();
		result["Electricity"]		= m_Electricity.GetJson();

		return result;
	}

	void Homestead::SetJson(nlohmann::basic_json<> js)
	{
		if (!js.is_object())
			return;

		nlohmann::SetValue(m_Data.m_Number,				js, "Number");
		nlohmann::SetValue(m_Data.m_Surname,			js, "Surname");
		nlohmann::SetValue(m_Data.m_Forename,			js, "Forename");
		nlohmann::SetValue(m_Data.m_Patronymic,			js, "Patronymic");
		nlohmann::SetValue(m_Data.m_PhoneNumber,		js, "PhoneNumber");
		nlohmann::SetValue(m_Data.m_Note,				js, "Note");

		nlohmann::SetValue(m_Data.m_HasBenefits,		js, "HasBenefits");
		nlohmann::SetValue(m_Data.m_AddMembershipFees,	js, "AddMembershipFees");

		m_MembershipFee.SetJson(js["MembershipFee"]);
		m_Electricity.SetJson(  js["Electricity"]);
	}

}
