#pragma once

#include <vector>

#include "Homestead.h"

namespace LM
{

    class DataBase
    {
    public:
        void Push(const Homestead& homestead);

        std::vector<Homestead>& GetHomesteads() { return m_Homestead; }

        std::vector<Homestead> m_Homestead;
    };

}
