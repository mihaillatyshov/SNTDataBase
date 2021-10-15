#include "Unloader.h"

#include "Unloader_0_0_0.h"
#include "Unloader_0_0_1.h"
#include "Unloader_0_0_2.h"
#include "Versions.h"

namespace LM
{

    Unloader* Unloader::Create()
    {
        std::string version = s_Versions.back();
        if (version == "0.0.0")
            return new Unloader_0_0_0();
        if (version == "0.0.1")
            return new Unloader_0_0_1();
        if (version == "0.0.2")
            return new Unloader_0_0_2();

         std::cout << "Wrong Version!" << version << std::endl;
         return nullptr;
    }

}
