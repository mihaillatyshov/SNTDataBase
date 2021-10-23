#pragma once

namespace LM
{

	class ITable 
	{
	public:
		ITable();
		virtual ~ITable();

		inline int GetSelectedId() { return m_SelectedId; }
		inline void SetUnselected() { m_SelectedId = -1; }

		void Draw();

	protected:
		virtual void DrawCenter() = 0;
		virtual int GetRawCount() = 0;
	protected:
		int m_SelectedId;
	};

}