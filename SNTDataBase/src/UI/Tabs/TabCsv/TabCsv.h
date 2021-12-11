#pragma once

#include "LoaderUnloader/CsvReader.h"
#include "Data/DataBase/DataBase.h"
#include "UI/Table/CsvTable.h"

namespace LM
{

	class TabCsv
	{
	public:
		TabCsv(std::string_view _FileName, const std::vector<std::string>& _Names, Ref<DataBase> _DataBase);
		virtual void Draw() = 0;
		inline void Close() { m_IsOpen = false; }
	protected:
		bool m_IsOpen = true;
		Ref<DataBase>	m_DataBase;
		Ref<CsvReader>	m_Reader;
		Ref<CsvTable>	m_Table;
	};

}