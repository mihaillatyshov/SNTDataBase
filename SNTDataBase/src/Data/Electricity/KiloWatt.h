#pragma once

#include <nlohmann/json.hpp>

namespace LM
{

    struct KiloWatt
    {
    public:
        KiloWatt(int64_t _KiloWatts = 0, int64_t _Watts = 0)
        {
            m_Watt = _KiloWatts * 1000 + _Watts;
        }

        void Draw() const;
        bool DrawEdit(std::string_view _FieldName);

        nlohmann::basic_json<> GetJson() const;
        void SetJson(nlohmann::basic_json<> _JS);

    public:
        const KiloWatt operator+(const KiloWatt& _Other) const;
        const KiloWatt operator-(const KiloWatt& _Other) const;
        friend std::ostream& operator<< (std::ostream& _Out, const KiloWatt _KiloWatt);

    public:
        int64_t m_Watt = 0;
    };

}
