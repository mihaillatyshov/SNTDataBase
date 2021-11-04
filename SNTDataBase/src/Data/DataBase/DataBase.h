#pragma once

#include <vector>

#include "../Homestead/Homestead.h"
#include "Core/Base.h"

namespace LM
{

	class DataBase
	{
	public:
		void AddHomestead(const Homestead& _Homestead);
		void AddHomestead(Ref<const TabDataStruct> _TabDS);
		void EditHomestead(size_t _HsId, Ref<const TabDataStruct> _TabDS);
		void DeleteHomestead(size_t _HsId);

		const Ref<const Homestead> GetHomestead(size_t _Id) const { return m_Homesteads[_Id]; }
		size_t GetHomesteadsCount() const { return m_Homesteads.size(); }

		//std::vector<Homestead>& GetHomesteads() { return m_Homesteads; }

		VectorRef<Homestead> m_Homesteads;
	};

}
