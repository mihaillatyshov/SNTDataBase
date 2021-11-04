#include "DataBase.h"

namespace LM
{

	void DataBase::AddHomestead(const Homestead& _Homestead)
	{
		m_Homesteads.push_back(CreateRef<Homestead>(_Homestead));
	}
	
	void DataBase::AddHomestead(Ref<const TabDataStruct> _TabDS)
	{
		m_Homesteads.push_back(CreateRef<Homestead>(_TabDS));
	}

	void DataBase::EditHomestead(size_t _HsId, Ref<const TabDataStruct> _TabDS)
	{
		m_Homesteads[_HsId]->Edit(_TabDS);
	}

	void DataBase::DeleteHomestead(size_t _HsId)
	{
		m_Homesteads.erase(m_Homesteads.begin() + _HsId);
	}

}
