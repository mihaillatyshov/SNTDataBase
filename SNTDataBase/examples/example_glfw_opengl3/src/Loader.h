#pragma once

#include <iostream>
#include <string>
#include <string_view>
#include <fstream>

#include "DataBase.h"
#include "Homestead.h"

namespace LM
{

    class Loader
    {
    protected:
        static std::string GetVersionFromString(std::string_view line);
    public:
        static Loader* Create(std::ifstream &fin);

        Loader() { std::cout << "Loader Constructor" << std::endl; }
        virtual ~Loader() { std::cout << "Loader Destructor" << std::endl; }

        virtual DataBase* LoadDataBase(std::ifstream &fin) = 0;
        virtual Homestead* LoadHomestead(std::ifstream &fin) = 0;

        virtual std::string_view GetVersion() = 0;
    };

}
