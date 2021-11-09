#include "Application.h"

#include <iostream>
#include <string>
#include <codecvt>
#include <exception>
#include <filesystem>
#include <ctime>
#include <fstream>
#include <sstream>

#include <imgui.h>

#include "LoaderUnloader/JsonLoaderUnloader.h"

namespace fs = std::filesystem;

namespace LM
{
	std::unordered_map<std::string, Application::ColumnsInfo*> Application::ColumnsInfo::ColumnsMap;

	Application::Application(GLFWwindow* _Window)
		: m_IsEdit(false), m_Window(_Window)
	{
		s_Application = this;
		setlocale(LC_ALL, "Russian");

		CreateBackup("start");

		LoadFile();

		LoadColumns();

		m_HomesteadsTable = CreateRef<Table>
		(
			std::vector<std::string>{ u8"Номер участка", u8"ФИО", u8"Членские взносы", u8"Электросеть" },
			[=](size_t _Id) { return m_DataBase->GetHomestead(_Id); },
			[=]()			{ return m_DataBase->GetHomesteadsCount(); }
		);

		m_MembershipFeePaymentsTable = CreateRef<Table>
		(
			std::vector<std::string>{ u8"Дата", u8"Сумма", u8"Форма Платежа", u8"Номер документа" },
			[=](size_t _Id) { return m_DataBase->GetHomestead(m_HomesteadsTable->GetSelectedId())->GetMembershipFee().GetPayment(_Id); },
			[=]()			{ return m_DataBase->GetHomestead(m_HomesteadsTable->GetSelectedId())->GetMembershipFee().GetPaymentsCount(); }
		);

		m_ElectricityAccrualsTable = CreateRef<Table>
		(
			std::vector<std::string>{ u8"Дата", u8"День", u8"Ночь", u8"Общее" },
			[=](size_t _Id) { return m_DataBase->GetHomestead(m_HomesteadsTable->GetSelectedId())->GetElectricity().GetAccrual(_Id); },
			[=]() { return m_DataBase->GetHomestead(m_HomesteadsTable->GetSelectedId())->GetElectricity().GetAccrualsCount(); }
		);

		m_ElectricityPaymentsTable = CreateRef<Table>
		(
			std::vector<std::string>{ u8"Дата", u8"Сумма", u8"Форма Платежа", u8"Номер документа" },
			[=](size_t _Id) { return m_DataBase->GetHomestead(m_HomesteadsTable->GetSelectedId())->GetElectricity().GetPayment(_Id); },
			[=]() { return m_DataBase->GetHomestead(m_HomesteadsTable->GetSelectedId())->GetElectricity().GetPaymentsCount(); }
		);

		m_HomesteadTabsCollection				= CreateRef<TabsCollection>(u8"участка",	m_HomesteadsTable);
		m_MembershipFeePaymentTabsCollection	= CreateRef<TabsCollection>(u8"платежа",	m_MembershipFeePaymentsTable);
		m_ElectricityAccrualTabsCollection		= CreateRef<TabsCollection>(u8"начисления", m_ElectricityAccrualsTable);
		m_ElectricityPaymentTabsCollection		= CreateRef<TabsCollection>(u8"платежа",	m_ElectricityPaymentsTable);
	}

	Application::~Application()
	{
		SaveColumns();
		CreateBackup();
		SaveDataBase();

		ColumnsInfo::Clear();
		delete m_DataBase;
	}

	void Application::CreateBackup(std::string_view _Param)
	{
		time_t t;
		struct tm* t_m;
		t = time(NULL);
		t_m = localtime(&t);

		std::ostringstream Filename;
		Filename << 1900 + t_m->tm_year << "-" 
			<< std::setfill('0') << std::setw(2) << 1 + t_m->tm_mon	<< "-" 
			<< std::setfill('0') << std::setw(2) << t_m->tm_mday	<< "-" 
			<< std::setfill('0') << std::setw(2) << t_m->tm_hour	<< "-" 
			<< std::setfill('0') << std::setw(2) << t_m->tm_min		<< "-" 
			<< std::setfill('0') << std::setw(2) << t_m->tm_sec		<< (_Param.size() ? "-" : "") 
			<< _Param << ".json";
		std::cout << Filename.str() << std::endl;

		fs::path SourceFile		= "JsDB.json";
		fs::path TargetParent	= "backups/";

		auto Target = TargetParent / Filename.str();

		try // If you want to avoid exception handling, then use the error code overload of the following functions.
		{
			fs::create_directories(TargetParent); // Recursively create target directory if not existing.
			fs::copy_file(SourceFile, Target, fs::copy_options::overwrite_existing);
		}
		catch (std::exception& e) // Not using fs::filesystem_error since std::bad_alloc can throw too.  
		{
			std::cout << e.what();
		}
	}

	void Application::LoadFile()
	{
		m_DataBase = new DataBase;

		JsonLoaderUnloader JSLU("JsDB.json", m_DataBase);
		JSLU.Load();
	}

	void Application::LoadColumns()
	{
		ColumnsInfo::Add("MembershipFeeOpeningBalance", { u8"Номер участка", u8"ФИО", u8"Дата", u8"Начальное сальдо" });
		ColumnsInfo::Add("ElectricityAdding", { u8"Дата", u8"Участок", u8"День", u8"Ночь", u8"Общее" });
		ColumnsInfo::Add("ElectricityOpeningBalance", { u8"Номер участка", u8"ФИО", u8"Начальное сальдо" });

		std::ifstream fin;
		for (auto& [name, info] : ColumnsInfo::ColumnsMap)
		{
			fin.open(info->FileName.c_str());
			if (fin.is_open())
			{
				fin.read((char*)&(info->Widths[0]), info->Names.size() * sizeof(float));
				info->Update = true;
				info->WidthLoaded = true;
			}
			else
				std::cout << "Can't open file to read: " << info->FileName << std::endl;
			fin.close();
		}

		int WindowWidth, WindowHeight;
		glfwGetWindowSize(m_Window, &WindowWidth, &WindowHeight);
	}

