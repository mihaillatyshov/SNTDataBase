#pragma once

#include <nlohmann/json.hpp>

#include "Data/Electricity/KiloWatt.h"
#include "Data/Percent/Percent.h"

namespace LM
{

	class Money
	{
	public:
		Money() = default;
		Money(int64_t _Rub, int64_t _Cop = 0);

		void Draw() const;
		void DrawAbs() const;
		bool DrawEdit(std::string_view _Label = "", float _Width = 250.0f);

		std::string GetString() const;
		inline int64_t Get()		const { return m_Amount; }
		inline int64_t GetRubs()	const { return abs(m_Amount / 100); }
		inline int64_t GetCops()	const { return abs(m_Amount % 100); }

		nlohmann::basic_json<> GetJson() const;
		void SetJson(nlohmann::basic_json<> _JS);
	public:
		bool operator<(int _Val) const;
		bool operator==(const Money& _Other) const;
		bool operator!=(const Money& _Other) const;
		const Money operator+(const Money& _Other) const;
		const Money operator-(const Money& _Other) const;
		Money& operator+=(const Money& _Other);
		Money& operator-=(const Money& _Other);
		const Money operator*(const KiloWatt& _Other) const;
		const Money operator*(const Percent& _Other) const;
		friend std::ostream& operator<<(std::ostream& _Out, const Money& _Money);
	protected:
		int64_t m_Amount = 0;
	};

}
