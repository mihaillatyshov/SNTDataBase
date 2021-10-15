#include "Unloader_0_0_1.h"

#include <iostream>
#include <fstream>

namespace LM
{

    bool Unloader_0_0_1::SaveDataBase(DataBase* database)
    {
        std::ofstream fout;
	    fout.open("DataBase.db");
	
	    if (!fout.is_open())
	    {
		    std::cout << "Error: Can't Save file!" << std::endl;
            return false;
	    }
		std::cout << "File ok! Saving file..." << std::endl;
		fout << GetVersion() << std::endl;
        fout << database->GetHomesteads().size() << std::endl;
        for (auto& homestead : database->GetHomesteads())
        {
            SaveHomestead(homestead, fout);
        }

        SaveStaticDate(database, fout);


        std::cout << "File Saved!" << std::endl;
	    fout.close();

        return true;
    }


    void Unloader_0_0_1::SaveHomestead(Homestead &homestead, std::ofstream &fout)
    {
        fout << homestead.GetNumber() << std::endl;

        fout << homestead.GetSurname() << std::endl;

        fout << homestead.GetForename() << std::endl;

        fout << homestead.GetPatronymic() << std::endl;

        fout << homestead.GetPhoneNumber() << std::endl;

        fout << homestead.GetNote() << std::endl;


        SaveMembershipFee(homestead.m_MembershipFee, fout);
        SaveElectricity(homestead.m_Electricity, fout);

    }


    void Unloader_0_0_1::SaveMembershipFee(MembershipFee &membershipFee, std::ofstream &fout)
    {
        fout << membershipFee.m_Debt.m_Amount << std::endl;
        fout << membershipFee.m_OpeningBalance.m_Money.m_Amount << std::endl;

        fout << membershipFee.m_Payments.size() << std::endl;
        for (auto& payment : membershipFee.m_Payments)
        {
            fout << payment.m_Amount.m_Amount << std::endl;
            fout << payment.m_Date.GetStringDate() << std::endl;
            fout << payment.m_FormOfPayment << std::endl;
            fout << payment.m_DocumentNumber << std::endl;
        }


    }


    void Unloader_0_0_1::SaveElectricity(Electricity& electricity, std::ofstream& fout)
    {
        fout << electricity.m_All.m_Amount << std::endl;
        fout << electricity.m_OpeningBalance.m_Amount << std::endl;

        fout << electricity.m_ElectricityPayment.size() << std::endl;
        for (auto& payment : electricity.m_ElectricityPayment)
        {
            fout << payment.m_Amount.m_Amount << std::endl;
            fout << payment.m_Date.GetStringDate() << std::endl;
            fout << payment.m_FormOfPayment << std::endl;
            fout << payment.m_DocumentNumber << std::endl;
        }

        fout << electricity.m_ElectricityAccural.size() << std::endl;
        for (auto& accural : electricity.m_ElectricityAccural)
        {
            fout << accural.m_Date.GetStringDate() << std::endl;
            fout << accural.m_Day.m_Watt << std::endl;
            fout << accural.m_Night.m_Watt << std::endl;
            fout << accural.m_AllMonth.m_Watt << std::endl;

            fout << accural.m_Costs.m_DayCost.m_Amount << std::endl;
            fout << accural.m_Costs.m_NightCost.m_Amount << std::endl;

            fout << accural.m_Costs.m_ConstantAccurals.size() << std::endl;
            for (auto& [name, cost] : accural.m_Costs.m_ConstantAccurals)
            {
                fout << name << std::endl;
                fout << cost.m_Amount << std::endl;
            }
        }

        
    }


    void Unloader_0_0_1::SaveStaticDate(DataBase* database, std::ofstream& fout)
    {
        fout << OpeningBalance::s_Date.GetStringDate() << std::endl;

        fout << MembershipFee::s_Accural.size() << std::endl;
        for (auto& accural : MembershipFee::s_Accural)
        {
            fout << accural.m_Date.GetStringDate() << std::endl;
            fout << accural.m_Money.m_Amount << std::endl;
        }
    }


}
