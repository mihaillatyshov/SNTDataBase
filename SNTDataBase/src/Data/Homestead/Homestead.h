#pragma once

#include <string>

#include <nlohmann/json.hpp>

#include "Data/MembershipFee/MembershipFee.h"
#include "Data/Electricity/Electricity.h"
#include "UI/Table/TableElement.h"
#include "UI/Tabs/TabElement.h"
#include "HomesteadTabDS.h"
#include "HomesteadData.h"

namespace LM
{

	class Homestead : public TableElement, public TabElement
	{
	public:
		Homestead() = default;
		Homestead(Ref<const TabDataStruct> _TabDS);

		void Edit(Ref<const TabDataStruct> _TabDS);

		inline void SetNumber(std::string_view number)				{ m_Data.Number = number; }
		inline void SetSurname(std::string_view surname)			{ m_Data.Surname = surname; }
		inline void SetForename(std::string_view forename)			{ m_Data.Forename = forename; }
		inline void SetPatronymic(std::string_view patronymic)		{ m_Data.Patronymic = patronymic; }
		inline void SetPhoneNumber(std::string_view phoneNumber)	{ m_Data.PhoneNumber = phoneNumber; }
		inline void SetNote(std::string_view note)					{ m_Data.Note = note; }

		inline std::string_view     GetNumber()					const { return m_Data.Number; }
		inline std::string_view     GetSurname()				const { return m_Data.Surname; }
		inline std::string_view     GetForename()				const { return m_Data.Forename; }
		inline std::string_view     GetPatronymic()				const { return m_Data.Patronymic; }
		inline std::string_view     GetPhoneNumber()			const { return m_Data.PhoneNumber; }
		inline std::string_view     GetNote()					const { return m_Data.Note; }
		inline bool                 HasElectricityPrivilege()	const { return m_Data.ElectricityPrivilege.HasPrivilege; }
		inline bool                 GetAddMembershipFees()		const { return m_Data.AddMembershipFees; }
		inline const MembershipFee& GetMembershipFee()			const { return m_MembershipFee; }
		inline const Electricity&	GetElectricity()			const { return m_Electricity; }

		inline std::string&		GetNumberRef()			{ return m_Data.Number; }
		inline std::string&		GetSurnameRef()			{ return m_Data.Surname; }
		inline std::string&		GetForenameRef()		{ return m_Data.Forename; }
		inline std::string&		GetPatronymicRef()		{ return m_Data.Patronymic; }
		inline std::string&		GetPhoneNumberRef()		{ return m_Data.PhoneNumber; }
		inline std::string&		GetNoteRef()			{ return m_Data.Note; }
		inline MembershipFee&	GetMembershipFeeRef()	{ return m_MembershipFee; }

		void AddMembershipFeePayment(Ref<const TabDataStruct> _TabDS);
		void EditMembershipFeePayment(size_t _PayId, Ref<const TabDataStruct> _TabDS);
		void DeleteMembershipFeePayment(size_t _PayId);

		void AddElectricityAccrual(Ref<const TabDataStruct> _TabDS);
		void EditElectricityAccrual(size_t _AccId, Ref<const TabDataStruct> _TabDS);
		void DeleteElectricityAccrual(size_t _AccId);

		void AddElectricityPayment(Ref<const TabDataStruct> _TabDS);
		void EditElectricityPayment(size_t _PayId, Ref<const TabDataStruct> _TabDS);
		void DeleteElectricityPayment(size_t _PayId);

		virtual std::vector<std::function<void(void)>> GetDrawableColumns() const override;

		void FillDataStruct(Ref<TabDataStruct>& _TabDS) const override;

		nlohmann::basic_json<> GetJson() const;
		void SetJson(nlohmann::basic_json<> _JS);
	public:
		HomesteadData m_Data;

		MembershipFee m_MembershipFee;
		Electricity m_Electricity;
	};

}
