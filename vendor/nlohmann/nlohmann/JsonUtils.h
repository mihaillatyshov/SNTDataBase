#pragma once

#include <nlohmann/json.hpp>

namespace nlohmann
{
    template<typename T>
    void SetValue(T& Value, nlohmann::basic_json<> Js, std::string_view DataName)
    {
        if (!Js.is_object())
            return;

        if (!Js[DataName.data()].empty())
            Value = Js[DataName.data()];
    }

    template<typename T>
    void SetVector(std::vector<T>& Vec, nlohmann::basic_json<> Js, std::string_view DataName)
    {
        Vec.resize(         Js[DataName.data()].size());
        for (int i = 0; i < Js[DataName.data()].size(); i++)
        {
            Vec[i].SetJson(Js[DataName.data()][i]);
        }
    }

    template<typename T>
    nlohmann::basic_json<> GetVector(const std::vector<T>& Vec)
    {
        nlohmann::basic_json<> Result;

        for (int i = 0; i < Vec.size(); i++)
        {
            Result[i] = Vec[i].GetJson();
        }

        return Result;
    }
}
