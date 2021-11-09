#include "DataBase.h"

namespace LM
{

	void DataBase::AddHomestead(const Homestead& _Homestead)
	{
		m_Homesteads.push_back(CreateRef<Homestead>(_Homestead));
	}
	
	void DataBase::AddHomestead(Ref<const TabDataStruct> _TabDS)
	{
		m_Homesteads.push_back(CreateRef<Homestead>(_TabDS));
	}

	void DataBase::EditHomestead(size_t _HsId, Ref<const TabDataStruct> _TabDS)
	{
		m_Homesteads[_HsId]->Edit(_TabDS);
	}

	void DataBase::DeleteHomestead(size_t _HsId)
	{
		m_Homesteads.erase(m_Homesteads.begin() + _HsId);
	}

	void DataBase::AddMembershipFeePayment(size_t _HsId, Ref<const TabDataStruct> _TabDS)
	{
		m_Homesteads[_HsId]->AddMembershipFeePayment(_TabDS);
	}

	void DataBase::EditMembershipFeePayment(size_t _HsId, size_t _PayId, Ref<const TabDataStruct> _TabDS)
	{
		m_Homesteads[_HsId]->EditMembershipFeePayment(_PayId, _TabDS);
	}

	void DataBase::DeleteMembershipFeePayment(size_t _HsId, size_t _PayId)
	{
		m_Homesteads[_HsId]->DeleteMembershipFeePayment(_PayId);
	}

	void DataBase::AddElectricityAccrual(size_t _HsId, Ref<const TabDataStruct> _TabDS)
	{
		m_Homesteads[_HsId]->AddElectricityAccrual(_TabDS);
	}

	void DataBase::EditElectricityAccrual(size_t _HsId, size_t _AccId, Ref<const TabDataStruct> _TabDS)
	{
		m_Homesteads[_HsId]->EditElectricityAccrual(_AccId, _TabDS);

	}

	void DataBase::DeleteElectricityAccrual(size_t _HsId, size_t _AccId)
	{
		m_Homesteads[_HsId]->DeleteElectricityAccrual(_AccId);
	}

	void DataBase::AddElectricityPayment(size_t _HsId, Ref<const TabDataStruct> _TabDS)
	{
		m_Homesteads[_HsId]->AddElectricityPayment(_TabDS);
	}

	void DataBase::EditElectricityPayment(size_t _HsId, size_t _PayId, Ref<const TabDataStruct> _TabDS)
	{
		m_Homesteads[_HsId]->EditElectricityPayment(_PayId, _TabDS);
	}

	void DataBase::DeleteElectricityPayment(size_t _HsId, size_t _PayId)
	{
		m_Homesteads[_HsId]->DeleteElectricityPayment(_PayId);
	}

}
