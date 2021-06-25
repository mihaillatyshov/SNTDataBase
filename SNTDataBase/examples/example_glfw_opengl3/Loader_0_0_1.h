#pragma once

#include "Loader.h"

namespace LM
{

    class Loader_0_0_1 : public Loader
    {
    public:
        virtual DataBase* LoadDataBase(std::ifstream &fin) override;
        virtual Homestead* LoadHomestead(std::ifstream &fin) override;
        virtual MembershipFee* LoadMembershipFee(std::ifstream& fin);
        virtual Electricity* LoadElectricity(std::ifstream &fin);

        Loader_0_0_1() { std::cout << "Loader_0_0_1 Constructor" << std::endl; }
        virtual ~Loader_0_0_1() { std::cout << "Loader_0_0_1 Destructor" << std::endl; }
        
        virtual std::string_view GetVersion() override { return "0.0.1"; }
        int GetInt(std::string_view str);
        Date GetDate(std::string_view str);
    };

}
