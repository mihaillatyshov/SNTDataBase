#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include "../Versions.h"
#include "Unloader_v2.h"
#include "ClearLoaderUnloader.h"

namespace LM
{
    template <class T>
    class TUnloader_v2 : public T, public Unloader_v2
    {
    public:
        TUnloader_v2(std::ofstream&& fout)
        {
            m_File = std::move(fout);
            m_Func = ([&](char* mb, size_t size)
                {
                    m_File.write(mb, size);
                }
            );
        }

        ~TUnloader_v2()
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
        std::ofstream m_File;
    };

}
