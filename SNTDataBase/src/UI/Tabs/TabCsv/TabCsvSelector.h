#pragma once

#include <string>

#include "Core/Base.h"
#include "Data/DataBase/DataBase.h"
#include "TabCsv.h"

namespace LM
{

	class TabCsvSelector
	{
	public:
		TabCsvSelector(Ref<DataBase> _DataBase);

		void Open(std::string_view _FileName);

		void Close() { m_IsOpen = false; }

		void Draw();
	protected:
		void DrawOpen();
	protected:
		static inline const std::vector<const char*> s_DataTypes { u8"Электроэнергия",  u8"Участки" };
		enum class DataType
		{
			ELECTRICITY,
			HOMESTEAD
		};
	protected:
		Ref<DataBase> m_DataBase;
		bool m_IsOpen = false;
		DataType m_DataType = DataType::ELECTRICITY;
		std::string m_FileName = "";
		Ref<TabCsv> m_Tab = nullptr;
	};

}