#include "Unloader_0_0_0.h"

#include <iostream>
#include <fstream>

namespace LM
{

    bool Unloader_0_0_0::SaveDataBase(DataBase* database)
    {
        std::ofstream fout;
	    fout.open("DataBase.db");
	
	    if (!fout.is_open())
	    {
		    std::cout << "Error: Can't Save file!" << std::endl;
            return false;
	    }
		std::cout << "File ok! Saving file..." << std::endl;
		fout << GetVersion() << std::endl;
        fout << database->GetHomesteads().size() << std::endl;
        for (auto& homestead : database->GetHomesteads())
        {
            SaveHomestead(homestead, fout);
        }


        std::cout << "File Saved!" << std::endl;
	    fout.close();

        return true;
    }


    void Unloader_0_0_0::SaveHomestead(Homestead &homestead, std::ofstream &fout)
    {
        fout << homestead.GetNumber() << std::endl;

        fout << homestead.GetSurname() << std::endl;

        fout << homestead.GetForename() << std::endl;

        fout << homestead.GetPatronymic() << std::endl;

        fout << homestead.GetPhoneNumber() << std::endl;

        fout << homestead.GetNote() << std::endl;

    }

}
