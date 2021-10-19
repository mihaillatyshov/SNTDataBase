#pragma once

#include <nlohmann/json.hpp>

namespace LM
{

    class Money
    {
    public:
        Money() = default;
        Money(int64_t _Rub, int64_t _Cop = 0);

        nlohmann::basic_json<> GetJson() const;
        void SetJson(nlohmann::basic_json<> _JS);

        void Draw();
        void DrawAbs();
        bool DrawEdit(std::string_view _FieldName);
        inline int64_t GetRubs() { return abs(m_Amount / 100); }
        inline int64_t GetCops() { return abs(m_Amount % 100); }

    public:
        bool operator<(int _Val);
        const Money operator+(const Money& _Other) const;
        const Money operator-(const Money& _Other) const;
        Money& operator+=(const Money& _Other);
        Money& operator-=(const Money& _Other);
        friend std::ostream& operator<< (std::ostream& _Out, const Money _Money);
    public:
        int64_t m_Amount = 0;
    };

}
