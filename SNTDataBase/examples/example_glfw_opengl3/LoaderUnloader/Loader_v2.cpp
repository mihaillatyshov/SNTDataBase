#include "Loader_v2.h"

#include "TLoader_v2.h"
#include "ClearLoaderUnloader.h"

namespace LM
{

    std::string Loader_v2::GetVersionFromString(std::string_view line)
    {
        if (line == "")
            return "0.0.0";
        for (const auto& str : s_Versions)
            if (line == str)
                return line.data();

        std::cout << "Error: Wrong file or version: ' " << line << " '" << std::endl;
        return "";
    }

    Loader_v2* Loader_v2::Create()
    {
        std::ifstream fin;
        std::string file = "DataBase.db";
        fin.open(file.c_str());

        if (!fin.is_open())
        {
            std::cout << "Error: Can't open file: " << file << std::endl;
            return nullptr;
        }

        std::string line;
        if (std::getline(fin, line))
        {
            std::cout << "line: " << line << std::endl;
        }
        std::string version = GetVersionFromString(line);
        std::cout << "Version: " << version << std::endl;

        if (version == "0.0.0")
            return new TLoader_v2<ClearLoaderUnloader_0_0_0>(std::move(fin));
        //if (version == "0.0.1")
        //    return new TLoader_v2<ClearLoaderUnloader_0_0_1>(std::move(fin));
        //if (version == "0.0.2")
        //    return new TLoader_v2<ClearLoaderUnloader_0_0_2>(std::move(fin));

        std::cout << "Error: Wrong file version: " << file << std::endl;
        return nullptr;
    }

}