	void Application::SaveColumns()
	{
		std::ofstream fout;

		for (auto& [name, info] : ColumnsInfo::ColumnsMap)
		{
			std::cout << name << ": ";
			for (int i = 0; i < info->Names.size(); i++)
			{
				std::cout << info->Widths[i] << " ";
			}
			std::cout << std::endl;

			fout.open(info->FileName.c_str());
			if (fout.is_open())
				fout.write((char*)&(info->Widths[0]), info->Names.size() * sizeof(float));
			else
				std::cout << "Can't open file to write: " << info->FileName << std::endl;
			fout.close();
		}
	}

	void Application::Run()
	{
		DrawMenuBar();
		DrawDataBase();

		if (m_ShowDemoWindow)
			ImGui::ShowDemoWindow(&m_ShowDemoWindow);
	}

	void Application::OnResizeEvent()
	{
		for (auto& column : ColumnsInfo::ColumnsVector)
			column->Update = true;
		//HomesteadColumn.Update = true;
		//MembershipFeeColumn.Update = true;
	}

	void Application::OnDropEvent(const std::string& filename)
	{
		m_DropInfo = true;
		m_DropInfoAccruals.clear();

		m_ElectricityAccrualCostsIntermediate = ElectricityAccrualCosts();

		//for (int i = 0; i < count; i++)
		//{
		//	std::cout << paths[i] << std::endl;
		//}

		std::cout << filename << std::endl;

	}

	void Application::DrawMenuBar()
	{
		if (ImGui::BeginMainMenuBar())
		{
			DrawFileMenu();
			DrawWindowMenu();
			DrawEditMenu();

			ImGui::EndMainMenuBar();
		}
	}

	void Application::DrawFileMenu()
	{
		if (ImGui::BeginMenu(u8"Файл"))
		{
			//if (ImGui::MenuItem("New", "Ctrl+N"))
			//{
			//}
			//if (ImGui::MenuItem("Open..", "Ctrl+O"))
			//{
			//	if (!OpenWindow)
			//	{
			//		OpenWindow = true;
			//		OpenFolder = LastFolder;
			//		OpenFolder = "I:/MultiMC/instances/VG/.minecraft/saves/mapwriter_sp_worlds/";
			//	}
			//}
			if (ImGui::MenuItem("Save", "Ctrl+S"))
			{
				SaveDataBase();
			}

			if (ImGui::BeginMenu("Export CSV"))
			{
				if (ImGui::MenuItem("Electricity"))
				{
					SaveElectricityCSV();
				}
				ImGui::EndMenu();
			}

			ImGui::EndMenu();
		}
	}

	void Application::DrawWindowMenu()
	{
		if (ImGui::BeginMenu(u8"Окно"))
		{
			//if (ImGui::MenuItem("New", "Ctrl+N"))
			//{
			//}
			//if (ImGui::MenuItem("Open..", "Ctrl+O"))
			//{
			//	if (!OpenWindow)
			//	{
			//		OpenWindow = true;
			//		OpenFolder = LastFolder;
			//		OpenFolder = "I:/MultiMC/instances/VG/.minecraft/saves/mapwriter_sp_worlds/";
			//	}
			//}
			//if (ImGui::MenuItem("Save", "Ctrl+S")) { /* Do stuff */ }
			//if (ImGui::MenuItem("Close", "Ctrl+W")) {  }
			////if (ImGui::DragFloat("WidgetSize", &WidgetSize.x))
			////{
			////	WidgetSize.y = WidgetSize.x;
			////}
			if (ImGui::MenuItem(u8"Демо ImGui", NULL, &m_ShowDemoWindow))
			{
			}
			ImGui::EndMenu();
		}
	}

	void Application::DrawEditMenu()
	{
		if (ImGui::BeginMenu(u8"Правка"))
		{
			if (ImGui::MenuItem(u8"Режим редактирования", NULL, &m_IsEdit))
			{

			}
			ImGui::EndMenu();
		}
	}

	void Application::DrawDataBase()
	{
		static bool p_open;
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);

		//ImGui::SetNextWindowBgAlpha(0.0f);
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar;
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

		ImGui::Begin(u8"Окно табов", &p_open, window_flags);
		ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_Reorderable | ImGuiTabBarFlags_TabListPopupButton | ImGuiTabBarFlags_FittingPolicyScroll | ImGuiTabBarFlags_NoCloseWithMiddleMouseButton;
		//opt_fitting_flags
		if (ImGui::BeginTabBar("MyTabBar", tab_bar_flags))
		{
			DrawHomesteads();
			DrawMembershipFeePayments();
			DrawElectricityAccruals();
			DrawElectricityPayments();

			DrawMembershipFeeOpeningBalance();
			DrawElectricityOpeningBalance();

			DrawDropEvent();

			//if (ImGui::Begin("Tests"))
			//{
			//	for (int i = 0; i < m_Tests.size(); i++)
			//		ImGui::Text(m_Tests[i].c_str());
			//}
			//ImGui::End();


			ImGui::EndTabBar();
		}
		ImGui::End();

