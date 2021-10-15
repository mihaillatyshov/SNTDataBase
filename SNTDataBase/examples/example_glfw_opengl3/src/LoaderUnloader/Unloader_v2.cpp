#include "Unloader_v2.h"

#include "TUnloader_v2.h"
#include "ClearLoaderUnloader.h"
#include <iostream>
#include <fstream>

namespace LM
{

    Unloader_v2* Unloader_v2::Create()
    {
        std::ofstream fout;
        std::string file = "DataBase.db";
        fout.open(file.c_str());

        if (!fout.is_open())
        {
            std::cout << "Error: Can't open file: " << file << std::endl;
            return nullptr;
        }

        std::string version = s_Versions.back();
        fout << version << std::endl;

        if (version == "0.0.0")
            return new TUnloader_v2<ClearLoaderUnloader_0_0_0>(std::move(fout));
        //if (version == "0.0.1")
        //    return new TUnloader_v2<ClearLoaderUnloader_0_0_1>(std::move(fout));
        //if (version == "0.0.2")
        //    return new TUnloader_v2<ClearLoaderUnloader_0_0_2>(std::move(fout));

        return nullptr;
    }

}
