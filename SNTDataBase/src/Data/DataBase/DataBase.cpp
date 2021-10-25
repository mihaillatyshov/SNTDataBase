#include "DataBase.h"

namespace LM
{

	void DataBase::Push(const Homestead& homestead)
	{
		m_Homesteads.push_back(std::make_shared<Homestead>(homestead));
	}

}
