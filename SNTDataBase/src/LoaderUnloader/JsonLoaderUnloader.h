#pragma once

#include "Data/DataBase/DataBase.h"

namespace LM
{

	class JsonLoaderUnloader
	{
	public:
		JsonLoaderUnloader(std::string_view _FileName, DataBase* _DataBase);

		inline void SetFileName(std::string_view _FileName) { m_FileName = _FileName; }
		inline void SetDataBase(DataBase* _DataBase) { m_DataBase = _DataBase; }

		void Load();
		void Unload();

	protected:
		void UnloadFile(nlohmann::json JS);
		nlohmann::json LoadFile();
	protected:
		std::string m_FileName;
		DataBase* m_DataBase;
	};

}
