#include "JsonLoaderUnloader.h"

#include <fstream>
#include <iomanip>

#include <nlohmann/json.hpp>

#include "Utils/JsonUtils.h"

namespace LM
{

	JsonLoaderUnloader::JsonLoaderUnloader(std::string_view _FileName, DataBase* _DataBase)
	{
		SetFileName(_FileName);
		SetDataBase(_DataBase);
	}

	void JsonLoaderUnloader::Load()
	{
		nlohmann::json JS = LoadFile();

		if (!JS.is_object())
			return;

		nlohmann::SetVector(m_DataBase->m_Homesteads, JS, "Homesteads");

		if (JS["OpeningBalance"].is_object())
			OpeningBalance::s_Date.SetJson(JS["OpeningBalance"]["Date"]);

		if (JS["MembershipFee"].is_object())
		{
			nlohmann::SetVector(MembershipFee::s_Accrual, JS["MembershipFee"], "Accrual");
		}
	}

	void JsonLoaderUnloader::Unload()
	{
		nlohmann::json JS;

		JS["Homesteads"] = nlohmann::GetVector(m_DataBase->m_Homesteads);

		JS["OpeningBalance"]["Date"] = OpeningBalance::s_Date.GetJson();

		JS["MembershipFee"]["Accrual"] = nlohmann::GetVector(MembershipFee::s_Accrual);

		UnloadFile(JS);
	}

	void JsonLoaderUnloader::UnloadFile(nlohmann::json _JS)
	{
		std::ofstream Fout(m_FileName);
		if (!Fout.is_open())
		{
			std::cout << "Can't open file to Unload Json Data! \n";
			return;
		}
		//std::cout << "Writing JSON to file . . . \n";
		Fout << std::setw(4) << _JS;
		std::cout << "Writing is done \n";
		Fout.close();
	}

	nlohmann::json JsonLoaderUnloader::LoadFile()
	{
		std::ifstream Fin(m_FileName);
		if (!Fin.is_open())
		{
			std::cout << "Can't open file to Load Json Data! \n";
			return nlohmann::json();
		}
		nlohmann::json JS;
		try
		{
			Fin >> JS;
			std::cout << "Reading is done \n";
		}
		catch (...)
		{
			std::cout << "We gote some error in json! \n";
		}
		Fin.close();

		return JS;
	}

}
