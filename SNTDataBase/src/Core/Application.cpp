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
#include "Utils/StreamUtils.h"
#include "Utils/Converter.h"

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
			std::vector<std::string>{ u8"����� �������", u8"���", u8"�������� ������", u8"�����������" },
			[=](size_t _Id) { return m_DataBase->GetHomestead(_Id); },
			[=]()			{ return m_DataBase->GetHomesteadsCount(); }
		);

		m_MembershipFeePaymentsTable = CreateRef<Table>
		(
			std::vector<std::string>{ u8"����", u8"�����", u8"����� �������", u8"����� ���������" },
			[=](size_t _Id) { return m_DataBase->GetHomestead(m_HomesteadsTable->GetSelectedId())->GetMembershipFee().GetPayment(_Id); },
			[=]()			{ return m_DataBase->GetHomestead(m_HomesteadsTable->GetSelectedId())->GetMembershipFee().GetPaymentsCount(); }
		);

		m_MembershipFeeOpeningBalanceTable = CreateRef<SimpleTable>
		(
			std::vector<std::string>{ u8"����� �������", u8"���", u8"��������� ������" },
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
			std::vector<std::string>{ u8"����", u8"����", u8"����", u8"�����" },
			[=](size_t _Id) { return m_DataBase->GetHomestead(m_HomesteadsTable->GetSelectedId())->GetElectricity().GetAccrual(_Id); },
			[=]() { return m_DataBase->GetHomestead(m_HomesteadsTable->GetSelectedId())->GetElectricity().GetAccrualsCount(); }
		);

		m_ElectricityPaymentsTable = CreateRef<Table>
		(
			std::vector<std::string>{ u8"����", u8"�����", u8"����� �������", u8"����� ���������" },
			[=](size_t _Id) { return m_DataBase->GetHomestead(m_HomesteadsTable->GetSelectedId())->GetElectricity().GetPayment(_Id); },
			[=]() { return m_DataBase->GetHomestead(m_HomesteadsTable->GetSelectedId())->GetElectricity().GetPaymentsCount(); }
		);

		m_ElectricityOpeningBalanceTable = CreateRef<SimpleTable>
		(
			std::vector<std::string>{ u8"����� �������", u8"���", u8"��������� ������" },
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

		m_HomesteadTabsCollection				= CreateRef<TabsCollection>(u8"�������",	m_HomesteadsTable);
		m_MembershipFeePaymentTabsCollection	= CreateRef<TabsCollection>(u8"�������",	m_MembershipFeePaymentsTable);
		m_ElectricityAccrualTabsCollection		= CreateRef<TabsCollection>(u8"����������", m_ElectricityAccrualsTable);
		m_ElectricityPaymentTabsCollection		= CreateRef<TabsCollection>(u8"�������",	m_ElectricityPaymentsTable);

		m_TabCsvSelector = CreateRef<TabCsvSelector>(m_DataBase);

		m_TabConstants = CreateRef<TabConstants>(m_DataBase);
	}

	Application::~Application()
	{
		CreateBackup("end");
		SaveDataBase();
	}

	void Application::CreateBackup(std::string_view _Param)
	{
		using namespace Stream;
		auto LocalTime = Time::GetLocalTime();

		std::ostringstream Filename;
		Filename			<< LocalTime->tm_year + 1900	<< "-"
			<< Fill('0', 2)	<< LocalTime->tm_mon  + 1		<< "-"
			<< Fill('0', 2)	<< LocalTime->tm_mday			<< "-"
			<< Fill('0', 2)	<< LocalTime->tm_hour			<< "-"
			<< Fill('0', 2)	<< LocalTime->tm_min			<< "-"
			<< Fill('0', 2)	<< LocalTime->tm_sec			<< (_Param.size() ? "-" : "")
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

	void Application::OnDropEvent(std::string_view _FileName)
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
		if (ImGui::BeginMenu(u8"����"))
		{
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
		if (ImGui::BeginMenu(u8"����"))
		{
			if (ImGui::MenuItem(u8"���� ImGui", NULL, &m_ShowDemoWindow))
			{
			}
			if (ImGui::MenuItem(u8"���������", NULL, m_TabConstants->GetIsOpen()))
			{
				m_TabConstants->SwitchOpen();
			}
			ImGui::EndMenu();
		}
	}

	void Application::DrawEditMenu()
	{
		if (ImGui::BeginMenu(u8"������"))
		{
			if (ImGui::MenuItem(u8"����� ��������������", NULL, &m_IsEdit))
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

		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar;
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

		ImGui::Begin(u8"���� �����", &p_open, window_flags);
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
			m_TabConstants->Draw();

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

		if (ImGui::BeginTabItem(u8"�������"))
		{
			ImGui::Text(u8"���������� ��������: %d", m_DataBase->GetHomesteadsCount());

			if (ImGui::Button(u8"�������� �������"))
			{
				m_HomesteadTabsCollection->SetCreate
				(
					[=](Ref<const TabDataStruct> _TabDS) { m_DataBase->AddHomestead(_TabDS); },
					CreateRef<HomesteadTabDS>()
				);
			} 
			ImGui::SameLine();
			if (ImGui::Button(u8"������������� �������") && m_HomesteadsTable->HasSelectedRow())
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
					u8"������� �������",
					[=]() 
					{ 
						m_DataBase->DeleteHomestead(m_HomesteadsTable->GetSelectedId()); 
					},
					m_HomesteadsTable->HasSelectedRow()
				);
			}

			if (m_IsEdit && ImGui::Button(u8"��������� ������: �������� ������"))
			{
				m_IsMembershipFeeOpeningBalance = !m_IsMembershipFeeOpeningBalance;
			}

			if (m_IsEdit && ImGui::Button(u8"��������� ������: ��������������"))
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

		if (ImGui::BeginTabItem(u8"�������� ������"))
		{
			DrawCheckMembershipAccrual();

			auto LocalTime = Time::GetLocalTime();

			const Money Amount = m_DataBase->GetHomestead(m_HomesteadsTable->GetSelectedId())->GetMembershipFee().GetDebt();//m_MembershipFee.m_Debt;
			if (Amount < 0)
			{
				ImGui::Text(u8"��������� �� %02d.%02d.%d:", LocalTime->tm_mday, 1 + LocalTime->tm_mon, 1900 + LocalTime->tm_year); ImGui::SameLine();
				Amount.DrawAbs();
			}
			else
			{
				ImGui::Text(u8"���� �� %02d.%02d.%d:", LocalTime->tm_mday, 1 + LocalTime->tm_mon, 1900 + LocalTime->tm_year); ImGui::SameLine();
				Amount.DrawAbs();
			}

			if (ImGui::Button(u8"�������� ������"))
			{
				m_MembershipFeePaymentTabsCollection->SetCreate
				(
					[=](Ref<const TabDataStruct> _TabDS) { m_DataBase->AddMembershipFeePayment(m_HomesteadsTable->GetSelectedId(), _TabDS); },
					CreateRef<PaymentTabDS>()
				);
			} 
			ImGui::SameLine();
			if (ImGui::Button(u8"������������� ������") && m_MembershipFeePaymentsTable->HasSelectedRow())
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
			if (ImGui::Button(u8"�������� ����������"))
			{
				m_OpenedMembershipAccrual = true;
			}
			if (m_IsEdit)
			{
				ImGui::SameLine();
				m_MembershipFeePaymentTabsCollection->DrawDeleteButton
				(
					u8"������� ������",
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
		if ((m_IsEdit && m_IsMembershipFeeOpeningBalance) && ImGui::BeginTabItem(u8"��������� ������: �������� ������"))
		{
			ImGui::Text(u8"���������� ��������: %d", m_DataBase->GetHomesteadsCount());

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
				ImGui::OpenPopup(u8"���������� ��� ����������##�������� ������");
			}
		}

		ImGuiPrepareCenteredPopup(800.0f, 500.0f);
		if (ImGui::BeginPopupModal(u8"���������� ��� ����������##�������� ������", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDocking))
		{
			ImGui::Text(u8"������������� ���������� �������� ������� ��� ���� ��������:");

			ImGui::BeginChild("ChildNew", ImVec2(ImGui::GetWindowContentRegionWidth() * 0.5f, 400), true);
			for (int i = 0; i < AccrualsToAdd.size(); i++)
			{
				ImGui::PushID(i);
				AccrualsToAdd[i].DrawEdit();
				ImGui::SameLine();
				if (ImGui::Button(u8"��������"))
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
				if (ImGui::Button(u8"�������"))
				{
					MembershipFee::DeleteAccrual(AccrualsToDelete[i]);
					AccrualsToDelete.erase(AccrualsToDelete.begin() + i--);
					m_DataBase->RecalculateMembershipFee();
				}

				ImGui::PopID();
			}
			ImGui::EndChild();

			if (ImGui::Button(u8"�������", ImVec2(120, 0)))
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

		if (ImGui::BeginTabItem(u8"���������� ��������������"))
		{
			//if (!m_HomesteadsTable->HasSelectedRow())
			//{
			//	ImGui::Text(u8"������� �� ������!");
			//	ImGui::EndTabItem();
			//	return;
			//}

			ImGui::TextUnformatted(u8"������ ���������� �������� ��������� ������!");

			if (ImGui::Button(u8"�������� ����������"))
			{
				m_ElectricityAccrualTabsCollection->SetCreate
				(
					[=](Ref<const TabDataStruct> _TabDS) { m_DataBase->AddElectricityAccrual(m_HomesteadsTable->GetSelectedId(), _TabDS); },
					CreateRef<ElectricityAccrualTabDS>()
				);
			} 
			ImGui::SameLine();
			if (ImGui::Button(u8"������������� ����������") && m_ElectricityAccrualsTable->HasSelectedRow())
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
					u8"������� ����������",
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

		if (ImGui::BeginTabItem(u8"������ ��������������"))
		{
			//if (!m_HomesteadsTable->HasSelectedRow())
			//{
			//	ImGui::Text(u8"������� �� ������!");
			//	ImGui::EndTabItem();
			//	return;
			//}

			auto Hs = m_DataBase->GetHomestead(m_HomesteadsTable->GetSelectedId());

			auto LocalTime = Time::GetLocalTime();

			if (Hs->GetElectricity().GetAll() < 0)
			{
				ImGui::Text(u8"��������� �� %02d.%02d.%d:", LocalTime->tm_mday, 1 + LocalTime->tm_mon, 1900 + LocalTime->tm_year); ImGui::SameLine();
				Hs->GetElectricity().GetAll().DrawAbs();
			}
			else
			{
				ImGui::Text(u8"���� �� %02d.%02d.%d:", LocalTime->tm_mday, 1 + LocalTime->tm_mon, 1900 + LocalTime->tm_year); ImGui::SameLine();
				Hs->GetElectricity().GetAll().DrawAbs();
			}
			if (ImGui::Button(u8"�������� ������"))
			{
				m_ElectricityPaymentTabsCollection->SetCreate
				(
					[=](Ref<const TabDataStruct> _TabDS) { m_DataBase->AddElectricityPayment(m_HomesteadsTable->GetSelectedId(), _TabDS); },
					CreateRef<PaymentTabDS>()
				);
			} 
			ImGui::SameLine();
			if (ImGui::Button(u8"������������� ������") && m_ElectricityPaymentsTable->HasSelectedRow())
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
					u8"������� ������",
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
		{
			std::cout << "Can't create file to save electricity!" << std::endl;
			return;
		}

		const std::string SepCSV = ";";

		std::vector<std::string> Names = { "����� �������",
			"��� �� ��� ����� �����", "��� �� ��� ����� ����", "��� �� ��� ����� ����",
			"��� �� �������� ����� �����", "��� �� �������� ����� ����", "��� �� �������� ����� ����",
			"����� �����", "����� ����", "����� ����", "������"
		};
		for (size_t i = 0; i < m_DataBase->GetHomesteadsCount(); i++)
		{
			const auto& Hs = m_DataBase->GetHomestead(i);
			const auto& El = Hs->GetElectricity();

			if (El.GetAccrualsCount() < 2)
				continue;
			
			const auto& AccLast = El.GetAccrual(El.GetAccrualsCount() - 1);
			for (const auto& Other : AccLast->GetCosts().GetOthres())
			{
				Names.push_back(Utf8ToStr(Other.GetName()));
			}
			break;
		}

		Names.insert(Names.end(), { "���������� �����", "������ �����", "������ ����", "������ �����", "���� �� ����������� ����" });
		

		for (const auto& Name : Names)
		{
			Fout << Name << SepCSV;
		}
		Fout << std::endl;

		for (size_t i = 0; i < m_DataBase->GetHomesteadsCount(); i++)
		{
			const auto& Hs = m_DataBase->GetHomestead(i);
			const auto& El = Hs->GetElectricity();
			
			if (El.GetAccrualsCount() < 2)
				continue;

			Fout << Hs->GetNumber()			<< SepCSV;			// Write "����� �������"

			const auto& AccLast = El.GetAccrual(El.GetAccrualsCount() - 1);
			const auto& AccPreLast = El.GetAccrual(El.GetAccrualsCount() - 2);

			Fout << AccLast->GetAllMonth()	<< SepCSV;			// Write "��� �� ��� ����� �����"
			Fout << AccLast->GetDay()		<< SepCSV;			// Write "��� �� ��� ����� ����"
			Fout << AccLast->GetNight()		<< SepCSV;			// Write "��� �� ��� ����� ����"

			// Last accruals difference
			KiloWatt DayKW		= AccLast->GetDay()		- AccPreLast->GetDay();
			KiloWatt NightKW	= AccLast->GetNight()	- AccPreLast->GetNight();
			
			Fout << DayKW + NightKW << SepCSV;					// Write "��� �� �������� ����� �����"
			Fout << DayKW			<< SepCSV;					// Write "��� �� �������� ����� ����"
			Fout << NightKW			<< SepCSV;					// Write "��� �� �������� ����� ����"

			// ��������� ���
			Money DayMoney, NightMoney, LossesMoney;
			El.GetAccrualCsv(El.GetAccrualsCount() - 1, Hs->GetElectricityPrivilege(), &DayMoney, &NightMoney, &LossesMoney);
			
			Fout << DayMoney + NightMoney	<< SepCSV;			// Write "����� �����"
			Fout << DayMoney				<< SepCSV;
			Fout << NightMoney				<< SepCSV;
			Fout << LossesMoney				<< SepCSV;
			for (const auto& Other : AccLast->GetCosts().GetOthres())
			{
				Fout << Other.GetMoney() << SepCSV;
			}

			Money SumToDate = El.CalcAccrualsToDate(AccPreLast->GetDate(), Hs->GetElectricityPrivilege());
			Money FullSum = DayMoney + NightMoney + LossesMoney + SumToDate + AccLast->GetCosts().GetOthersSum();
			
			Fout << SumToDate	<< SepCSV;
			Fout << FullSum		<< SepCSV;
			
			Money LastPaysSum(0);
			Date  LastPaysDate;
			for (int j = 0; j < El.GetPaymentsCount(); j++)
			{
				if (El.GetPayment(j)->GetDate() > AccPreLast->GetDate() && El.GetPayment(j)->GetDate() <= AccLast->GetDate())
				{
					LastPaysSum += El.GetPayment(j)->GetAmount();
					LastPaysDate = El.GetPayment(j)->GetDate();
				}
			}

			if (LastPaysSum != Money(0))
			{
				Fout << LastPaysDate	<< SepCSV;
				Fout << LastPaysSum		<< SepCSV;
			}
			else
			{
				Fout << SepCSV << SepCSV;
			}

			Fout << FullSum - LastPaysSum << SepCSV;
			Fout << std::endl;
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
		//	// ������� ��������� ���
		//	KiloWatt dayKW = accrualBack->m_Data.Day - accrualPrev->m_Data.Day;
		//	KiloWatt nightKW = accrualBack->m_Data.Night - accrualPrev->m_Data.Night;
		//	KiloWatt monthKW = dayKW + nightKW;
		//
		//	fout << monthKW << SepCSV;
		//	fout << dayKW << SepCSV;
		//	fout << nightKW << SepCSV;
		//
		//	// ��������� ���
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
		// ??? �������

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
		if ((m_IsEdit && m_IsElectricityOpeningBalance) && ImGui::BeginTabItem(u8"��������� ������: ��������������"))
		{
			ImGui::Text(u8"���������� ��������: %d", m_DataBase->GetHomesteadsCount());

			m_ElectricityOpeningBalanceTable->Draw();

			ImGui::EndTabItem();
		}
	}

}
