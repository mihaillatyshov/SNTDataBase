#include "TabCsv.h"

#include <algorithm>

namespace LM
{

	TabCsv::TabCsv(std::string_view _FileName, Ref<DataBase> _DataBase)
		: m_Reader(_FileName), m_ColumnsIds(m_Reader.GetColumnsCount()), m_DataBase(_DataBase)
	{
		std::generate(m_ColumnsIds.begin(), m_ColumnsIds.end(), [n = 0]() mutable { return n++; });
	}

}