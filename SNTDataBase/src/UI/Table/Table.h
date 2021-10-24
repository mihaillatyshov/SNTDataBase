#pragma once

#include <string>
#include <vector>

#include "TableElement.h"

namespace LM
{

	class Table 
	{
	public:
		typedef std::function<const std::shared_ptr<const TableElement>(size_t i)> GE_F;
		typedef std::function<size_t(void)> GEC_F;
		
		Table(const std::vector<std::string>& _Names, const GE_F& _GetElement, const GEC_F& _GetElementsCount);
		//virtual ~Table();

		inline int GetSelectedId() { return m_SelectedId; }
		inline void SetUnselected() { m_SelectedId = -1; }

		void Draw();

	protected:
		std::vector<std::string> m_Names;
		int m_SelectedId = -1;

		GE_F m_GetElement;
		GEC_F m_GetElementsCount;
	};

}