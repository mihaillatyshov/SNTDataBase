#include "Application.h"

#include <iostream>
#include <string>
#include <codecvt>
#include <exception>
#include <filesystem>
#include <ctime>
#include <fstream>
#include <sstream>
#include <chrono>

#include <imgui.h>
#include <OpenXLSX.hpp>

#include "LoaderUnloader/JsonLoaderUnloader.h"
#include "LoaderUnloader/CsvReader.h"
#include "Utils/Time.h"
#include "Utils/ImGuiUtils.h"

namespace fs = std::filesystem;

namespace LM
{
	
	Application::Application(GLFWwindow* _Window)
		: m_IsEdit(false), m_Window(_Window)
	{
		s_Application = this;
		setlocale(LC_ALL, "Russian");

		CreateBackup("start");

		LoadDataBase();

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

		m_MembershipFeeOpeningBalanceTable = CreateRef<SimpleTable>
		(
			std::vector<std::string>{ u8"Номер участка", u8"ФИО", u8"Начальное сальдо" },
			SimpleTable::GEDF_V 
			{
				[=](size_t _Id) { ImGui::TextUnformatted(m_DataBase->GetHomestead(_Id)->GetNumber().data()); },
				[=](size_t _Id) { m_DataBase->GetHomestead(_Id)->DrawFullName(); },
				[=](size_t _Id) 
				{ 
					Money Inter = m_DataBase->GetHomestead(_Id)->GetMembershipFee().GetOpeningBalance();
					if (Inter.DrawEdit())
					{
						m_DataBase->SetMembershipFeeOpeningBalance(_Id, Inter);
					}
				} 
			},
			[=]() { return m_DataBase->GetHomesteadsCount(); }
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

		m_ElectricityOpeningBalanceTable = CreateRef<SimpleTable>
		(
			std::vector<std::string>{ u8"Номер участка", u8"ФИО", u8"Начальное сальдо" },
			SimpleTable::GEDF_V
			{
				[=](size_t _Id) { ImGui::TextUnformatted(m_DataBase->GetHomestead(_Id)->GetNumber().data()); },
				[=](size_t _Id) { m_DataBase->GetHomestead(_Id)->DrawFullName(); },
				[=](size_t _Id)
				{
					Money Inter = m_DataBase->GetHomestead(_Id)->GetElectricity().GetOpeningBalance();
					if (Inter.DrawEdit())
					{
						m_DataBase->SetElectricityOpeningBalance(_Id, Inter);
					}
				}
			},
			[=]() { return m_DataBase->GetHomesteadsCount(); }
		);

		m_HomesteadTabsCollection				= CreateRef<TabsCollection>(u8"участка",	m_HomesteadsTable);
		m_MembershipFeePaymentTabsCollection	= CreateRef<TabsCollection>(u8"платежа",	m_MembershipFeePaymentsTable);
		m_ElectricityAccrualTabsCollection		= CreateRef<TabsCollection>(u8"начисления", m_ElectricityAccrualsTable);
		m_ElectricityPaymentTabsCollection		= CreateRef<TabsCollection>(u8"платежа",	m_ElectricityPaymentsTable);

		m_TabCsvSelector = CreateRef<TabCsvSelector>(m_DataBase);

		Date D1("01.06.2000");
		Date D2("1.10.2001");


		std::istringstream Ostr("da12345");
		int I1;
		Ostr >> I1;
		std::cout << "I1: " << I1 << std::endl;
	}

	Application::~Application()
	{
		CreateBackup("end");
		SaveDataBase();
	}

	void Application::CreateBackup(std::string_view _Param)
	{
		auto LocalTime = Time::GetLocalTime();

		std::ostringstream Filename;
		Filename									<< LocalTime->tm_year + 1900	<< "-"
			<< std::setfill('0') << std::setw(2)	<< LocalTime->tm_mon  + 1		<< "-"
			<< std::setfill('0') << std::setw(2)	<< LocalTime->tm_mday			<< "-"
			<< std::setfill('0') << std::setw(2)	<< LocalTime->tm_hour			<< "-"
			<< std::setfill('0') << std::setw(2)	<< LocalTime->tm_min			<< "-"
			<< std::setfill('0') << std::setw(2)	<< LocalTime->tm_sec			<< (_Param.size() ? "-" : "")
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

	void Application::LoadDataBase()
	{
		m_DataBase = CreateRef<DataBase>();

		JsonLoaderUnloader JSLU("JsDB.json", m_DataBase);
		JSLU.Load();
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
	}

	void Application::OnDropEvent(const std::string& _FileName)
	{
		m_TabCsvSelector->Open(_FileName);

		std::cout << _FileName << std::endl;
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

			m_TabCsvSelector->Draw();

			//if (ImGui::Begin("Tests"))
			//{
			//	for (int i = 0; i < m_Tests.size(); i++)
			//		ImGui::Text(m_Tests[i].c_str());
			//}
			//ImGui::End();

			ImGui::EndTabBar();
		}
		ImGui::End();

		//if (ImGui::Begin("Accruals"))
		//{
		//	auto LocalTime = Time::GetLocalTime();
		//	ImGui::Text("%d%02d%02d", LocalTime->tm_year, LocalTime->tm_mon, LocalTime->tm_mday);
		//	if (ImGui::Button("Sort"))
		//	{
		//		std::sort(MembershipFee::s_Accruals.begin(), MembershipFee::s_Accruals.end(), [](const MembershipFeeAccrual& _First, const MembershipFeeAccrual& _Second)
		//			{
		//				return _First.m_Date < _Second.m_Date;
		//			});
		//	}
		//	for (int i = 0; i < MembershipFee::s_Accruals.size(); i++)
		//	{
		//		ImGui::Text("%03d", i);
		//		ImGui::SameLine();
		//		MembershipFee::s_Accruals[i].m_Date.Draw();
		//		ImGui::SameLine();
		//		MembershipFee::s_Accruals[i].m_Money.Draw();
		//	}
		//}
		//ImGui::End();
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

			auto LocalTime = Time::GetLocalTime();

			const Money Amount = m_DataBase->GetHomestead(m_HomesteadsTable->GetSelectedId())->GetMembershipFee().GetDebt();//m_MembershipFee.m_Debt;
			if (Amount < 0)
			{
				ImGui::Text(u8"Переплата на %02d.%02d.%d:", LocalTime->tm_mday, 1 + LocalTime->tm_mon, 1900 + LocalTime->tm_year); ImGui::SameLine();
				Amount.DrawAbs();
			}
			else
			{
				ImGui::Text(u8"Долг на %02d.%02d.%d:", LocalTime->tm_mday, 1 + LocalTime->tm_mon, 1900 + LocalTime->tm_year); ImGui::SameLine();
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

			Date date = OpeningBalance::GetDate();
			if (date.DrawEdit())
			{
				OpeningBalance::SetDate(date);
			}

			m_MembershipFeeOpeningBalanceTable->Draw();

			ImGui::EndTabItem();
		}
	}

	void Application::DrawCheckMembershipAccrual()
	{
		static std::vector<MembershipFeeAccrual> AccrualsToAdd;
		static std::vector<Date> AccrualsToDelete;
		if (m_OpenedMembershipAccrual)
		{
			m_OpenedMembershipAccrual = false;
			MembershipFee::GetAccrualsToAdd(AccrualsToAdd);
			MembershipFee::GetAccrualsToDelete(AccrualsToDelete);
			if (AccrualsToAdd.size() > 0 || AccrualsToDelete.size() > 0)
			{
				ImGui::OpenPopup(u8"Начисления для добавления##Членские взносы");
			}
		}

		ImGuiPrepareCenteredPopup(800.0f, 500.0f);
		if (ImGui::BeginPopupModal(u8"Начисления для добавления##Членские взносы", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDocking))
		{
			ImGui::Text(u8"Отсутствующие начисления членских взносов для всех участков:");

			ImGui::BeginChild("ChildNew", ImVec2(ImGui::GetWindowContentRegionWidth() * 0.5f, 400), true);
			for (int i = 0; i < AccrualsToAdd.size(); i++)
			{
				ImGui::PushID(i);
				AccrualsToAdd[i].DrawEdit();
				ImGui::SameLine();
				if (ImGui::Button(u8"Добавить"))
				{
					MembershipFee::AddAccrual(AccrualsToAdd[i]);
					AccrualsToAdd.erase(AccrualsToAdd.begin() + i--);
					m_DataBase->RecalculateMembershipFee();
				}
				ImGui::Separator();
				ImGui::PopID();
			}
			ImGui::EndChild();

			ImGui::SameLine();
			ImGui::BeginChild("ChildDel", ImVec2(0, 400), true);
			for (int i = 0; i < AccrualsToDelete.size(); i++)
			{
				ImGui::PushID(i);
				AccrualsToDelete[i].Draw();
				ImGui::SameLine();
				if (ImGui::Button(u8"Удалить"))
				{
					MembershipFee::DeleteAccrual(AccrualsToDelete[i]);
					AccrualsToDelete.erase(AccrualsToDelete.begin() + i--);
					m_DataBase->RecalculateMembershipFee();
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

			auto Hs = m_DataBase->GetHomestead(m_HomesteadsTable->GetSelectedId());

			auto LocalTime = Time::GetLocalTime();

			if (Hs->GetElectricity().GetAll() < 0)
			{
				ImGui::Text(u8"Переплата на %02d.%02d.%d:", LocalTime->tm_mday, 1 + LocalTime->tm_mon, 1900 + LocalTime->tm_year); ImGui::SameLine();
				Hs->GetElectricity().GetAll().DrawAbs();
			}
			else
			{
				ImGui::Text(u8"Долг на %02d.%02d.%d:", LocalTime->tm_mday, 1 + LocalTime->tm_mon, 1900 + LocalTime->tm_year); ImGui::SameLine();
				Hs->GetElectricity().GetAll().DrawAbs();
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

	void Application::SaveDataBase()
	{
		JsonLoaderUnloader jslu("JsDB.json", m_DataBase);
		jslu.Unload();
	}

	void Application::SaveElectricityCSV()
	{
		std::ofstream Fout("Electricity.csv");
		if (!Fout.is_open())
			return;

		const std::string SepCSV = ";";

		Fout << u8"Номер участка" << SepCSV << u8"КВт за все время Общее" << SepCSV << u8"КВт за все время День" << SepCSV << u8"КВт за все время Ночь" << SepCSV;
		Fout << u8"КВт за последий месяц Общее" << SepCSV << u8"КВт за последий месяц День" << SepCSV << u8"КВт за последий месяц Ночь" << SepCSV;

		for (size_t i = 0; i < m_DataBase->GetHomesteadsCount(); i++)
		{
			const auto& Hs = m_DataBase->GetHomestead(i);
			const auto& El = Hs->GetElectricity();
			
			if (El.GetAccrualsCount() < 2)
				continue;

			Fout << Hs->GetNumber()			<< SepCSV;			// Write homestead number

			const auto& AccLast = El.GetAccrual(El.GetAccrualsCount() - 1);
			const auto& AccPreLast = El.GetAccrual(El.GetAccrualsCount() - 2);

			Fout << AccLast->GetAllMonth()	<< SepCSV;			// Write accrual KW (All full monthes)
			Fout << AccLast->GetDay()		<< SepCSV;			// Write accrual KW (All days)
			Fout << AccLast->GetNight()		<< SepCSV;			// Write accrual KW (All nights)

			// Last accruals difference
			KiloWatt DayKW		= AccLast->GetDay()		- AccPreLast->GetDay();
			KiloWatt NightKW	= AccLast->GetNight()	- AccPreLast->GetNight();
			
			Fout << DayKW + NightKW << SepCSV;					// Write accrual KW (Last full monthes)
			Fout << DayKW			<< SepCSV;					// Write accrual KW (Last days)
			Fout << NightKW			<< SepCSV;					// Write accrual KW (Last nights)


		}


		//std::ofstream fout;
		//fout.open("Electricity.csv");
		//
		//const std::string SepCSV = ";";
		//
		//if (!fout.is_open())
		//{
		//	std::cout << "Error: Can't Save file!: " << "Electricity.csv" << std::endl;
		//	return;
		//}
		//std::cout << "File ok! Saving file..." << std::endl;
		//for (auto& homestead : m_DataBase->m_Homesteads)
		//{
		//	if (homestead->m_Electricity.m_Accruals.size() < 2)
		//		continue;
		//
		//	auto& accruals = homestead->m_Electricity.m_Accruals;
		//	auto& accrualBack = accruals.back();
		//	auto& accrualPrev = accruals[accruals.size() - 2];
		//	fout << homestead->m_Data.Number << SepCSV;
		//	fout << accrualBack->GetAllMonth() << SepCSV;
		//	fout << accrualBack->m_Data.Day << SepCSV;
		//	fout << accrualBack->m_Data.Night << SepCSV;
		//
		//	// Разница показаний кВт
		//	KiloWatt dayKW = accrualBack->m_Data.Day - accrualPrev->m_Data.Day;
		//	KiloWatt nightKW = accrualBack->m_Data.Night - accrualPrev->m_Data.Night;
		//	KiloWatt monthKW = dayKW + nightKW;
		//
		//	fout << monthKW << SepCSV;
		//	fout << dayKW << SepCSV;
		//	fout << nightKW << SepCSV;
		//
		//	// Начислено РУБ
		//	Money fullSum;
		//	Money calcDay = Electricity::CalcMonthMoney(dayKW.m_Watt, accrualBack->m_Data.Costs.m_Day);
		//	Money calcNight = Electricity::CalcMonthMoney(nightKW.m_Watt, accrualBack->m_Data.Costs.m_Night);
		//	Money calcLosses = Electricity::CalcLosses(calcDay, calcNight);
		//	calcDay = Electricity::CalcWithBenefits(calcDay, Money(), false /* Homestead benefits 70% */);
		//	calcNight = Electricity::CalcWithBenefits(calcNight, Money(), false /* Homestead benefits 70% */);
		//	Money calcMonth = calcDay + calcNight;
		//	fullSum = calcMonth + calcLosses;
		//
		//	fout << calcMonth << SepCSV;
		//	fout << calcDay << SepCSV;
		//	fout << calcNight << SepCSV;
		//	fout << calcLosses << SepCSV;
		//	for (auto& [name, money] : accrualBack->m_Data.Costs.m_Others)
		//	{
		//		fout << money << SepCSV;
		//		fullSum += money;
		//	}
		//
		//	Money sumToDate = homestead->m_Electricity.CalcAccrualsToDate(accrualPrev->m_Data.Date, homestead->m_Data.ElectricityPrivilege.HasPrivilege);
		//	fullSum += sumToDate;
		//
		//	fout << sumToDate << SepCSV;
		//	fout << fullSum << SepCSV;
		//
		//	bool Payed = false;
		//	Payment LastPay;
		//	for (auto& payment : homestead->m_Electricity.m_Payments)
		//	{
		//		if (payment->m_Data.Date > accrualPrev->m_Data.Date && payment->m_Data.Date <= accrualBack->m_Data.Date)
		//		{
		//			Payed = true;
		//			LastPay.m_Data.Amount += payment->m_Data.Amount;
		//			LastPay.m_Data.Date = payment->m_Data.Date;
		//		}
		//	}
		//
		//	if (Payed)
		//	{
		//		fout << LastPay.m_Data.Date.GetString() << SepCSV;
		//		fout << LastPay.m_Data.Amount << SepCSV;
		//	}
		//	else
		//	{
		//		fout << SepCSV << SepCSV;
		//	}
		//
		//	fout << fullSum - LastPay.m_Data.Amount << std::endl;
		//
		//	std::cout << homestead->m_Electricity.CalcAccrualsToDate(accrualBack->m_Data.Date, homestead->m_Data.ElectricityPrivilege.HasPrivilege) << std::endl;
		//}
		//
		//std::cout << "File Saved!" << std::endl;
		//
		//fout << "Test message! " << std::endl;
		//
		//fout.close();
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
		//std::string fileName = "MembershipFeeDebt" + date.GetString() + ".csv";
		//
		//std::ofstream fout;
		//fout.open(fileName);
		//
		//if (!fout.is_open())
		//{
		//	std::cout << "Error: Can't Save file!: " << fileName << std::endl;
		//	return;
		//}
		//std::cout << "File ok! Saving file..." << std::endl;
		//
		//for (auto& homestead : m_DataBase->m_Homesteads)
		//{
		//	fout << homestead->m_Data.Number << ";";
		//	Money EndSum = homestead->m_MembershipFee.m_OpeningBalance.m_Money;
		//	for (auto& accrual : homestead->m_MembershipFee.s_Accruals)
		//	{
		//		if (accrual.m_Date > date)
		//			break;
		//		EndSum += accrual.m_Money;
		//	}
		//	for (auto& payment : homestead->m_MembershipFee.m_Payments)
		//	{
		//		if (payment->m_Data.Date > date)
		//			break;
		//		EndSum -= payment->m_Data.Amount;
		//	}
		//}
		//
		//std::cout << "File Saved!" << std::endl;
		//fout.close();
	}

	void Application::DrawElectricityOpeningBalance()
	{
		if ((m_IsEdit && m_IsElectricityOpeningBalance) && ImGui::BeginTabItem(u8"Начальное сальдо: электроэнергия"))
		{
			ImGui::Text(u8"Количество участков: %d", m_DataBase->GetHomesteadsCount());

			m_ElectricityOpeningBalanceTable->Draw();

			ImGui::EndTabItem();
		}
	}

}
