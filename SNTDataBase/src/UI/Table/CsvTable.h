#pragma once

#include <string>
#include <vector>

#include "ITable.h"
#include "Core/Base.h"
#include "LoaderUnloader/CsvReader.h"

namespace LM
{

	class CsvTable : public ITable
	{
	public:
		CsvTable(const std::vector<std::string>& _Names, Ref<CsvReader> _Reader);

		void Draw() override;
		void GenColumnsIds();
		size_t GetColId(size_t _ColName) const;
	protected:
		std::vector<size_t> m_ColumnsIds;

		Ref<CsvReader> m_Reader;
	};

}