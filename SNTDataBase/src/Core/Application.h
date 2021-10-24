#pragma once

#include <string>
#include <array>
#include <tuple>
#include <unordered_map>

#include <GLFW/glfw3.h>

#include "Data/DataBase/DataBase.h"
#include "Windows/TabsColumns/TabsColumns.h"
#include "UI/Table/Table.h"


namespace LM
{

    class Application
    {
    public:

        template <typename T>
        struct CreateWindow
        {
            T Intermediate;
            Homestead* SelectedHomestead = nullptr;
            bool Create = false;
        
            void CreateNewIntermediate()
            {
                if (Create)
                    return;

                if (s_Application->m_HomesteadsTable->GetSelectedId() != -1 && s_Application->m_HomesteadsTable->GetSelectedId() < s_Application->m_DataBase->m_Homesteads.size())
                    SelectedHomestead = s_Application->m_DataBase->m_Homesteads[s_Application->m_HomesteadsTable->GetSelectedId()].get();
                else
                    SelectedHomestead = nullptr;

                Intermediate = T();
                Create = true;
            }
        };

        template <typename T>
        struct EditWindow
        {
            T* Editable = nullptr;
            T Intermediate;
            Homestead* SelectedHomestead = nullptr;
            bool Create = false;
            bool Dirty = false;
            
            virtual void CreateNewIntermediate(T* editable)
            {
                if (Create)
                    return;
                
                if (s_Application->m_HomesteadsTable->GetSelectedId() != -1 && s_Application->m_HomesteadsTable->GetSelectedId() < s_Application->m_DataBase->m_Homesteads.size())
                    SelectedHomestead = s_Application->m_DataBase->m_Homesteads[s_Application->m_HomesteadsTable->GetSelectedId()].get();
                else
                    SelectedHomestead = nullptr;

                Editable = editable;
                Intermediate = *editable;
                Create = true;
                Dirty = false;
            }
        };

        struct ColumnsInfo
        {
        public:
            //static inline std::vector<std::string> ColumFilePaths;
            static inline std::vector<ColumnsInfo*> ColumnsVector;
            static std::unordered_map<std::string, ColumnsInfo*> ColumnsMap;
            //{ "Homestead.column", "MembershipFee.column", "MembershipFeeOpeningBalanceColumn.column" };
            ColumnsInfo(const std::string &name, const std::vector<std::string> names) : Names(names)
            {
                Name = name;
                FileName = "./res/" + name + ".column";
                Widths = new float[names.size()];
                int WindowWidth, WindowHeight;
                glfwGetWindowSize(s_Application->m_Window, &WindowWidth, &WindowHeight);
                for (int i = 0; i < names.size(); i++)
                    Widths[i] = WindowWidth / names.size();
                //ColumFilePaths.push_back("Columns/" + fileName + ".column");
                ColumnsVector.push_back(this);
                ColumnsMap[name] = this;
            }
            ~ColumnsInfo() { delete Widths; }

            static ColumnsInfo* Add(const std::string& name, const std::vector<std::string> names) { return new ColumnsInfo(name, names); }
            static void Clear() { for (auto& columnInfo : ColumnsMap) { auto& [name, info] = columnInfo; delete info; } } 
            void DrawNames() {
                for (auto& name : Names)
                {
                    Widths[ImGui::GetColumnIndex()] =  ImGui::GetColumnWidth();
                    ImGui::Text(name.c_str());
                    ImGui::NextColumn();
                }
            }


            std::string Name;
            std::string FileName;
            float* Widths = nullptr;
            std::vector<std::string> Names;
            //int Size = 0;
            bool Update = false;
            bool WidthLoaded = false;
        };

        
    protected:
        void DrawMenuBar();
        void DrawFileMenu();
        void DrawWindowMenu();
        void DrawEditMenu();

        void DrawDataBase();

        void DrawHomestead();
        void DrawCreateHomestead();
        void DrawEditHomestead();
        void DrawDeleteHomestead(); // With button

        void DrawMembershipFee();
        void DrawCreateMembershipFeePayment();
        void DrawEditMembershipFeePayment();
        void DrawDeleteMembershipFeePayment(); // With button


        void DrawElectricity();
        void DrawCreateElectricity();
        void DrawEditElectricity();
        void DrawDeleteElectricity();

        void DrawElectricityPayment();
        void DrawCreateElectricityPayment();
        void DrawEditElectricityPayment();
        void DrawDeleteElectricityPayment();

        void DrawMembershipFeeOpeningBalance();
        void DrawElectricityOpeningBalance();

        void DrawCheckMembershipAccural();

        void DrawDropEvent();
        void DrawEditTariffOnLoad();

        void DrawRect(int colId, ColumnsInfo& column);
        void DrawRectBig(int colId, ColumnsInfo& column);
        bool InputTextString(std::string_view name, std::string& data, int itemWidth = 0, ImGuiInputTextFlags_ flag = ImGuiInputTextFlags_None);

        void RecalculateElectricityAccurals();


        void SaveDataBase();

        void SaveElectricityCSV();
        void SavePaymentsToDateCSV(const Date &date);
        void SaveMembershipToCSV(const Date &date);


        //void ImGuiEditString(std::string& str, const std::string &name, int id, int bufSize = 50); // , bool isEdit || if in Homestead class
        //void DrawHomesteadString(std::string& str, const std::string &name, int id);
        //void DrawSelectedHomestead(Homestead& homestead);
        //void HomesteadDrawNumber(Homestead& homestead, int id);
        //void HomesteadDrawSurname(Homestead& homestead, int id);

        void CreateBackup(std::string_view _Param = "");
        void LoadFile();

        void LoadColumns();
        void SaveColumns();
    public:
        Application(GLFWwindow* _Window);
        ~Application();

        void Run();

        static inline Application *s_Application;

        void OnResizeEvent();
        void OnDropEvent(const std::string& filename);
    protected:
        DataBase* m_DataBase = nullptr;

        GLFWwindow* m_Window;

        static inline std::string s_LoaderVersion;
        
        bool m_IsEdit;
        bool m_IsMembershipFeeOpeningBalance = false;
        bool m_IsElectricityOpeningBalance = false;

        bool m_OpenedMembershipAccural = true;    
        
        CreateWindow<Homestead> CreateHomestead;
        EditWindow<Homestead> EditHomestead;
        //int SelectedHomestead = -1;

        CreateWindow<Payment> CreateMembershipFeePayment;
        EditWindow<Payment> EditMembershipFeePayment;
        int SelectedMembershipFee = -1;

        CreateWindow<ElectricityAccural> CreateElectricityAccural;
        EditWindow<ElectricityAccural> EditElectricityAccural;
        int SelectedElectricityAccural = -1;
        
        CreateWindow<Payment> CreateElectricityPayment;
        EditWindow<Payment> EditElectricityPayment;
        int SelectedElectricityPayment = -1;

        std::vector<std::string> m_Tests;

        bool m_DropInfo = false;
        std::vector<std::tuple<std::string, ElectricityAccural>> m_DropInfoAccurals;
        bool m_EditTariffOnLoad = false;
        ElectricityAccuralCosts m_ElectricityAccuralCostsIntermediate;

        bool m_ShowDemoWindow = false;

        TabsColumns m_TabsColumns;

        std::shared_ptr<Table> m_HomesteadsTable;
    };

}
