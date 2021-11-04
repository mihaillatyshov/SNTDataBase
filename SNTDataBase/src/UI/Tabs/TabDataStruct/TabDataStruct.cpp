#include "TabDataStruct.h"

namespace LM
{

	bool TabDataStruct::InputTextString(std::string_view _Name, std::string& _Data, const int _ItemWidth, const ImGuiInputTextFlags_ _Flag)
	{
		if (_ItemWidth)
			ImGui::PushItemWidth(_ItemWidth);

		int BufferSize = _Data.size() + 128;
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