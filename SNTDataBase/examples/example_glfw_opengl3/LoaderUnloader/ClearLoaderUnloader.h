#pragma once

#include <algorithm>
#include <fstream>
#include <sstream>
#include <type_traits>
#include <functional>

namespace LM
{

    class ClearLoaderUnloader
    {
    public:
        ClearLoaderUnloader() = default;
        ~ClearLoaderUnloader()
        {
            //CloseFile();
        }

        virtual void CloseFile() = 0;
    protected:
        template <class T>
        typename std::enable_if<
            std::is_same<T, char>::value ||
            std::is_same<T, bool>::value ||
            std::is_same<T, int>::value ||
            std::is_same<T, long long>::value ||
            std::is_same<T, double>::value ||
            std::is_same<T, float>::value>::type
            Handle(T& v)
        {
            char* mb = (char*)&v;
            m_Func(mb, sizeof(T));
        }

        void Handle(std::string& v)
        {
            int size = v.size();
            Handle(size);
            v.resize(size);
            char* mb = (char*)v.data();
            m_Func(mb, size);
        }

        template<class T>
        void Handle(std::vector<T>& v)
        {
            int size = v.size();
            Handle(size);
            v.resize(size);
            for (int i = 0; i < size; i++)
            {
                Handle(v[i]);
            }
        }

        virtual void Handle(DataBase& v)
        {
            Handle(v.m_Homestead);

            Handle(OpeningBalance::s_Date);
            Handle(MembershipFee::s_Accural);
        }

        virtual void Handle(Accural& v)
        {
            Handle(v.m_Date);
            Handle(v.m_Money.m_Amount);
        }

        virtual void Handle(Homestead& v)
        {
            Handle(v.m_Number);
            Handle(v.m_Surname);
            Handle(v.m_Forename);
            Handle(v.m_Patronymic);
            Handle(v.m_PhoneNumber);
            Handle(v.m_Note);

            Handle(v.m_HasBenefits);
            Handle(v.m_AddMembershipFees);

            Handle(v.m_MembershipFee);
            Handle(v.m_Electricity);
        }

        virtual void Handle(MembershipFee& v)
        {
            Handle(v.m_Debt.m_Amount);
            Handle(v.m_OpeningBalance.m_Money.m_Amount);

            Handle(v.m_Payments);
        }

        virtual void Handle(Payment& v)
        {
            Handle(v.m_Amount.m_Amount);
            Handle(v.m_Date);
            Handle(v.m_FormOfPayment);
            Handle(v.m_DocumentNumber);
        }

        virtual void Handle(Date& v)
        {
            Handle(v.Year);
            Handle(v.Month);
            Handle(v.Day);
        }

        virtual void Handle(Electricity& v)
        {
            Handle(v.m_All.m_Amount);
            Handle(v.m_OpeningBalance.m_Amount);

            Handle(v.m_ElectricityPayment);
            Handle(v.m_ElectricityAccural);
        }

        virtual void Handle(ElectricityAccural& v)
        {
            Handle(v.m_Date);

            Handle(v.m_Day.m_Watt);
            Handle(v.m_Night.m_Watt);
            Handle(v.m_AllMonth.m_Watt);

            Handle(v.m_Costs.m_DayCost.m_Amount);
            Handle(v.m_Costs.m_NightCost.m_Amount);

            Handle(v.m_Costs.m_ConstantAccurals);

        }

        virtual void Handle(std::pair<std::string, Money>& v)
        {
            Handle(v.first);
            Handle(v.second.m_Amount);
        }

    protected:
        std::function<void(char*, size_t)> m_Func;
    };

    class ClearLoaderUnloader_0_0_0 : public ClearLoaderUnloader
    {

    };

    class ClearLoaderUnloader_0_0_1 : public ClearLoaderUnloader
    {

    };

    class ClearLoaderUnloader_0_0_2 : public ClearLoaderUnloader
    {

    };

}
