#pragma once

#include "Loader.h"

namespace LM
{

    class Loader_0_0_0 : public Loader
    {
    public:
        virtual DataBase* LoadDataBase(std::ifstream &fin) override;
        virtual Homestead* LoadHomestead(std::ifstream &fin) override;

        Loader_0_0_0() { std::cout << "Loader_0_0_0 Constructor" << std::endl; }
        virtual ~Loader_0_0_0() { std::cout << "Loader_0_0_0 Destructor" << std::endl; }
        
        virtual std::string_view GetVersion() override { return "0.0.0"; }
    };

}
