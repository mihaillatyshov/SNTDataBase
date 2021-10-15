#include "Loader_0_0_2.h"

#include <sstream>
#include <iostream>

namespace LM
{

    // IMPLEMENT THIS!!!
    DataBase* Loader_0_0_2::LoadDataBase(std::ifstream &fin)
    {
        std::string line;
        if (std::getline(fin, line))
        {
            int HomesteadCount = 0;
            std::istringstream iss (line);
            iss >> HomesteadCount;                          // Get Homestead count;
    
            if (iss.fail()) 
                return new DataBase;                        // something wrong happened
    
            std::cout << HomesteadCount << std::endl;
            DataBase* database = new DataBase;
            for (int i = 0; i < HomesteadCount; i++)
            {
                Homestead homestead = *LoadHomestead(fin);   // Load Homesteads
                database->Push(homestead);
            }

            // Load static data
            if (std::getline(fin, line))
                OpeningBalance::s_Date = GetDate(line);

            if (std::getline(fin, line))
                MembershipFee::s_Accural = std::vector<Accural>(GetInt(line));
            for (auto& accural : MembershipFee::s_Accural)
            {
                if (std::getline(fin, line))
                    accural.m_Date = GetDate(line);
                if (std::getline(fin, line))
                    accural.m_Money.m_Amount = GetInt(line);
            }


                               
            return database;
        }
    
        return new DataBase;
    }
    
    Homestead* Loader_0_0_2::LoadHomestead(std::ifstream &fin)
    {
        Homestead *homestead = new Homestead;
        std::string line;
        if (std::getline(fin, line))
            homestead->SetNumber(line);
    
        if (std::getline(fin, line))
            homestead->SetSurname(line);
    
        if (std::getline(fin, line))
            homestead->SetForename(line);
    
        if (std::getline(fin, line))
            homestead->SetPatronymic(line);
    
        if (std::getline(fin, line))
            homestead->SetPhoneNumber(line);
    
        if (std::getline(fin, line))
            homestead->SetNote(line);

        if (std::getline(fin, line))
            homestead->m_HasBenefits = GetBool(line);

        if (std::getline(fin, line))
            homestead->m_AddMembershipFees = GetBool(line);

        homestead->m_MembershipFee = *LoadMembershipFee(fin);
        homestead->m_Electricity   = *LoadElectricity(fin);

        return homestead;
    }


    MembershipFee* Loader_0_0_2::LoadMembershipFee(std::ifstream& fin)
    {
        MembershipFee *membershipFee = new MembershipFee;

        std::string line;


        if (std::getline(fin, line))
            membershipFee->m_Debt.m_Amount = GetInt(line);
        if (std::getline(fin, line))
            membershipFee->m_OpeningBalance.m_Money.m_Amount = GetInt(line);

        if (std::getline(fin, line))
            membershipFee->m_Payments = std::vector<Payment> (GetInt(line));
        for (auto& payment : membershipFee->m_Payments)
        {
            if (std::getline(fin, line))
                payment.m_Amount.m_Amount = GetInt(line);
            if (std::getline(fin, line))
                payment.m_Date = GetDate(line);
            if (std::getline(fin, line))
                payment.m_FormOfPayment = GetInt(line);
            if (std::getline(fin, line))
                payment.m_DocumentNumber = GetInt(line);
        }

        return membershipFee;
    }


    Electricity* Loader_0_0_2::LoadElectricity(std::ifstream& fin)
    {
        Electricity *electricity = new Electricity;

        std::string line;

        if (std::getline(fin, line))
            electricity->m_All.m_Amount = GetInt(line);
        if (std::getline(fin, line))
            electricity->m_OpeningBalance.m_Amount = GetInt(line);

        if (std::getline(fin, line))
            electricity->m_ElectricityPayment = std::vector<Payment> (GetInt(line));
        for (auto& payment : electricity->m_ElectricityPayment)
        {
            if (std::getline(fin, line))
                payment.m_Amount.m_Amount = GetInt(line);
            if (std::getline(fin, line))
                payment.m_Date = GetDate(line);
            if (std::getline(fin, line))
                payment.m_FormOfPayment = GetInt(line);
            if (std::getline(fin, line))
                payment.m_DocumentNumber = GetInt(line);
        }


        if (std::getline(fin, line))
            electricity->m_ElectricityAccural = std::vector<ElectricityAccural> (GetInt(line));
        for (auto& accural : electricity->m_ElectricityAccural)
        {
            if (std::getline(fin, line))
                accural.m_Date = GetDate(line);
            if (std::getline(fin, line))
                accural.m_Day.m_Watt = GetInt(line);
            if (std::getline(fin, line))
                accural.m_Night.m_Watt = GetInt(line);
            if (std::getline(fin, line))
                accural.m_AllMonth.m_Watt = GetInt(line);

            if (std::getline(fin, line))
                accural.m_Costs.m_DayCost.m_Amount = GetInt(line);
            if (std::getline(fin, line))
                accural.m_Costs.m_NightCost.m_Amount = GetInt(line);

            if (std::getline(fin, line))
                accural.m_Costs.m_ConstantAccurals = std::vector<std::pair<std::string, Money>> (GetInt(line));
            for (auto& [name, money] : accural.m_Costs.m_ConstantAccurals)
            {
                if (std::getline(fin, line))
                    name = line;
                if (std::getline(fin, line))
                    money.m_Amount = GetInt(line);
            }
        }


        return electricity;
    }


    int Loader_0_0_2::GetInt(std::string_view str)
    {
        int temp = 0;
        std::istringstream iss (str.data());
        iss >> temp;                          
    
        if (iss.fail())
        {
            std::cout << "Fail to load int from string: " << str << std::endl;
            return 0;
        }
        return temp;
    }


    int Loader_0_0_2::GetBool(std::string_view str)
    {
        bool temp = false;
        std::istringstream iss (str.data());
        iss >> temp;                          
    
        if (iss.fail())
        {
            std::cout << "Fail to load bool from string: " << str << std::endl;
            return 0;
        }
        return temp;
    }


    Date Loader_0_0_2::GetDate(std::string_view str)
    {
        Date temp;
        char trash;
        std::istringstream iss (str.data());

        iss >> temp.Day >> trash >> temp.Month >> trash >> temp.Year;
        if (iss.fail())
        {
            std::cout << "Fail to load date from string: " << str << std::endl;
            return Date();
        }

        temp.FixDate();
        std::cout << "Test date Load: " << temp.GetStringDate() << std::endl;
        return temp;
    }



}
