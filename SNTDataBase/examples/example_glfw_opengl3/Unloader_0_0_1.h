#pragma once

#include "Unloader.h"

namespace LM
{

    class Unloader_0_0_1 : public Unloader
    {
    protected:

    public:
        virtual bool SaveDataBase(DataBase* database) override;
        virtual void SaveHomestead(Homestead &homestead, std::ofstream &fout);
        virtual void SaveMembershipFee(MembershipFee &membershipFee, std::ofstream &fout);
        virtual void SaveElectricity(Electricity& electricity, std::ofstream& fout);

        virtual void SaveStaticDate(DataBase* database, std::ofstream &fout);

        virtual std::string_view GetVersion() override { return "0.0.1"; }
    };

}
