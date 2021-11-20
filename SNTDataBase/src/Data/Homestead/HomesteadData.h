#pragma once

#include <string>

#include "Privilege/Privilege.h"

namespace LM
{

	struct HomesteadData
	{
		std::string Number				= "";
		std::string Surname				= "";
		std::string Forename			= "";
		std::string Patronymic			= "";
		std::string PhoneNumber			= "";
		std::string Note				= "";

		//bool HasElectricityPrivileges	= false;
		Privilege ElectricityPrivilege;
		Privilege MembershipFeePrivilege;
	};

}