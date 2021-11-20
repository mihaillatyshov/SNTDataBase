#pragma once

#include <ctime>

namespace LM
{

	class Time
	{
	public:
		static std::tm* GetLocalTime()
		{

			std::time_t Time = std::time(NULL);
			return std::localtime(&Time);
		}
	};

}