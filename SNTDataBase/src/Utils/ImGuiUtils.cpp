#include "ImGuiUtils.h"

#include <iostream>
#include <sstream>

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

	int64_t GetIntFromSplitStr(std::string_view _Str, uint32_t _Precision)
	{
		std::istringstream IStream(_Str.data());
		
		int64_t First = 0;
		IStream >> First;
		
		IStream.ignore(1, '.');

		std::string SecondStr;
		IStream >> SecondStr;
		while (SecondStr.length() < _Precision)
			SecondStr.append("0");
		SecondStr = SecondStr.substr(0, _Precision);
		int64_t Second = std::stoi(SecondStr);
		
		int K = 1;
		for (int i = 0; i < _Precision; i++)
			K *= 10;
		return (First < 0 ? -1 : 1) * (abs(First) * K + abs(Second));
	}

}