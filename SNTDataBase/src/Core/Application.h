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
#include "UI/Tabs/TabCsv/TabCsvSelector.h"
#include "Base.h"


namespace LM
{

	class Application
	{
	public:
		Application(GLFWwindow* _Window);
		~Application();

		void Run();

		static inline Application* s_Application;

		void OnResizeEvent();
		void OnDropEvent(const std::string& _FileName);
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

		void SaveDataBase();

		void SaveElectricityCSV();
		void SavePaymentsToDateCSV(const Date& date);
		void SaveMembershipToCSV(const Date& date);

		void CreateBackup(std::string_view _Param = "");
		void LoadDataBase();
	protected:
		Ref<DataBase> m_DataBase = nullptr;

		GLFWwindow* m_Window;

		bool m_IsEdit;
		bool m_IsMembershipFeeOpeningBalance = false;
		bool m_IsElectricityOpeningBalance = false;

		bool m_OpenedMembershipAccrual = true;

		std::vector<std::string> m_Tests;

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

		Ref<TabCsvSelector> m_TabCsvSelector;
	};

}
