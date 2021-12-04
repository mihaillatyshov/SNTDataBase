#pragma once

#include <imgui.h>
#include <vector>

namespace LM
{

	class ITable
	{
	public:
		ITable(const std::vector<std::string>& _Names) : m_Names(_Names) { };
		virtual ~ITable() = default;

		virtual void Draw() = 0;
	protected:
		static inline ImGuiTableFlags s_Flags = ImGuiTableFlags_Resizable
			| ImGuiTableFlags_Sortable | ImGuiTableFlags_SortMulti
			| ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders
			| ImGuiTableFlags_ScrollY
			| ImGuiTableFlags_SizingFixedFit;
	protected:
		std::vector<std::string> m_Names;

		ImGuiListClipper m_Clipper;
	};

}