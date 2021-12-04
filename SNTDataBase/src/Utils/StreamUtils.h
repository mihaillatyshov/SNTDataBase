#pragma once

#include <iostream>
#include <iomanip>

namespace LM::Stream
{

	class Fill
	{
	public:
		Fill(char _Fill, size_t _Width) : m_Fill(_Fill), m_Width(_Width) {}

		friend std::ostream& operator<<(std::ostream& _Out, const Fill& _Fill)
		{
			_Out << std::setfill(_Fill.m_Fill) << std::setw(_Fill.m_Width);
			return _Out;
		}
	protected:
		char m_Fill;
		size_t m_Width;
	};

}