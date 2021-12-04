#pragma once

#include <nlohmann/json.hpp>

namespace LM
{

	class KiloWatt
	{
	public:
		KiloWatt(int64_t _KiloWatts = 0, int64_t _Watts = 0);
		KiloWatt(std::string _Str);

		void Draw() const;
		bool DrawEdit(std::string_view _FieldName);

		int64_t Get() const { return m_Watt; }

		nlohmann::basic_json<> GetJson() const;
		void SetJson(nlohmann::basic_json<> _JS);

	public:
		const KiloWatt operator+(const KiloWatt& _Other) const;
		const KiloWatt operator-(const KiloWatt& _Other) const;
		friend std::ostream& operator<<(std::ostream& _Out, const KiloWatt& _KiloWatt);

	protected:
		int64_t m_Watt = 0;
	};

}
