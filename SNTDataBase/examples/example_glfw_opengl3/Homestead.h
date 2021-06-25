#pragma once

#include <string>

#include "MembershipFee.h"
#include "Accural.h"
#include "Electricity.h"

namespace LM
{

    class Homestead
    {
    public:
        std::string m_Number;
        std::string m_Surname;
        std::string m_Forename;
        std::string m_Patronymic;
        std::string m_PhoneNumber;
        std::string m_Note;

        bool m_HasBenefits = false;
        bool m_AddMembershipFees = true;

        MembershipFee m_MembershipFee;
        Electricity m_Electricity;

        inline void SetNumber(std::string_view number) { m_Number = number; }
        inline void SetSurname(std::string_view surname) { m_Surname = surname; }
        inline void SetForename(std::string_view forename) { m_Forename = forename; }
        inline void SetPatronymic(std::string_view patronymic) { m_Patronymic = patronymic; }
        inline void SetPhoneNumber(std::string_view phoneNumber) { m_PhoneNumber = phoneNumber; }
        inline void SetNote(std::string_view note) { m_Note = note; }

        inline std::string_view GetNumber() { return m_Number; }
        inline std::string_view GetSurname() { return m_Surname; }
        inline std::string_view GetForename() { return m_Forename; }
        inline std::string_view GetPatronymic() { return m_Patronymic; }
        inline std::string_view GetPhoneNumber() { return m_PhoneNumber; }
        inline std::string_view GetNote() { return m_Note; }

        inline std::string& GetNumberRef() { return m_Number; }
        inline std::string& GetSurnameRef() { return m_Surname; }
        inline std::string& GetForenameRef() { return m_Forename; }
        inline std::string& GetPatronymicRef() { return m_Patronymic; }
        inline std::string& GetPhoneNumberRef() { return m_PhoneNumber; }
        inline std::string& GetNoteRef() { return m_Note; }
        inline MembershipFee& GetMembershipFeeRef() { return m_MembershipFee; }

        //void DrawNumber(int id, bool isEdit);
    };

}
