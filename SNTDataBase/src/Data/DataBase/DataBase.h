#pragma once

#include <vector>

#include "../Homestead/Homestead.h"

namespace LM
{

	class DataBase
	{
	public:
		void Push(const Homestead& homestead);

		const std::shared_ptr<const Homestead> GetHomestead(size_t _Id) const { return m_Homesteads[_Id]; }
		size_t GetHomesteadsCount() const { return m_Homesteads.size(); }

		//std::vector<Homestead>& GetHomesteads() { return m_Homesteads; }

		std::vector<std::shared_ptr<Homestead>> m_Homesteads;
	};

}
