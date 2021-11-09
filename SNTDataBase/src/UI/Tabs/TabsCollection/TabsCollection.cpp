#include "TabsCollection.h"

namespace LM
{

	TabsCollection::TabsCollection(std::string_view _TabName, Ref<Table> _Table)
		: m_TabName(_TabName), m_Table(_Table)
	{

	}

	void TabsCollection::Draw()
	{
		PushId();

		switch (m_SelectedTab)
		{
		case SelectedTab::CREATE:
			DrawCreate();
			break;
		case SelectedTab::EDIT:
			DrawEdit();
			break;
		default:
			break;
		}

		PopId();
	}

	void TabsCollection::SetCreate(TAB_F _CreateElement, Ref<TabDataStruct> _TabDataStruct)
	{
		m_SelectedTab = SelectedTab::CREATE;
		m_Callback = _CreateElement;
		m_DataStrucure = _TabDataStruct;
	}

	void TabsCollection::SetEdit(TAB_F _EditElement, Ref<const TabElement> _TabElement)
	{
		m_SelectedTab = SelectedTab::EDIT;
		m_Callback = _EditElement;
		_TabElement->FillDataStruct(m_DataStrucure);
	}

	void TabsCollection::DrawDeleteButton(std::string_view _BtnLable, std::function<void(void)> _DeleteElement, bool _IsSelected)
	{
		PushId();

		if (ImGui::Button(_BtnLable.data()) && _IsSelected)
			ImGui::OpenPopup(u8"�������?");

		DrawPopup
		(
			[=]() 
			{ 
				_DeleteElement();
				NotifyTable();
			}, 
			u8"�������� ��� ����������� ��������������!\n\n\n\n\n", 
			u8"�������"
		);

		PopId();
	}

	void TabsCollection::DrawCreate()
	{
		if (ImGui::BeginTabItem((u8"���������� " + m_TabName).data(), NULL, ImGuiTabItemFlags_SetSelected))
		{
			m_DataStrucure->Draw();
			DrawButtons(u8"��������");

			ImGui::EndTabItem();
		}
	}

	void TabsCollection::DrawEdit()
	{
		if (ImGui::BeginTabItem((u8"�������������� " + m_TabName).data(), NULL, ImGuiTabItemFlags_SetSelected))
		{
			m_DataStrucure->Draw();
			DrawButtons(u8"��������");

			ImGui::EndTabItem();
		}
	}

	void TabsCollection::DrawButtons(std::string_view _BtnOk)
	{
		if (ImGui::Button(_BtnOk.data(), ImVec2(120, 0)))
		{
			m_Callback(m_DataStrucure);
			CloseTab();
		}
		ImGui::SameLine();
		if (ImGui::Button(u8"��������", ImVec2(120, 0)))
		{
			if (m_DataStrucure->IsDirty())
				ImGui::OpenPopup(u8"�������?");
			else
				CloseTab(false);
		}

		DrawPopup([=]() { CloseTab(false); }, u8"��� ������ ����� ������� \n��� ����������� ��������������!\n\n\n\n", u8"��");
	}

	void TabsCollection::DrawPopup(std::function<void(void)> _OkCallback, std::string_view _Desc, std::string_view _BtnOk)
	{
		// Setup Popup
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImVec2 window_size = ImVec2(400, 150);
		ImVec2 Distance = ImVec2(viewport->Size.x / 2, viewport->Size.y / 2);
		ImVec2 window_pos = ImVec2(viewport->Pos.x + Distance.x - window_size.x / 2, viewport->Pos.y + Distance.y - window_size.y / 2);
		ImVec2 window_pos_pivot = ImVec2(0.0f, 0.0f);
		ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
		ImGui::SetNextWindowSize(window_size);
		//ImGui::SetNextWindowPos();
		ImGui::SetNextWindowViewport(viewport->ID);

		// Draw Popup
		if (ImGui::BeginPopupModal(u8"�������?", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDocking))
		{
			ImGui::Text(_Desc.data());
			ImGui::Separator();

			if (ImGui::Button(_BtnOk.data(), ImVec2(120, 0)))
			{
				_OkCallback();
				ImGui::CloseCurrentPopup();
			}
			ImGui::SetItemDefaultFocus();
			ImGui::SameLine();
			if (ImGui::Button(u8"������", ImVec2(120, 0)))
			{
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
	}

	void TabsCollection::NotifyTable()
	{
		m_Table->SetUnselected();
	}

	void TabsCollection::CloseTab(bool _NeedNotifyTable)
	{
		if (_NeedNotifyTable)
			NotifyTable();
		m_SelectedTab = SelectedTab::NONE;
		m_Callback = nullptr;
		m_DataStrucure = nullptr;
	}

	void TabsCollection::PushId()
	{
		ImGui::PushID(m_TabName.data());
		ImGui::PushID((int)m_SelectedTab);
	}

	void TabsCollection::PopId()
	{
		ImGui::PopID();
		ImGui::PopID();
	}

}