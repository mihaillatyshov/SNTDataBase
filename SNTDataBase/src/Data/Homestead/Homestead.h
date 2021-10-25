#pragma once

#include <string>
#include <nlohmann/json.hpp>

#include "../MembershipFee/MembershipFee.h"
#include "../Accural/Accural.h"
#include "../Electricity/Electricity.h"
#include "UI/Table/TableElement.h"

namespace LM
{

	class Homestead : public TableElement
	{
	public:
		inline void SetNumber(std::string_view number)				{ m_Number = number; }
		inline void SetSurname(std::string_view surname)			{ m_Surname = surname; }
		inline void SetForename(std::string_view forename)			{ m_Forename = forename; }
		inline void SetPatronymic(std::string_view patronymic)		{ m_Patronymic = patronymic; }
		inline void SetPhoneNumber(std::string_view phoneNumber)	{ m_PhoneNumber = phoneNumber; }
		inline void SetNote(std::string_view note)					{ m_Note = note; }

		inline std::string_view     GetNumber()             const { return m_Number; }
		inline std::string_view     GetSurname()            const { return m_Surname; }
		inline std::string_view     GetForename()           const { return m_Forename; }
		inline std::string_view     GetPatronymic()         const { return m_Patronymic; }
		inline std::string_view     GetPhoneNumber()        const { return m_PhoneNumber; }
		inline std::string_view     GetNote()               const { return m_Note; }
		inline bool                 GetHasBenefits()        const { return m_HasBenefits; }
		inline bool                 GetAddMembershipFees()  const { return m_AddMembershipFees; }
		inline const MembershipFee& GetMembershipFee()      const { return m_MembershipFee; }
		inline const Electricity&	GetElectricity()        const { return m_Electricity; }

		inline std::string&		GetNumberRef()			{ return m_Number; }
		inline std::string&		GetSurnameRef()			{ return m_Surname; }
		inline std::string&		GetForenameRef()		{ return m_Forename; }
		inline std::string&		GetPatronymicRef()		{ return m_Patronymic; }
		inline std::string&		GetPhoneNumberRef()		{ return m_PhoneNumber; }
		inline std::string&		GetNoteRef()			{ return m_Note; }
		inline MembershipFee&	GetMembershipFeeRef()	{ return m_MembershipFee; }

		virtual std::vector<std::function<void(void)>> GetDrawableColumns() const override;

		nlohmann::basic_json<> GetJson() const;
		void SetJson(nlohmann::basic_json<> js);
	public:
		std::string m_Number		= "";
		std::string m_Surname		= "";
		std::string m_Forename		= "";
		std::string m_Patronymic	= "";
		std::string m_PhoneNumber	= "";
		std::string m_Note			= "";

		bool m_HasBenefits			= false;
		bool m_AddMembershipFees	= true;

		MembershipFee m_MembershipFee;
		Electricity m_Electricity;
	};

}
