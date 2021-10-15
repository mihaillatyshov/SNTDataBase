#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include "../Versions.h"
#include "../DataBase.h"

namespace LM
{

    class Loader_v2
    {
    public:
        static std::string GetVersionFromString(std::string_view line);

        static Loader_v2* Create();

        virtual void HandleDataBase(DataBase* db) = 0;
    };

}
