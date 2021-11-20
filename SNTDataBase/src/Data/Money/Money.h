#pragma once

#include <nlohmann/json.hpp>

namespace LM
{

	class Money
	{
	public:
		Money() = default;
		Money(int64_t _Rub, int64_t _Cop = 0);

		void Draw() const;
		void DrawAbs() const;
		bool DrawEdit(std::string_view _FieldName = "", float _ItemWidth = 250.0f);
		inline int64_t GetRubs() const { return abs(m_Amount / 100); }
		inline int64_t GetCops() const { return abs(m_Amount % 100); }

		nlohmann::basic_json<> GetJson() const;
		void SetJson(nlohmann::basic_json<> _JS);
	public:
		bool operator<(int _Val) const;
		const Money operator+(const Money& _Other) const;
		const Money operator-(const Money& _Other) const;
		Money& operator+=(const Money& _Other);
		Money& operator-=(const Money& _Other);
		friend std::ostream& operator<< (std::ostream& _Out, const Money _Money);
	public:
		int64_t m_Amount = 0;
	};

}
