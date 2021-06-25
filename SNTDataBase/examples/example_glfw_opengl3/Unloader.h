#pragma once

#include <iostream>
#include <string>
#include <string_view>
#include <fstream>

#include "DataBase.h"

namespace LM
{

    class Unloader
    {
    protected:

    public:
        static Unloader* Create();
        virtual ~Unloader() = default;

        virtual bool SaveDataBase(DataBase* database) = 0;
        virtual std::string_view GetVersion() = 0;
    };

}
