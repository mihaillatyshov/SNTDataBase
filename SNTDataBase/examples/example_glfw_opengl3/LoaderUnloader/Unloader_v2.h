#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include "../Versions.h"
#include "../DataBase.h"

namespace LM
{
    class ClearLoaderUnloader_0_0_0;
    class ClearLoaderUnloader_0_0_1;
    class ClearLoaderUnloader_0_0_2;


    class Unloader_v2
    {
    public:
        static Unloader_v2* Create();

        virtual void HandleDataBase(DataBase* db) = 0;
    };

}
