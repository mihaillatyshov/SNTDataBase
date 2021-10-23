#pragma once

#include <vector>

#include <nlohmann/json.hpp>

namespace LM
{

    class Date
    {
    public:
        Date() = default;

        void Draw();
        bool DrawEdit();

        int GetDaysInMonth(int _Month, int _Year) const;

        const std::string GetString() const;

        void FixDate();

        nlohmann::basic_json<> GetJson() const;
        void SetJson(nlohmann::basic_json<> _JS);

        bool operator< (const Date& _Other) const;
        bool operator> (const Date& _Other) const;
        bool operator<=(const Date& _Other) const;
        bool operator>=(const Date& _Other) const;
        bool operator==(const Date& _Other) const;
        bool operator!=(const Date& _Other) const;
    public:
        int m_Year = 2019;
        int m_Month = 1;
        int m_Day = 1;
    };

}
