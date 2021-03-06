#pragma once

#include <memory>

#include <nlohmann/json.hpp>

#include "Core/Base.h"

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
		Vec.resize(Js[DataName.data()].size());
		for (int i = 0; i < Js[DataName.data()].size(); i++)
		{
			Vec[i].SetJson(Js[DataName.data()][i]);
		}
	}

	template<typename T>
	void SetVector(LM::VectorRef<T>& Vec, nlohmann::basic_json<> Js, std::string_view DataName)
	{
		Vec.resize(Js[DataName.data()].size());
		for (int i = 0; i < Js[DataName.data()].size(); i++)
		{
			Vec[i] = std::make_shared<T>();
			Vec[i]->SetJson(Js[DataName.data()][i]);
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

	template<typename T>
	nlohmann::basic_json<> GetVector(const LM::VectorRef<T>& Vec)
	{
		nlohmann::basic_json<> Result;

		for (int i = 0; i < Vec.size(); i++)
		{
			Result[i] = Vec[i]->GetJson();
		}

		return Result;
	}

}
