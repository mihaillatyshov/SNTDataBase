#include "Privilege.h"

#include "Utils/JsonUtils.h"

namespace LM
{

	nlohmann::basic_json<> Privilege::GetJson() const
	{
		nlohmann::basic_json<> result;
		result["HasPrivilege"] = HasPrivilege;
		result["Start"] = Start.GetJson();

		return result;
	}

	void Privilege::SetJson(nlohmann::basic_json<> js)
	{
		if (!js.is_object())
			return;

		nlohmann::SetValue(HasPrivilege, js, "HasPrivilege");
		Start.SetJson(js["Start"]);
	}

}