#include "ImGuiUtils.h"

namespace LM
{

	void ImGuiPrepareCenteredPopup(float _X, float _Y)
	{
		ImGuiViewport* Viewport = ImGui::GetMainViewport();
		ImVec2 WindowSize = ImVec2(_X, _Y);
		ImVec2 Distance = ImVec2(Viewport->Size.x / 2, Viewport->Size.y / 2);
		ImVec2 WindowPos = ImVec2(Viewport->Pos.x + Distance.x - WindowSize.x / 2, Viewport->Pos.y + Distance.y - WindowSize.y / 2);
		ImGui::SetNextWindowPos(WindowPos);
		ImGui::SetNextWindowSize(WindowSize);
		ImGui::SetNextWindowViewport(Viewport->ID);
	}

	bool ImGuiInputTextString(std::string_view _Name, std::string& _Data, const float _ItemWidth, const ImGuiInputTextFlags_ _Flag)
	{
		if (_ItemWidth)
			ImGui::PushItemWidth(_ItemWidth);

		size_t BufferSize = _Data.size() + 128;
		char* Buffer = new char[BufferSize] { 0 };
		memcpy(Buffer, _Data.data(), _Data.size());

		bool isStringEdited = ImGui::InputText(_Name.data(), Buffer, BufferSize, _Flag);

		_Data = Buffer;
		delete[] Buffer;

		if (_ItemWidth)
			ImGui::PopItemWidth();

		return isStringEdited;
	}
}