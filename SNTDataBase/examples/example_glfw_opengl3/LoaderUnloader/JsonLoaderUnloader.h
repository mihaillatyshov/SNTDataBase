#pragma once

#include <DataBase.h>

namespace LM
{

    class JsonLoaderUnloader
    {
    public:
        JsonLoaderUnloader(std::string_view fileName, DataBase* dataBase);

        inline void SetFileName(std::string_view fileName) { m_FileName = fileName; }
        inline void SetDataBase(DataBase* dataBase) { m_DataBase = dataBase; }

        void Load();
        void Unload();

    protected:
        void UnloadFile(nlohmann::json js);
        nlohmann::json LoadFile();
    protected:
        std::string m_FileName;
        DataBase* m_DataBase;
    };

}
