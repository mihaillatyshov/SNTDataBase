#pragma once

#include <string>
#include <vector>

#include "TableElement.h"
#include "Core/Base.h"

namespace LM
{

	class SimpleTable
	{
	public:
		typedef std::vector<std::function<void(size_t)>> GEDF_V;
		typedef std::function<size_t(void)> GEC_F;

		SimpleTable(const std::vector<std::string>& _Names, const GEDF_V& _GetElementDrawFunc, const GEC_F& _GetElementsCount);
		//virtual ~Table();

		void Draw();
	protected:
		std::vector<std::string> m_Names;
		int m_SelectedId = -1;

		GEDF_V m_GetElementDraw;
		GEC_F m_GetElementsCount;
	};

}