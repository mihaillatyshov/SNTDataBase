#pragma once

#include <string>

namespace LM
{
	struct HomesteadData
	{
		std::string m_Number		= "";
		std::string m_Surname		= "";
		std::string m_Forename		= "";
		std::string m_Patronymic	= "";
		std::string m_PhoneNumber	= "";
		std::string m_Note			= "";

		bool m_HasBenefits			= false;
		bool m_AddMembershipFees	= true;
	};
}