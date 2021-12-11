#pragma once

#include <nlohmann/json.hpp>

namespace LM
{

	class Percent
	{
	public:
		Percent(uint64_t _Percent) : m_Val(_Percent) { }
		bool DrawEdit(std::string_view _Label = "", const uint64_t _Min = 0, const uint64_t _Max = 100);
		inline uint64_t Get() const { return m_Val; }

		nlohmann::basic_json<> GetJson() const;
		void SetJson(nlohmann::basic_json<> _JS);
	protected:
		uint64_t m_Val;
	};


}