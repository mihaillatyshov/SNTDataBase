#pragma once

#include <string>
#include <array>
#include <tuple>
#include <unordered_map>

#include <GLFW/glfw3.h>

#include "Data/DataBase/DataBase.h"
#include "UI/Table/Table.h"
#include "UI/Table/SimpleTable.h"
#include "UI/Tabs/TabsCollection/TabsCollection.h"
#include "Base.h"


namespace LM
{

	class Application
	{
	public:

		struct ColumnsInfo
		{
		public:
			//static inline std::vector<std::string> ColumFilePaths;
			static inline std::vector<ColumnsInfo*> ColumnsVector;
			static std::unordered_map<std::string, ColumnsInfo*> ColumnsMap;
			//{ "Homestead.column", "MembershipFee.column", "MembershipFeeOpeningBalanceColumn.column" };
			ColumnsInfo(const std::string& name, const std::vector<std::string> names) : Names(names)
			{
				Name = name;
				FileName = "./res/" + name + ".column";
				Widths = new float[names.size()];
				int WindowWidth, WindowHeight;
				glfwGetWindowSize(s_Application->m_Window, &WindowWidth, &WindowHeight);
				for (size_t i = 0; i < names.size(); i++)
					Widths[i] = (float)WindowWidth / (float)names.size();
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
					Widths[ImGui::GetColumnIndex()] = ImGui::GetColumnWidth();
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

		void DrawHomesteads();
		void DrawMembershipFeePayments();
		void DrawElectricityAccruals();
		void DrawElectricityPayments();

		void DrawMembershipFeeOpeningBalance();
		void DrawElectricityOpeningBalance();

		void DrawCheckMembershipAccrual();

		void DrawDropEvent();
		void DrawEditTariffOnLoad();

		void DrawRect(int colId, ColumnsInfo& column);
		void DrawRectBig(int colId, ColumnsInfo& column);
		bool InputTextString(std::string_view name, std::string& data, float itemWidth = 0.0f, ImGuiInputTextFlags_ flag = ImGuiInputTextFlags_None);

		void RecalculateElectricityAccruals();


		void SaveDataBase();

		void SaveElectricityCSV();
		void SavePaymentsToDateCSV(const Date& date);
		void SaveMembershipToCSV(const Date& date);


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

		static inline Application* s_Application;

		void OnResizeEvent();
		void OnDropEvent(const std::string& filename);
	protected:
		Ref<DataBase> m_DataBase = nullptr;

		GLFWwindow* m_Window;

		bool m_IsEdit;
		bool m_IsMembershipFeeOpeningBalance = false;
		bool m_IsElectricityOpeningBalance = false;

		bool m_OpenedMembershipAccrual = true;

		std::vector<std::string> m_Tests;

		bool m_DropInfo = false;
		std::vector<std::tuple<std::string, ElectricityAccrual>> m_DropInfoAccruals;
		bool m_EditTariffOnLoad = false;
		ElectricityAccrualCosts m_ElectricityAccrualCostsIntermediate;

		bool m_ShowDemoWindow = false;

		Ref<Table> m_HomesteadsTable;
		Ref<Table> m_MembershipFeePaymentsTable;
		Ref<SimpleTable> m_MembershipFeeOpeningBalanceTable;
		Ref<Table> m_ElectricityAccrualsTable;
		Ref<Table> m_ElectricityPaymentsTable;
		Ref<SimpleTable> m_ElectricityOpeningBalanceTable;

		Ref<TabsCollection> m_HomesteadTabsCollection;
		Ref<TabsCollection> m_MembershipFeePaymentTabsCollection;
		Ref<TabsCollection> m_ElectricityAccrualTabsCollection;
		Ref<TabsCollection> m_ElectricityPaymentTabsCollection;
	};

}
