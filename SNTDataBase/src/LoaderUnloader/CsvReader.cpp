#include "CsvReader.h"

#include <iostream>
#include <iomanip>
#include <fstream>
#include <algorithm>

namespace LM
{

	CsvReader::CsvReader(std::string_view _FileName)
		: m_FileName(_FileName)
	{
		Load();
	}

	void CsvReader::RemoveColumn(int _Id)
	{
		std::for_each(m_Cells.begin(), m_Cells.end(), [_Id](std::vector<std::string>& _Row) { _Row.erase(_Row.begin() + _Id); });
	}

	void CsvReader::DebugPrint() const
	{
		for (size_t i = 0; i < m_Cells.size(); i++)
		{
			std::cout << std::setw(10) << i;
			for (const auto& Cell : m_Cells[i])
			{
				std::cout << std::setw(15) << Cell;
			}
			std::cout << std::endl;
		}
	}
	
	void CsvReader::Load()
	{
		std::ifstream Fin(m_FileName.data());
		m_IsFileOk = Fin.is_open();
		if (!m_IsFileOk)
			return;

		std::string Line;
		int MaskUTF;
		Fin.get((char*)&MaskUTF, 4);
		std::cout << "UTF-8 Mask: " << std::hex << MaskUTF << std::dec << std::endl;
		if (MaskUTF != 0xbfbbef)
		{
			Fin.close();
			Fin.open(m_FileName.data());
			m_IsFileOk = Fin.is_open();
			if (!m_IsFileOk)
				return;
		}

		while (std::getline(Fin, Line))
		{
			m_Cells.push_back(std::vector<std::string>());

			size_t Start = 0;
			size_t Pos = Line.find(';');
			while (Pos != std::string::npos)
			{
				//std::cout << Start << ";" << Pos - Start << " : " << Line.substr(Start, Pos - Start) << "    ";
				m_Cells.back().push_back(Line.substr(Start, Pos - Start));
	
				Start = Pos + 1;
				Pos = Line.find(';', Start);
			}
			//std::cout << std::endl;
		}
	}

}