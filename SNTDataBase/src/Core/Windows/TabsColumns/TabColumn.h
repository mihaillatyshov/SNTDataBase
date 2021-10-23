#pragma once

#include <string>
#include <nlohmann/json.hpp>

namespace LM
{

    class TabColumn
    {
    public:
        TabColumn(std::string_view _Name, int _Width = 100);

        void SetWidth(int _Width);
        inline int GetWidth() { return m_Width; }
        inline std::string_view GetName() const { return m_Name; }
        void Print() const;

        nlohmann::basic_json<> GetJson() const;
        void SetJson(nlohmann::basic_json<> _JS);
    protected:
        std::string m_Name;
        int m_Width;
    };

}
