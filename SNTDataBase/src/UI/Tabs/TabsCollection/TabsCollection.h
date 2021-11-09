#pragma once

#include <functional>

#include "Core/Base.h"
#include "../TabDataStruct/TabDataStruct.h"
#include "../TabElement.h"
#include "../../Table/Table.h"

namespace LM
{

	class TabsCollection
	{
	public:
		typedef std::function<void(Ref<const TabDataStruct>)> TAB_F;

		TabsCollection(std::string_view _TabName, Ref<Table> _Table);

		void Draw();

		void SetCreate(TAB_F _CreateElement, Ref<TabDataStruct> _TabDataStruct);
		void SetEdit(TAB_F _EditElement, Ref<const TabElement> _TabEditElement);
		void DrawDeleteButton(std::string_view _BtnLable, std::function<void(void)> _DeleteElement, bool _IsSelected);
	protected:
		void DrawCreate();
		void DrawEdit();
		void DrawButtons(std::string_view _BtnOk);
		void DrawPopup(std::function<void(void)> _OkCallback, std::string_view _Desc, std::string_view _BtnOk);
		void NotifyTable();
		void CloseTab(bool _NeedNotifyTable = true);

		void PushId();
		void PopId();
	protected:
		enum class SelectedTab { NONE, CREATE, EDIT };
	protected:
		std::string			m_TabName;
		Ref<Table>			m_Table;
		Ref<TabDataStruct>	m_DataStrucure = nullptr;
		TAB_F				m_Callback = nullptr;
		SelectedTab			m_SelectedTab = SelectedTab::NONE;
	};

}