		if (ImGui::Begin("Accruals"))
		{
			if (ImGui::Button("Sort"))
			{
				std::sort(MembershipFee::s_Accrual.begin(), MembershipFee::s_Accrual.end(), [](const MembershipFeeAccrual& _First, const MembershipFeeAccrual& _Second)
					{
						return _First.m_Date < _Second.m_Date;
					});
			}
			for (int i = 0; i < MembershipFee::s_Accrual.size(); i++)
			{
				ImGui::Text("%03d", i);
				ImGui::SameLine();
				MembershipFee::s_Accrual[i].m_Date.Draw();
				ImGui::SameLine();
				MembershipFee::s_Accrual[i].m_Money.Draw();
			}
			ImGui::End();
		}
	}

	void Application::DrawRect(int colId, ColumnsInfo& column)
	{
		if (colId % 2)
		{
			ImGui::Columns(1);

			ImDrawList* draw_list = ImGui::GetWindowDrawList();
			int sz = 16 + 4;
			const ImVec2 p = ImGui::GetCursorScreenPos();
			const ImU32 col32 = 0xffdddddd;
			float x = p.x - 24, y = p.y - 3;
			draw_list->AddRectFilled(ImVec2(x, y), ImVec2(x + 9000 + 48, y + sz), col32);

			ImGui::Columns(column.Names.size(), column.Name.c_str(), false);
		}
	}

	void Application::DrawRectBig(int colId, ColumnsInfo& column)
	{
		if (colId % 2)
		{
			ImGui::Columns(1);

			ImDrawList* draw_list = ImGui::GetWindowDrawList();
			int sz = 26;
			const ImVec2 p = ImGui::GetCursorScreenPos();
			const ImU32 col32 = 0xffdddddd;
			float x = p.x - 24, y = p.y - 2;
			draw_list->AddRectFilled(ImVec2(x, y), ImVec2(x + 9000 + 48, y + sz), col32);

			ImGui::Columns(column.Names.size(), column.Name.c_str(), false);
		}
	}

	bool Application::InputTextString(std::string_view name, std::string& data, int itemWidth, ImGuiInputTextFlags_ flag)
	{
		if (itemWidth)
			ImGui::PushItemWidth(itemWidth);

		int Size = data.size() + 128;

		char* CharBufferNumber = new char[Size] { 0 };
		memcpy(CharBufferNumber, data.data(), data.size());
		bool isStringEdited = ImGui::InputText(name.data(), CharBufferNumber, Size, flag);
		data = CharBufferNumber;
		delete[] CharBufferNumber;

		if (itemWidth)
			ImGui::PopItemWidth();

		return isStringEdited;
	}

	void Application::DrawHomesteads()
	{
		m_HomesteadTabsCollection->Draw();

		if (ImGui::BeginTabItem(u8"Участки"))
		{
			ImGui::Text(u8"Количество участков: %d", m_DataBase->GetHomesteadsCount());

			if (ImGui::Button(u8"Добавить участок"))
			{
				m_HomesteadTabsCollection->SetCreate
				(
					[=](Ref<const TabDataStruct> _TabDS) { m_DataBase->AddHomestead(_TabDS); },
					CreateRef<HomesteadTabDS>()
				);
			} 
			ImGui::SameLine();
			if (ImGui::Button(u8"Редактировать участок") && m_HomesteadsTable->HasSelectedRow())
			{
				m_HomesteadTabsCollection->SetEdit
				(
					[=](Ref<const TabDataStruct> _TabDS) { m_DataBase->EditHomestead(m_HomesteadsTable->GetSelectedId(), _TabDS); },
					m_DataBase->GetHomestead(m_HomesteadsTable->GetSelectedId())
				);
			}

			if (m_IsEdit)
			{
				ImGui::SameLine();
				m_HomesteadTabsCollection->DrawDeleteButton
				(
					u8"Удалить участок",
					[=]() 
					{ 
						m_DataBase->DeleteHomestead(m_HomesteadsTable->GetSelectedId()); 
					},
					m_HomesteadsTable->HasSelectedRow()
				);
			}

			if (m_IsEdit && ImGui::Button(u8"Начальное сальдо: членские взносы"))
			{
				m_IsMembershipFeeOpeningBalance = !m_IsMembershipFeeOpeningBalance;
			}

			if (m_IsEdit && ImGui::Button(u8"Начальное сальдо: электроэнергия"))
			{
				m_IsElectricityOpeningBalance = !m_IsElectricityOpeningBalance;
			}

			m_HomesteadsTable->Draw();

			ImGui::EndTabItem();
		}
	}

	void Application::DrawMembershipFeePayments()
	{
		if (!m_HomesteadsTable->HasSelectedRow())
			return;

		m_MembershipFeePaymentTabsCollection->Draw();

		if (ImGui::BeginTabItem(u8"Членские взносы"))
		{
			//if (!m_HomesteadsTable->HasSelectedRow())
			//{
			//	ImGui::Text(u8"Участок не выбран!");
			//	ImGui::EndTabItem();
			//	return;
			//}

			DrawCheckMembershipAccrual();

			time_t t;
			struct tm* t_m;
			t = time(NULL);
			t_m = localtime(&t);
			//std::string date = std::to_string(1900 + t_m->tm_year) + "-" + std::to_string(1 + t_m->tm_mon) + "-" + std::to_string(t_m->tm_mday);

			const Money Amount = m_DataBase->GetHomestead(m_HomesteadsTable->GetSelectedId())->GetMembershipFee().GetDebt();//m_MembershipFee.m_Debt;
			if (Amount < 0)
			{
				ImGui::Text(u8"Переплата на %02d.%02d.%d:", t_m->tm_mday, 1 + t_m->tm_mon, 1900 + t_m->tm_year); ImGui::SameLine();
				Amount.DrawAbs();
			}
			else
			{
				ImGui::Text(u8"Долг на %02d.%02d.%d:", t_m->tm_mday, 1 + t_m->tm_mon, 1900 + t_m->tm_year); ImGui::SameLine();
				Amount.DrawAbs();
			}

			if (ImGui::Button(u8"Добавить платеж"))
			{
				m_MembershipFeePaymentTabsCollection->SetCreate
				(
					[=](Ref<const TabDataStruct> _TabDS) { m_DataBase->AddMembershipFeePayment(m_HomesteadsTable->GetSelectedId(), _TabDS); },
					CreateRef<PaymentTabDS>()
				);
			} 
			ImGui::SameLine();
			if (ImGui::Button(u8"Редактировать платеж") && m_MembershipFeePaymentsTable->HasSelectedRow())
			{
				m_MembershipFeePaymentTabsCollection->SetEdit
				(
					[=](Ref<const TabDataStruct> _TabDS) 
					{	
						m_DataBase->EditMembershipFeePayment(m_HomesteadsTable->GetSelectedId(), m_MembershipFeePaymentsTable->GetSelectedId(), _TabDS); 
					},
					m_DataBase->GetHomestead(m_HomesteadsTable->GetSelectedId())->GetMembershipFee().GetPayment(m_MembershipFeePaymentsTable->GetSelectedId())
				);
			} 
			ImGui::SameLine();
			if (ImGui::Button(u8"Проверка начислений"))
			{
				m_OpenedMembershipAccrual = true;
			}
			if (m_IsEdit)
			{
				ImGui::SameLine();
				m_MembershipFeePaymentTabsCollection->DrawDeleteButton
				(
					u8"Удалить платеж",
					[=]()
					{
						m_DataBase->DeleteMembershipFeePayment(m_HomesteadsTable->GetSelectedId(), m_MembershipFeePaymentsTable->GetSelectedId());
					},
					m_MembershipFeePaymentsTable->HasSelectedRow()
				);
			}

			m_MembershipFeePaymentsTable->Draw();

			ImGui::EndTabItem();
		}
	}

	void Application::DrawMembershipFeeOpeningBalance()
	{
		if ((m_IsEdit && m_IsMembershipFeeOpeningBalance) && ImGui::BeginTabItem(u8"Начальное сальдо: членские взносы"))
		{
			ImGui::Text(u8"Количество участков: %d", m_DataBase->GetHomesteadsCount());

			Date& date = OpeningBalance::s_Date;

			date.DrawEdit();

			ColumnsInfo& column = *ColumnsInfo::ColumnsMap["MembershipFeeOpeningBalance"];

			ImGui::Columns(column.Names.size(), column.Name.c_str(), true); // 2-ways, with border
			ImGui::Separator();
			if (column.Update)
			{
				if (column.WidthLoaded)
				{
					for (int i = 0; i < column.Names.size() - 1; i++)
					{
						ImGui::SetColumnWidth(i, column.Widths[i]);
						std::cout << column.Widths[i] << " ";
					}
					std::cout << column.Widths[column.Names.size() - 1] << "\n";
					column.Update = false;
				}
				else
				{
					column.WidthLoaded = true;
				}
			}
			column.DrawNames();

			ImGui::Separator();
			ImGui::Columns(1);
			ImGui::BeginChild(column.Name.c_str(), ImVec2(0, 0), false);
			ImGui::Columns(column.Names.size(), column.Name.c_str(), false);
			for (int i = 0; i < column.Names.size() - 1; i++)
			{
				ImGui::SetColumnWidth(i, column.Widths[i]);
			}

			for (int i = 0; i < m_DataBase->GetHomesteadsCount(); i++)
			{
				DrawRectBig(i, column);

				ImGui::PushID(i);

				auto& homestead = m_DataBase->m_Homesteads[i];

				ImGui::Text(homestead->GetNumber().data());
				ImGui::NextColumn();

				ImGui::Text("%s %s %s", homestead->GetSurname().data(), homestead->GetForename().data(), homestead->GetPatronymic().data());
				ImGui::NextColumn();

				homestead->m_MembershipFee.m_OpeningBalance.s_Date.Draw();
				ImGui::NextColumn();

				homestead->m_MembershipFee.m_Debt -= homestead->m_MembershipFee.m_OpeningBalance.m_Money;
				homestead->m_MembershipFee.m_OpeningBalance.m_Money.DrawEdit(u8"Сумма");
				homestead->m_MembershipFee.m_Debt += homestead->m_MembershipFee.m_OpeningBalance.m_Money;

				ImGui::NextColumn();

				ImGui::PopID();
			}
			ImGui::Columns(1);
			ImGui::EndChild();
			ImGui::EndTabItem();
		}
	}

	void Application::DrawCheckMembershipAccrual()
	{
		static std::vector<MembershipFeeAccrual> accruals;
		static std::vector<int> AccrualsToDelete;
		if (m_OpenedMembershipAccrual)
		{
			m_OpenedMembershipAccrual = false;
			accruals.clear();
			AccrualsToDelete.clear();
			time_t t;
			struct tm* t_m;
			t = time(NULL);
			t_m = localtime(&t);

			int NowYear = 1900 + t_m->tm_year;
			int NowMonth = 1 + t_m->tm_mon;
			OpeningBalance::s_Date;
			int Size = (NowYear - OpeningBalance::s_Date.m_Year) * 12 - OpeningBalance::s_Date.m_Month + NowMonth;
			//std::cout << Size << std::endl;
			if (Size < 0)
			{
				// 1. Дата начального сальдо больше текушей
				ImGui::OpenPopup(u8"Дата начального сальдо больше текушей");
			}
			else if (Size == 0)
			{
				// 2.
			}
			else if (Size > 0)
			{
				// 3.
				int TestYear = OpeningBalance::s_Date.m_Year;
				int TestMonth = OpeningBalance::s_Date.m_Month + 1;
				for (int i = 0; i < Size; i++, TestMonth++)
				{
					if (TestMonth == 13)
					{
						TestMonth = 1;
						TestYear += 1;
					}
					bool founded = false;
					for (auto& accrual : MembershipFee::s_Accrual)
					{
						if (accrual.m_Date.m_Year == TestYear && accrual.m_Date.m_Month == TestMonth)
						{
							founded = true;
							break;
						}
					}
					if (!founded)
					{
						accruals.push_back(MembershipFeeAccrual());
						accruals.back().m_Date.m_Month = TestMonth;
						accruals.back().m_Date.m_Year = TestYear;
						accruals.back().m_Money = MembershipFeeAccrual::MembershipFeeAmount;
						//std::cout << accruals.back().m_Money.m_Amount << std::endl;
					}
				}
			}

			char lable[12]{ 0 };
			sprintf(lable, "%d%02d", OpeningBalance::s_Date.m_Year, OpeningBalance::s_Date.m_Month);
			int OpeningBalanceDate;
			sscanf(lable, "%d", &OpeningBalanceDate);
			for (int i = 0; i < MembershipFee::s_Accrual.size(); i++)
			{
				char buffer[12]{ 0 };
				sprintf(buffer, "%d%02d", MembershipFee::s_Accrual[i].m_Date.m_Year, MembershipFee::s_Accrual[i].m_Date.m_Month);
				int AccrualDate;
				sscanf(buffer, "%d", &AccrualDate);
				std::cout << "Accrual check " << i << ": " << AccrualDate << " " << OpeningBalanceDate;
				if (AccrualDate <= OpeningBalanceDate)
				{
					std::cout << " need to delete";
					AccrualsToDelete.push_back(i);
				}
				std::cout << std::endl;
			}

			std::sort(AccrualsToDelete.begin(), AccrualsToDelete.end(), [](const int& first, const int& second)
				{
					MembershipFeeAccrual& ac1 = MembershipFee::s_Accrual[first];
					MembershipFeeAccrual& ac2 = MembershipFee::s_Accrual[second];

					return ac1.m_Date < ac2.m_Date;
				});

			if (accruals.size() > 0 || AccrualsToDelete.size() > 0)
			{
				ImGui::OpenPopup(u8"Начисления для добавления##Членские взносы");
			}
		}
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImVec2 window_size = ImVec2(600, 400);
		ImVec2 Distance = ImVec2(viewport->Size.x / 2, viewport->Size.y / 2);
		ImVec2 window_pos = ImVec2(viewport->Pos.x + Distance.x - window_size.x / 2, viewport->Pos.y + Distance.y - window_size.y / 2);
		ImVec2 window_pos_pivot = ImVec2(0.0f, 0.0f);
		ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
		ImGui::SetNextWindowSize(window_size);
		//ImGui::SetNextWindowPos();
		ImGui::SetNextWindowViewport(viewport->ID);

		if (ImGui::BeginPopupModal(u8"Начисления для добавления##Членские взносы", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDocking))
		{
			//std::cout << "Accruals to delete count:  " << AccrualsToDelete.size() << std::endl;
			ImGui::Text(u8"Отсутствующие начисления членских взносов для всех участков:");

			ImGui::BeginChild("ChildNew", ImVec2(ImGui::GetWindowContentRegionWidth() * 0.5f, 300), true);
			for (int i = 0; i < accruals.size(); i++)
			{
				MembershipFeeAccrual& accrual = accruals[i];
				ImGui::PushID(i);
				accrual.m_Date.Draw();
				ImGui::SameLine();
				if (ImGui::Button(u8"Добавить"))
				{
					MembershipFee::s_Accrual.push_back(accrual);
					for (auto& homestead : m_DataBase->m_Homesteads)
					{
						if (homestead->GetAddMembershipFees())
						{
							homestead->m_MembershipFee.m_Debt += accrual.m_Money;
						}
					}
					accruals.erase(accruals.begin() + i);
					i--;
				}

				ImGui::PopID();
			}
			ImGui::EndChild();

			ImGui::SameLine();

			ImGui::BeginChild("ChildDel", ImVec2(0, 300), true);
			for (int i = 0; i < AccrualsToDelete.size(); i++)
			{
				MembershipFeeAccrual& accrual = MembershipFee::s_Accrual[AccrualsToDelete[i]];
				ImGui::PushID(i);
				accrual.m_Date.Draw();
				ImGui::SameLine();
				if (ImGui::Button(u8"Удалить"))
				{
					for (auto& homestead : m_DataBase->m_Homesteads)
					{
						if (homestead->GetAddMembershipFees())
						{
							homestead->m_MembershipFee.m_Debt -= accrual.m_Money;
						}
					}
					MembershipFee::s_Accrual.erase(MembershipFee::s_Accrual.begin() + AccrualsToDelete[i]);
					AccrualsToDelete.erase(AccrualsToDelete.begin() + i);

					AccrualsToDelete.clear();
					for (int i = 0; i < MembershipFee::s_Accrual.size(); i++)
					{
						if (MembershipFee::s_Accrual[i].m_Date <= OpeningBalance::s_Date)
						{
							std::cout << " need to delete";
							AccrualsToDelete.push_back(i);
						}
						std::cout << std::endl;
					}

					std::sort(AccrualsToDelete.begin(), AccrualsToDelete.end(), [](const int& first, const int& second)
						{
							MembershipFeeAccrual& ac1 = MembershipFee::s_Accrual[first];
							MembershipFeeAccrual& ac2 = MembershipFee::s_Accrual[second];

							return ac1.m_Date < ac2.m_Date;
						});

					i--;
				}

				ImGui::PopID();
			}
			ImGui::EndChild();

			if (ImGui::Button(u8"Закрыть", ImVec2(120, 0)))
			{
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
	}

	void Application::DrawElectricityAccruals()
	{
		if (m_HomesteadsTable->GetSelectedId() == -1)
			return;

		m_ElectricityAccrualTabsCollection->Draw();

		if (ImGui::BeginTabItem(u8"Начисление электроэнергии"))
		{
			//if (!m_HomesteadsTable->HasSelectedRow())
			//{
			//	ImGui::Text(u8"Участок не выбран!");
			//	ImGui::EndTabItem();
			//	return;
			//}

			ImGui::TextUnformatted(u8"Первое начисление является начальным сальдо!");

			auto& homestead = m_DataBase->m_Homesteads[m_HomesteadsTable->GetSelectedId()];

			if (ImGui::Button(u8"Добавить начисление"))
			{
				m_ElectricityAccrualTabsCollection->SetCreate
				(
					[=](Ref<const TabDataStruct> _TabDS) { m_DataBase->AddElectricityAccrual(m_HomesteadsTable->GetSelectedId(), _TabDS); },
					CreateRef<ElectricityAccrualTabDS>()
				);
			} 
			ImGui::SameLine();
			if (ImGui::Button(u8"Редактировать начисление") && m_ElectricityAccrualsTable->HasSelectedRow())
			{
				m_ElectricityAccrualTabsCollection->SetEdit
				(
					[=](Ref<const TabDataStruct> _TabDS)
					{
						m_DataBase->EditElectricityAccrual(m_HomesteadsTable->GetSelectedId(), m_ElectricityAccrualsTable->GetSelectedId(), _TabDS);
					},
					m_DataBase->GetHomestead(m_HomesteadsTable->GetSelectedId())->GetElectricity().GetAccrual(m_ElectricityAccrualsTable->GetSelectedId())
				);
			}
			if (m_IsEdit)
			{
				ImGui::SameLine();
				m_ElectricityAccrualTabsCollection->DrawDeleteButton
				(
					u8"Удалить начисление",
					[=]()
					{
						m_DataBase->DeleteElectricityAccrual(m_HomesteadsTable->GetSelectedId(), m_ElectricityAccrualsTable->GetSelectedId());
					},
					m_ElectricityAccrualsTable->HasSelectedRow()
				);
			}

			m_ElectricityAccrualsTable->Draw();

			ImGui::EndTabItem();
		}
	}

	void Application::DrawElectricityPayments()
	{
		if (m_HomesteadsTable->GetSelectedId() == -1)
			return;

		m_ElectricityPaymentTabsCollection->Draw();

		if (ImGui::BeginTabItem(u8"Оплата электроэнергии"))
		{
			//if (!m_HomesteadsTable->HasSelectedRow())
			//{
			//	ImGui::Text(u8"Участок не выбран!");
			//	ImGui::EndTabItem();
			//	return;
			//}

			Homestead& homestead = *m_DataBase->m_Homesteads[m_HomesteadsTable->GetSelectedId()];

			time_t t;
			struct tm* t_m;
			t = time(NULL);
			t_m = localtime(&t);

			Money& Amount = homestead.m_Electricity.m_All;
			if (Amount < 0)
			{
				ImGui::Text(u8"Переплата на %02d.%02d.%d:", t_m->tm_mday, 1 + t_m->tm_mon, 1900 + t_m->tm_year); ImGui::SameLine();
				Amount.DrawAbs();
			}
			else
			{
				ImGui::Text(u8"Долг на %02d.%02d.%d:", t_m->tm_mday, 1 + t_m->tm_mon, 1900 + t_m->tm_year); ImGui::SameLine();
				Amount.DrawAbs();
			}
			if (ImGui::Button(u8"Добавить платеж"))
			{
				m_ElectricityPaymentTabsCollection->SetCreate
				(
					[=](Ref<const TabDataStruct> _TabDS) { m_DataBase->AddElectricityPayment(m_HomesteadsTable->GetSelectedId(), _TabDS); },
					CreateRef<PaymentTabDS>()
				);
			} 
			ImGui::SameLine();
			if (ImGui::Button(u8"Редактировать платеж") && m_ElectricityPaymentsTable->HasSelectedRow())
			{
				m_ElectricityPaymentTabsCollection->SetEdit
				(
					[=](Ref<const TabDataStruct> _TabDS)
					{
						m_DataBase->EditElectricityPayment(m_HomesteadsTable->GetSelectedId(), m_ElectricityPaymentsTable->GetSelectedId(), _TabDS);
					},
					m_DataBase->GetHomestead(m_HomesteadsTable->GetSelectedId())->GetElectricity().GetPayment(m_ElectricityPaymentsTable->GetSelectedId())
				);
			}
			if (m_IsEdit)
			{
				ImGui::SameLine();
				m_ElectricityPaymentTabsCollection->DrawDeleteButton
				(
					u8"Удалить платеж",
					[=]()
					{
						m_DataBase->DeleteElectricityPayment(m_HomesteadsTable->GetSelectedId(), m_ElectricityPaymentsTable->GetSelectedId());
					},
					m_ElectricityPaymentsTable->HasSelectedRow()
				);
			}

			m_ElectricityPaymentsTable->Draw();

			ImGui::EndTabItem();
		}
	}

	void Application::RecalculateElectricityAccruals()
	{
		for (auto& homestead : m_DataBase->m_Homesteads)
		{
			homestead->m_Electricity.Recalculate(homestead->m_Data.ElectricityPrivilege.HasPrivilege);
		}
	}

	void Application::DrawDropEvent()
	{
		if (m_DropInfo)
		{
			DrawEditTariffOnLoad();
			if (ImGui::BeginTabItem(u8"Добавление показаний электроэнергии"))
			{
				ColumnsInfo& column = *ColumnsInfo::ColumnsMap["ElectricityAdding"];

				if (ImGui::Button(u8"Сохранить в базе"))
				{
					for (auto& homestead : m_DataBase->m_Homesteads)
					{
						for (auto& accrualInfo : m_DropInfoAccruals)
						{
							auto& [name, accrual] = accrualInfo;
							if (homestead->m_Data.Number == name)
							{
								accrual.m_Data.Costs = m_ElectricityAccrualCostsIntermediate;
								homestead->m_Electricity.m_Accruals.push_back(CreateRef<ElectricityAccrual>(accrual));
								break;
							}
						}
					}
					m_DropInfo = false;
					RecalculateElectricityAccruals();
				}
				ImGui::SameLine();
				if (ImGui::Button(u8"Отмена"))
				{
					m_DropInfo = false;
				}
				if (ImGui::Button(u8"Изменение констант и тарифов"))
				{
					m_EditTariffOnLoad = true;
					//DrawEditTariffOnLoad();
				}
				// Сохранить в базе
				// Отмена

				//if (m_DropInfoAccruals.size() > 0)
				//{
				//	auto& [number, accrual] = m_DropInfoAccruals[0];
				//	ImGui::Text("%02d.%02d.%d", accrual.m_Date.Day, accrual.m_Date.Month, accrual.m_Date.Year);
				//}

				ImGui::Columns(column.Names.size(), column.Name.c_str(), true);
				ImGui::Separator();
				//static bool Init = true;
				if (column.Update)
				{
					if (column.WidthLoaded)
					{
						for (int i = 0; i < column.Names.size() - 1; i++)
						{
							ImGui::SetColumnWidth(i, column.Widths[i]);
							std::cout << column.Widths[i] << " ";
						}
						std::cout << column.Widths[column.Names.size() - 1] << "\n";
						column.Update = false;
					}
					else
					{
						column.WidthLoaded = true;
					}
				}

				column.DrawNames();

				ImGui::Separator();
				ImGui::Columns(1);
				ImGui::BeginChild(column.Name.c_str(), ImVec2(0, 0), false);
				ImGui::Columns(column.Names.size(), column.Name.c_str(), false);
				for (int i = 0; i < column.Names.size() - 1; i++)
				{
					ImGui::SetColumnWidth(i, column.Widths[i]);
				}

				for (int i = 0; i < m_DropInfoAccruals.size(); i++)
				{
					DrawRect(i, column);

					ImGui::PushID(i);

					//Homestead& homestead = m_DataBase->GetHomesteads()[SelectedHomestead];
					//ElectricityAccrual& electricityAccrual = m_DataBase->GetHomesteads()[SelectedHomestead].m_Electricity.m_Accruals[i];
					auto& [number, accrual] = m_DropInfoAccruals[i];

					////////////////////////////////////////////////////////////////////////
					//////// 1 /////////////////////////////////////////////////////////////
					////////////////////////////////////////////////////////////////////////
					//ImGui::Text("%02d.%02d.%d", accrual.m_Date.Day, accrual.m_Date.Month, accrual.m_Date.Year);
					accrual.m_Data.Date.Draw();
					ImGui::NextColumn();

					ImGui::Text(number.c_str());
					ImGui::NextColumn();

					////////////////////////////////////////////////////////////////////////
					//////// 2 /////////////////////////////////////////////////////////////
					////////////////////////////////////////////////////////////////////////
					accrual.m_Data.Day.Draw();
					ImGui::NextColumn();

					////////////////////////////////////////////////////////////////////////
					//////// 3 /////////////////////////////////////////////////////////////
					////////////////////////////////////////////////////////////////////////
					accrual.m_Data.Night.Draw();
					ImGui::NextColumn();

					////////////////////////////////////////////////////////////////////////
					//////// 4 /////////////////////////////////////////////////////////////
					////////////////////////////////////////////////////////////////////////
					ImGui::Text("%lld.%03lld", accrual.GetAllMonth().m_Watt / 1000, accrual.GetAllMonth().m_Watt % 1000);
					ImGui::NextColumn();

					ImGui::PopID();
				}
				ImGui::Columns(1);
				ImGui::EndChild();
				ImGui::EndTabItem();
			}
		}
	}

	void Application::DrawEditTariffOnLoad()
	{
		if (m_EditTariffOnLoad && ImGui::BeginTabItem(u8"Изменение тарифа##OnLoad", &m_EditTariffOnLoad))
		{
			m_ElectricityAccrualCostsIntermediate.m_Day.DrawEdit(u8"Дневной тариф");
			m_ElectricityAccrualCostsIntermediate.m_Night.DrawEdit(u8"Ночной тариф");

			for (int i = 0; i < m_ElectricityAccrualCostsIntermediate.m_Others.size(); i++)
			{
				ImGui::PushID(i);
				ImGui::Separator();
				auto& [name, money] = m_ElectricityAccrualCostsIntermediate.m_Others[i];
				money.DrawEdit("##Sum"); ImGui::SameLine();
				//ImGui::Text(name.c_str());
				InputTextString("##Name", name, 250);
				if (ImGui::Button(u8"Удалить константку"))
				{
					m_ElectricityAccrualCostsIntermediate.m_Others.erase(m_ElectricityAccrualCostsIntermediate.m_Others.begin() + i);
					i--;
				}
				ImGui::PopID();
			}
			ImGui::Separator();
			if (ImGui::Button(u8"Добавить константу"))
			{
				m_ElectricityAccrualCostsIntermediate.m_Others.push_back({ "<NONE>", Money() });
			}

			if (ImGui::Button("Ok"))
			{
				m_EditTariffOnLoad = false;
			}

			ImGui::EndTabItem();
		}

	}

	void Application::SaveDataBase()
	{
		JsonLoaderUnloader jslu("JsDB.json", m_DataBase);
		jslu.Unload();

		//Unloader_v2* unloader = Unloader_v2::Create();
		//unloader->HandleDataBase(m_DataBase);
	}

	void Application::SaveElectricityCSV()
	{
		std::ofstream fout;
		fout.open("Electricity.csv");

		const std::string SepCSV = ",";

		if (!fout.is_open())
		{
			std::cout << "Error: Can't Save file!: " << "Electricity.csv" << std::endl;
			return;
		}
		std::cout << "File ok! Saving file..." << std::endl;
		for (auto& homestead : m_DataBase->m_Homesteads)
		{
			if (homestead->m_Electricity.m_Accruals.size() < 2)
				continue;

			auto& accruals = homestead->m_Electricity.m_Accruals;
			auto& accrualBack = accruals.back();
			auto& accrualPrev = accruals[accruals.size() - 2];
			fout << homestead->m_Data.Number << SepCSV;
			fout << accrualBack->GetAllMonth() << SepCSV;
			fout << accrualBack->m_Data.Day << SepCSV;
			fout << accrualBack->m_Data.Night << SepCSV;

			// Разница показаний кВт
			KiloWatt dayKW = accrualBack->m_Data.Day - accrualPrev->m_Data.Day;
			KiloWatt nightKW = accrualBack->m_Data.Night - accrualPrev->m_Data.Night;
			KiloWatt monthKW = dayKW + nightKW;

			fout << monthKW << SepCSV;
			fout << dayKW << SepCSV;
			fout << nightKW << SepCSV;

			// Начислено РУБ
			Money fullSum;
			Money calcDay = Electricity::CalcMonthMoney(dayKW.m_Watt, accrualBack->m_Data.Costs.m_Day);
			Money calcNight = Electricity::CalcMonthMoney(nightKW.m_Watt, accrualBack->m_Data.Costs.m_Night);
			Money calcLosses = Electricity::CalcLosses(calcDay, calcNight);
			calcDay = Electricity::CalcWithBenefits(calcDay, Money(), false /* Homestead benefits 70% */);
			calcNight = Electricity::CalcWithBenefits(calcNight, Money(), false /* Homestead benefits 70% */);
			Money calcMonth = calcDay + calcNight;
			fullSum = calcMonth + calcLosses;

			fout << calcDay << SepCSV;
			fout << calcNight << SepCSV;
			fout << calcMonth << SepCSV;
			fout << calcLosses << SepCSV;
			for (auto& [name, money] : accrualBack->m_Data.Costs.m_Others)
			{
				fout << money << SepCSV;
				fullSum += money;
			}

			Money sumToDate = homestead->m_Electricity.CalcAccrualsToDate(accrualPrev->m_Data.Date, homestead->m_Data.ElectricityPrivilege.HasPrivilege);
			fullSum += sumToDate;

			fout << sumToDate << SepCSV;
			fout << fullSum << SepCSV;

			bool Payed = false;
			Payment LastPay;
			for (auto& payment : homestead->m_Electricity.m_Payments)
			{
				if (payment->m_Data.Date > accrualPrev->m_Data.Date && payment->m_Data.Date <= accrualBack->m_Data.Date)
				{
					Payed = true;
					LastPay.m_Data.Amount += payment->m_Data.Amount;
					LastPay.m_Data.Date = payment->m_Data.Date;
				}
			}

			if (Payed)
			{
				fout << LastPay.m_Data.Date.GetString() << SepCSV;
				fout << LastPay.m_Data.Amount << SepCSV;
			}
			else
			{
				fout << SepCSV << SepCSV;
			}

			fout << fullSum - LastPay.m_Data.Amount << std::endl;

			std::cout << homestead->m_Electricity.CalcAccrualsToDate(accrualBack->m_Data.Date, homestead->m_Data.ElectricityPrivilege.HasPrivilege) << std::endl;
		}

		std::cout << "File Saved!" << std::endl;

		fout << "Test message! " << std::endl;

		fout.close();
	}

	void Application::SavePaymentsToDateCSV(const Date& date)
	{
		// ElectricityDebt.csv
		// MembershipFeeDebt.csv
		// ??? целевые

		SaveMembershipToCSV(date);
	}

	void Application::SaveMembershipToCSV(const Date& date)
	{
		std::string fileName = "MembershipFeeDebt" + date.GetString() + ".csv";

		std::ofstream fout;
		fout.open(fileName);

		if (!fout.is_open())
		{
			std::cout << "Error: Can't Save file!: " << fileName << std::endl;
			return;
		}
		std::cout << "File ok! Saving file..." << std::endl;

		for (auto& homestead : m_DataBase->m_Homesteads)
		{
			fout << homestead->m_Data.Number << ";";
			Money EndSum = homestead->m_MembershipFee.m_OpeningBalance.m_Money;
			for (auto& accrual : homestead->m_MembershipFee.s_Accrual)
			{
				if (accrual.m_Date > date)
					break;
				EndSum += accrual.m_Money;
			}
			for (auto& payment : homestead->m_MembershipFee.m_Payments)
			{
				if (payment->m_Data.Date > date)
					break;
				EndSum -= payment->m_Data.Amount;
			}
		}

		std::cout << "File Saved!" << std::endl;
		fout.close();
	}

	void Application::DrawElectricityOpeningBalance()
	{
		if ((m_IsEdit && m_IsElectricityOpeningBalance) && ImGui::BeginTabItem(u8"Начальное сальдо: электроэнергия"))
		{
			ImGui::Text(u8"Количество участков: %d", m_DataBase->GetHomesteadsCount());

			ColumnsInfo& column = *ColumnsInfo::ColumnsMap["ElectricityOpeningBalance"];

			ImGui::Columns(column.Names.size(), column.Name.c_str(), true); // 2-ways, with border
			ImGui::Separator();
			if (column.Update)
			{
				if (column.WidthLoaded)
				{
					for (int i = 0; i < column.Names.size() - 1; i++)
					{
						ImGui::SetColumnWidth(i, column.Widths[i]);
						std::cout << column.Widths[i] << " ";
					}
					std::cout << column.Widths[column.Names.size() - 1] << "\n";
					column.Update = false;
				}
				else
				{
					column.WidthLoaded = true;
				}
			}
			column.DrawNames();

			ImGui::Separator();
			ImGui::Columns(1);
			ImGui::BeginChild(column.Name.c_str(), ImVec2(0, 0), false);
			ImGui::Columns(column.Names.size(), column.Name.c_str(), false);
			for (int i = 0; i < column.Names.size() - 1; i++)
			{
				ImGui::SetColumnWidth(i, column.Widths[i]);
			}

			for (int i = 0; i < m_DataBase->GetHomesteadsCount(); i++)
			{
				DrawRectBig(i, column);


				ImGui::PushID(i);

				Homestead& homestead = *m_DataBase->m_Homesteads[i];

				ImGui::Text(homestead.GetNumber().data());
				ImGui::NextColumn();

				ImGui::Text("%s %s %s", homestead.GetSurname().data(), homestead.GetForename().data(), homestead.GetPatronymic().data());
				ImGui::NextColumn();

				homestead.m_Electricity.m_All -= homestead.m_Electricity.m_OpeningBalance;
				homestead.m_Electricity.m_OpeningBalance.DrawEdit(u8"Сумма");
				homestead.m_Electricity.m_All += homestead.m_Electricity.m_OpeningBalance;

				ImGui::NextColumn();

				ImGui::PopID();
			}
			ImGui::Columns(1);
			ImGui::EndChild();
			ImGui::EndTabItem();
		}
	}

}
