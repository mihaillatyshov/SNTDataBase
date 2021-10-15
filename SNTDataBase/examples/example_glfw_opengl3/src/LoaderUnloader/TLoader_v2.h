#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include "../Versions.h"
#include "Loader_v2.h"
#include "ClearLoaderUnloader.h"

namespace LM
{
    class ClearLoaderUnloader_0_0_0;
    class ClearLoaderUnloader_0_0_1;
    class ClearLoaderUnloader_0_0_2;


    template <class T>
    class TLoader_v2 : public T, public Loader_v2
    {
    public:
        TLoader_v2(std::ifstream&& fin)
        {
            m_File = std::move(fin);
            m_Func = ([&](char* mb, size_t size)
                {
                    m_File.read(mb, size);
                }
            );
        }

        ~TLoader_v2()
        {
            CloseFile();
        }

        virtual void CloseFile() override
        {
            m_File.close();
        }

        virtual void HandleDataBase(DataBase* db) override
        {
            Handle(*db);
        }
    protected:
        std::ifstream m_File;
    };

}
