#pragma once

#include <nlohmann/json.hpp>

#include "Data/Date/Date.h"

namespace LM
{

	struct Privilege
	{
	public:
		bool DrawEdit(std::string_view _Name);

		inline bool GetHasPrivilege() const { return m_HasPrivilege; }
			   bool GetHasPrivilege(const Date& _Date) const;
		inline const Date& GetStart() const { return m_Start; }

		nlohmann::basic_json<> GetJson() const;
		void SetJson(nlohmann::basic_json<> _JS);
	protected:
		bool m_HasPrivilege = false;
		Date m_Start;
	};

}