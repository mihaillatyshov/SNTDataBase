#pragma once

#include <unordered_map>

#include "TabCsv.h"
#include "Data/Electricity/Accrual/ElectricityAccrualCosts.h"

namespace LM
{
	struct TabCsvElectricityCheckData
	{
		TabCsvElectricityCheckData() = default;
		TabCsvElectricityCheckData(size_t _HomesteadsCount, size_t _TableRowsCount)
			: CheckDataBase(_HomesteadsCount, false), CheckCsv(_TableRowsCount, false)
		{

		}

		std::vector<bool> CheckDataBase;
		std::unordered_map<size_t, size_t> FixPair;
		std::vector<bool> CheckCsv;
	};

	class TabCsvElectricity : public TabCsv
	{
	protected:
		enum class ColumnName
		{
			DATE,
			NUMBER,
			DAY,
			NIGHT,
			ALL
		};
	public:
		TabCsvElectricity(std::string_view _FileName, Ref<DataBase> _DataBase);
		~TabCsvElectricity();

		void Draw() override;
	protected:
		void DrawFailed();
		void DrawOk();
		void DrawChangeCosts();
		void DrawAdd();
		void Add();
		ElectricityAccrual CreateAccrual(size_t _CsvId);
		size_t GetIdByNumber(std::string_view _Number);
		void DrawChildName(std::string_view _Name);
	protected:
		std::vector<std::string> m_Names;
		TabCsvElectricityCheckData m_CheckData;
		ElectricityAccrualCosts m_Costs;
	};

}