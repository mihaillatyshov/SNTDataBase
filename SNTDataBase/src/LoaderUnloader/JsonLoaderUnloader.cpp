#include "JsonLoaderUnloader.h"

#include <fstream>
#include <iomanip>

#include <nlohmann/json.hpp>

namespace LM
{

    JsonLoaderUnloader::JsonLoaderUnloader(std::string_view fileName, DataBase* dataBase)
    {
        SetFileName(fileName);
        SetDataBase(dataBase);
    }

    void JsonLoaderUnloader::Load()
    {
        nlohmann::json js = LoadFile();

        if (!js.is_object())
            return;

        m_DataBase->m_Homesteads.resize(js["Homesteads"].size());
        for (int i = 0; i < js["Homesteads"].size(); i++)
        {
            m_DataBase->m_Homesteads[i] = std::make_shared<Homestead>();
            m_DataBase->m_Homesteads[i]->SetJson(js["Homesteads"][i]);
        }

        if (js["OpeningBalance"].is_object())
            OpeningBalance::s_Date.SetJson(js["OpeningBalance"]["Date"]);

        if (js["MembershipFee"].is_object())
        {
            MembershipFee::s_Accural.resize(js["MembershipFee"]["Accural"].size());
            for (int i = 0; i < js["MembershipFee"]["Accural"].size(); i++)
            {
                MembershipFee::s_Accural[i].SetJson(js["MembershipFee"]["Accural"][i]);
            }
        }
    }

    void JsonLoaderUnloader::Unload()
    {

        nlohmann::json js;
        for (int i = 0; i < m_DataBase->m_Homesteads.size(); i++)
        {
            js["Homesteads"][i] = m_DataBase->m_Homesteads[i]->GetJson();
        }

        js["OpeningBalance"]["Date"] = OpeningBalance::s_Date.GetJson();

        for (int i = 0; i < MembershipFee::s_Accural.size(); i++)
        {
            js["MembershipFee"]["Accural"][i] = MembershipFee::s_Accural[i].GetJson();
        }

        UnloadFile(js);
    }

    void JsonLoaderUnloader::UnloadFile(nlohmann::json js)
    {
        std::ofstream fout(m_FileName);
        if (!fout.is_open())
        {
            std::cout << "Can't open file to Unload Json Data! \n";
            return;
        }
        //std::cout << "Writing JSON to file . . . \n";
        fout << std::setw(4) << js;
        std::cout << "Writing is done \n";
        fout.close();
    }

    nlohmann::json JsonLoaderUnloader::LoadFile()
    {
        std::ifstream fin(m_FileName);
        if (!fin.is_open())
        {
            std::cout << "Can't open file to Load Json Data! \n";
            return nlohmann::json();
        }
        nlohmann::json js;
        try
        {
            fin >> js;
            std::cout << "Reading is done \n";
        }
        catch (...)
        {
            std::cout << "We gote some error in json! \n";
        }
        fin.close();

        return js;
    }

}
