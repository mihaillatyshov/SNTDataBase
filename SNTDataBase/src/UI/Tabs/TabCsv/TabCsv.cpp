#include "TabCsv.h"

#include <algorithm>

namespace LM
{

	TabCsv::TabCsv(std::string_view _FileName, const std::vector<std::string>& _Names, Ref<DataBase> _DataBase)
		: m_DataBase(_DataBase)
	{
		m_Reader = CreateRef<CsvReader>(_FileName);
		m_Reader->FillEmptyColumns(_Names.size());
		m_Table = CreateRef<CsvTable>(_Names, m_Reader);
	}

}