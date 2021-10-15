#include "Loader_0_0_0.h"

#include <sstream>
#include <iostream>

namespace LM
{

    DataBase* Loader_0_0_0::LoadDataBase(std::ifstream &fin)
    {
        std::string line;
        if (std::getline(fin, line))
        {
            int HomesteadCount = 0;
            std::istringstream iss (line);
            iss >> HomesteadCount;                          // Get Homestead count;

            if (iss.fail()) 
                return new DataBase;                        // something wrong happened

            std::cout << HomesteadCount << std::endl;
            DataBase* database = new DataBase;
            for (int i = 0; i < HomesteadCount; i++)
            {
                Homestead homestead = *LoadHomestead(fin);   // Load Homesteads
                database->Push(homestead);
            }
            return database;
        }

        return new DataBase;
    }

    Homestead* Loader_0_0_0::LoadHomestead(std::ifstream &fin)
    {
        Homestead *homestead = new Homestead;
        std::string line;
        if (std::getline(fin, line))
            homestead->SetNumber(line);

        if (std::getline(fin, line))
            homestead->SetSurname(line);

        if (std::getline(fin, line))
            homestead->SetForename(line);

        if (std::getline(fin, line))
            homestead->SetPatronymic(line);

        if (std::getline(fin, line))
            homestead->SetPhoneNumber(line);

        if (std::getline(fin, line))
            homestead->SetNote(line);
        
        return homestead;
    }

}
