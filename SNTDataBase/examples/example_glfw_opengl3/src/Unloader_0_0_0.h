#pragma once

#include "Unloader.h"

namespace LM
{

    class Unloader_0_0_0 : public Unloader
    {
    protected:

    public:
        virtual bool SaveDataBase(DataBase* database) override;
        virtual void SaveHomestead(Homestead &homestead, std::ofstream &fout);

        virtual std::string_view GetVersion() override { return "0.0.0"; }
    };

}
