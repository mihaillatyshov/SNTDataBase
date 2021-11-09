#pragma once

#include <nlohmann/json.hpp>

#include "Data/Date/Date.h"

namespace LM
{

	struct Privilege
	{
		bool HasPrivilege = false;
		Date Start;

		nlohmann::basic_json<> GetJson() const;
		void SetJson(nlohmann::basic_json<> js);
	};

}