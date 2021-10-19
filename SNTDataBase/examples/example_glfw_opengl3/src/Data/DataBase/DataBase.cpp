#include "DataBase.h"

namespace LM
{

    void DataBase::Push(const Homestead& homestead)
    {
        m_Homestead.push_back(homestead);
    }

}
