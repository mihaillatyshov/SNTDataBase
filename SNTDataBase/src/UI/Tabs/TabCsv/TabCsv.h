#pragma once

#include "LoaderUnloader/CsvReader.h"
#include "Data/DataBase/DataBase.h"

namespace LM
{

	class TabCsv
	{
	public:
		TabCsv(std::string_view _FileName, Ref<DataBase> _DataBase);
		virtual void Draw() = 0;
		void Close() { m_IsOpen = false; }
	protected:
		bool m_IsOpen = true;
		Ref<DataBase> m_DataBase;
		CsvReader m_Reader;
		std::vector<size_t> m_ColumnsIds;
	};

}