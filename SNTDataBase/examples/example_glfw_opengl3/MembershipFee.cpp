#include "MembershipFee.h"
#include <time.h>

#include <algorithm>
#include <iostream>

namespace LM
{

    void MembershipFee::DrawActivePayment(Payment &payment, int id, bool isEdit)
    {
        if (payment.DrawPaymentBegin())
        {
            m_Debt.m_Amount += payment.GetAmountRef().m_Amount;
            payment.DrawPayment(isEdit);
            m_Debt.m_Amount -= payment.GetAmountRef().m_Amount;

            if (isEdit)
            {
                if (payment.DrawDeleteButton())
                {
                    m_Debt.m_Amount += payment.GetAmountRef().m_Amount;
                    ImGui::OpenPopup("Delete?");
                    IdToDetele = id;
                    //payment.SetActive(false);
                }
            }
            payment.DrawPaymentEnd();
        }
    }


    void MembershipFee::DrawInactivePayment(Payment &payment, int id)
    {
        if (payment.DrawPaymentBegin())
        {
            payment.DrawPayment(true);

            if (payment.DrawRestoreButton())
            {
                m_Debt.m_Amount -= payment.GetAmountRef().m_Amount;
                //payment.SetActive(true);
            }

            bool NeedToDelete = payment.DrawDeleteButton();

            //if (payment.DrawDeleteButton())
            //{
            //    ImGui::OpenPopup("Delete?");
            //    IdToDetele = id;
            //}
            
            //DrawDeletePayment();

            payment.DrawPaymentEnd();

            if (NeedToDelete)
                m_Payments.erase(m_Payments.begin() + id);
        }
    }

    
    void MembershipFee::DrawMembershipFee(bool IsEdit)
    {
        if (ImGui::CollapsingHeader(u8"Членские взносы"))
        {
            time_t t;
            struct tm *t_m;
            t = time(NULL);
            t_m = localtime(&t);
            //std::string date = std::to_string(1900 + t_m->tm_year) + "-" + std::to_string(1 + t_m->tm_mon) + "-" + std::to_string(t_m->tm_mday);

            if (m_Debt.m_Amount < 0)
                ImGui::Text(u8"Переплата на %02d.%02d.%d: %d.%02d руб", t_m->tm_mday, 1 + t_m->tm_mon, 1900 + t_m->tm_year, abs(m_Debt.m_Amount / 100), m_Debt.m_Amount % 100);
            else
                ImGui::Text(u8"Долг на %02d.%02d.%d: %d.%02d руб", t_m->tm_mday, 1 + t_m->tm_mon, 1900 + t_m->tm_year, m_Debt.m_Amount / 100, m_Debt.m_Amount % 100);

            ImGui::PushID("Active");
            for (int i = 0, id = 0; i < m_Payments.size(); i++)
            {
                //if (m_Payments[i].IsActive())
                {
                    ImGui::PushID(i);
                    //ImGui::PushID((void*)(&m_Payments[i]));

                    //if (m_Payments[i].DrawPaymentBegin())
                    //{
                    //    m_Debt.m_Amount += m_Payments[i].GetAmountRef().m_Amount;
                    //    m_Payments[i].DrawPayment(IsEdit);
                    //    m_Debt.m_Amount -= m_Payments[i].GetAmountRef().m_Amount;
                    //
                    //    if (IsEdit)
                    //    {
                    //        if (m_Payments[i].DrawDeleteButton())
                    //        {
                    //            m_Payments[i].SetActive(false);
                    //
                    //        }
                    //    }
                    //    m_Payments[i].DrawPaymentEnd();
                    //
                    //    //if (NeedToDelete)
                    //    //    m_Payments.erase(m_Payments.begin() + i);
                    //}
                    DrawActivePayment(m_Payments[i], i, IsEdit);
                    ImGui::PopID();
                    id++;
                }
            }
            ImGui::PopID();

            if (IsEdit)
            {
                ImGui::PushID("Inactive");
                for (int i = 0, id = 0; i < m_Payments.size(); i++)
                {
                    //if (!m_Payments[i].IsActive())
                    {
                        //ImGui::PushID(i);
                        ImGui::PushID(i);

                        //if (m_Payments[i].DrawPaymentBegin())
                        //{
                        //    m_Debt.m_Amount += m_Payments[i].GetAmountRef().m_Amount;
                        //    m_Payments[i].DrawPayment(IsEdit);
                        //    m_Debt.m_Amount -= m_Payments[i].GetAmountRef().m_Amount;
                        //
                        //    if (m_Payments[i].DrawRestoreButton())
                        //        m_Payments[i].SetActive(true);
                        //
                        //    bool NeedToDelete = false;
                        //    if (IsEdit)
                        //        NeedToDelete = m_Payments[i].DrawDeleteButton();
                        //
                        //    m_Payments[i].DrawPaymentEnd();
                        //
                        //    if (NeedToDelete)
                        //        m_Payments.erase(m_Payments.begin() + i);
                        //}
                        DrawInactivePayment(m_Payments[i], i);
                        ImGui::PopID();
                        id++;
                    }
                }
                ImGui::PopID();
                
                if (ImGui::Button(u8"Добавить платеж"))
                {
                    m_Payments.emplace_back();
                }
            }
        }

        
    }


    void MembershipFee::DrawDeletePayment()
    {
        if (ImGui::BeginPopupModal("Delete?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::Text("All those beautiful files will be deleted.\nThis operation cannot be undone!\n\n");
            ImGui::Separator();

            //static int dummy_i = 0;
            //ImGui::Combo("Combo", &dummy_i, "Delete\0Delete harder\0");

            static bool dont_ask_me_next_time = false;
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
            ImGui::Checkbox("Don't ask me next time", &dont_ask_me_next_time);
            ImGui::PopStyleVar();

            if (ImGui::Button("OK", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
            ImGui::SetItemDefaultFocus();
            ImGui::SameLine();
            if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
            ImGui::EndPopup();
        }
    }


    void MembershipFee::SortPayments()
    {
        std::sort(m_Payments.begin(), m_Payments.end(), [](const Payment &first, const Payment &second)
            {
                long long DateFirst, DateSecond;
                char lable1[20] { 0 };
                sprintf(lable1, "%d%02d%02d", first.GetDate().Year, first.GetDate().Month, first.GetDate().Day);
                sscanf(lable1, "%lld", &DateFirst);
                //std::cout << DateFirst << " ";
                char lable2[20] { 0 };
                sprintf(lable2, "%d%02d%02d", second.GetDate().Year, second.GetDate().Month, second.GetDate().Day);
                sscanf(lable2, "%lld", &DateSecond);
                //std::cout << DateSecond << std::endl;
                return DateFirst > DateSecond;
            });
    }
}
