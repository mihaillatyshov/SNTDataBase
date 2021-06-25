#pragma once

#include "Loader.h"

namespace LM
{

    class Loader_0_0_2 : public Loader
    {
    public:
        virtual DataBase* LoadDataBase(std::ifstream &fin) override;
        virtual Homestead* LoadHomestead(std::ifstream &fin) override;
        virtual MembershipFee* LoadMembershipFee(std::ifstream& fin);
        virtual Electricity* LoadElectricity(std::ifstream &fin);

        Loader_0_0_2() { std::cout << "Loader_0_0_2 Constructor" << std::endl; }
        virtual ~Loader_0_0_2() { std::cout << "Loader_0_0_2 Destructor" << std::endl; }
        
        virtual std::string_view GetVersion() override { return "0.0.2"; }
        int GetInt(std::string_view str);
        int GetBool(std::string_view str);
        Date GetDate(std::string_view str);
    };

}
