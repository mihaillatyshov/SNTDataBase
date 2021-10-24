#pragma once

#include <vector>
#include <functional>

namespace LM
{

	class TableElement
	{
	public:
		virtual std::vector<std::function<void(void)>> GetDrawableColumns() const = 0;
	};

}