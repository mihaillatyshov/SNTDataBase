#pragma once

#include <vector>

#include "../Homestead/Homestead.h"
#include "Core/Base.h"

namespace LM
{

	class DataBase
	{
	public:
		void AddHomestead(const Homestead& _Homestead);
		void AddHomestead(Ref<const TabDataStruct> _TabDS);
		void EditHomestead(size_t _HsId, Ref<const TabDataStruct> _TabDS);
		void DeleteHomestead(size_t _HsId);

		void AddMembershipFeePayment(size_t _HsId, Ref<const TabDataStruct> _TabDS);
		void EditMembershipFeePayment(size_t _HsId, size_t _PayId, Ref<const TabDataStruct> _TabDS);
		void DeleteMembershipFeePayment(size_t _HsId, size_t _PayId);
		void SetMembershipFeeOpeningBalance(size_t _HsId, const Money& _Money);

		void AddElectricityAccrual(size_t _HsId, Ref<const TabDataStruct> _TabDS);
		void EditElectricityAccrual(size_t _HsId, size_t _AccId, Ref<const TabDataStruct> _TabDS);
		void DeleteElectricityAccrual(size_t _HsId, size_t _AccId);

		void AddElectricityPayment(size_t _HsId, Ref<const TabDataStruct> _TabDS);
		void EditElectricityPayment(size_t _HsId, size_t _PayId, Ref<const TabDataStruct> _TabDS);
		void DeleteElectricityPayment(size_t _HsId, size_t _PayId);

		void SetElectricityOpeningBalance(size_t _HsId, const Money& _Money);

		const Ref<const Homestead> GetHomestead(size_t _Id) const { return m_Homesteads[_Id]; }
		size_t GetHomesteadsCount() const { return m_Homesteads.size(); }


		void RecalculateMembershipFee();
		void RecalculateElectricity();

		//std::vector<Homestead>& GetHomesteads() { return m_Homesteads; }

		VectorRef<Homestead> m_Homesteads;
	};

}
