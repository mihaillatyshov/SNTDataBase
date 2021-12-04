#include "JsonLoaderUnloader.h"

#include <fstream>
#include <iomanip>

#include <nlohmann/json.hpp>

#include "Utils/JsonUtils.h"

namespace LM
{

	JsonLoaderUnloader::JsonLoaderUnloader(std::string_view _FileName, Ref<DataBase> _DataBase)
	{
		SetFileName(_FileName);
		SetDataBase(_DataBase);
	}

	void JsonLoaderUnloader::Load()
	{
		nlohmann::json JS = LoadFile();

		if (!JS.is_object())
			return;

		m_DataBase->SetJson(JS["DataBase"]); //nlohmann::SetVector(m_DataBase->m_Homesteads, JS, "Homesteads");

		if (JS["OpeningBalance"].is_object())
			OpeningBalance::SetDateJson(JS["OpeningBalance"]["Date"]);

		if (JS["MembershipFee"].is_object())
		{
			MembershipFee::SetAccrualsJson(JS["MembershipFee"], "Accruals");
		}
	}

	void JsonLoaderUnloader::Unload()
	{
		nlohmann::json JS;

		JS["DataBase"] = m_DataBase->GetJson(); //nlohmann::GetVector(m_DataBase->m_Homesteads);

		JS["OpeningBalance"]["Date"] = OpeningBalance::GetDateJson();
		
		JS["MembershipFee"]["Accruals"] = MembershipFee::GetAccrualsJson();

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
