#include "Loader.h"

#include <iostream>

#include "Loader_0_0_0.h"
#include "Loader_0_0_1.h"
#include "Loader_0_0_2.h"
#include "Versions.h"

namespace LM
{

    Loader* Loader::Create(std::ifstream& fin)
    {
        std::string line;
        if (std::getline(fin, line))
	    {
		    std::cout << "line: " << line << std::endl;
	    }
        std::string version = GetVersionFromString(line);
        std::cout << "Version: " << version << std::endl;

        if (version == "0.0.0")
            return new Loader_0_0_0();
        if (version == "0.0.1")
            return new Loader_0_0_1();
         if (version == "0.0.2")
            return new Loader_0_0_2();

        return nullptr;
    }


    std::string Loader::GetVersionFromString(std::string_view line)
    {
        //if (line == "" || line == "0.0.0")
        //    return "0.0.0";
        //else if (line == "0.0.1" || line == "0.0.2")
        //    return line.data();

        if (line == "")
            return "0.0.0";
        for (const auto& str : s_Versions)
            if (line == str)
                return line.data();

        std::cout << "Error: Wrong file or version: ' " << line << " '" << std::endl;
        return "";
    }


    //DataBase* Loader::LoadDataBase(std::string_view file)
    //{
    //    std::ifstream fin;
	//    fin.open(file.data());
    //    if (!fin.is_open())
	//    {
	//	    std::cout << "Error!!!" << std::endl;
    //        return nullptr;
	//    }
    //
    //    std::cout << "File ok!" << std::endl;
    //
    //    std::string line;
    //    // Get DataBase Version
	//    if (std::getline(fin, line))
	//    {
	//	    std::cout << "line: " << line << std::endl;
    //        SetVersion(line);
    //        std::cout << "Version: " << m_Version << std::endl;
	//    }
    //    else
    //    {
    //        SetVersion(line); // Set Version to 0.0.0
    //    }
    //
    //}
    //
    //
    //Homestead* Loader::LoadHomestead()
    //{
    //
    //}




}
