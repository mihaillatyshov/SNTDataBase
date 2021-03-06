#pragma once

#include <string>
#include <vector>

namespace LM
{

	class CsvReader
	{
	public:
		CsvReader(std::string_view _FileName);
		inline std::string_view GetCell(size_t _Row, size_t _Col) const { return m_Cells[_Row][_Col]; };
		void RemoveColumn(int _Id);
		void FillEmptyColumns(size_t size);

		inline bool IsFileOk()					const { return m_IsFileOk; }
		inline size_t GetRowsCount()			const { return m_RowsCount; };
		inline size_t GetColumnsCount()			const { return m_ColumnsCount; };
		inline std::string_view GetFileName()	const { return m_FileName; }

		const std::vector<std::string>& operator[](size_t _Row) { return m_Cells[_Row]; }
		void DebugPrint() const;
	protected:
		void Load();
	protected:
		std::string m_FileName;
		bool m_IsFileOk;
		std::vector<std::vector<std::string>> m_Cells;
		size_t m_RowsCount		= 0;
		size_t m_ColumnsCount	= 0;
	};

}