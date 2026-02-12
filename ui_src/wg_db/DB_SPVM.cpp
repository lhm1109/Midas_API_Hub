#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_SPVM.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_SPVM::CDB_SPVM()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_spvm.InitHashTable(HASHSIZESPVM);
}

CDB_SPVM::~CDB_SPVM()
{

}

void CDB_SPVM::Add(T_SPVM_K Key,T_SPVM_D& rData,CDB_ELEM* pElem)
{
	T_SPVM_D Data;
	BOOL bExist=m_spvm.Lookup(Key, Data);
	m_spvm.SetAt(Key,rData);
	if(!bExist)pElem->AddListItem(Key, LT_SPVM_CMD, Key);

	// Current Stage가 Final Stage이면 Stage DB를 Update한다.
	// 이미 있는 것이 아니면 새로 Active시킨다.
	/*if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveDesign(LT_SPVM_CMD, Key);
	}*/
}

BOOL CDB_SPVM::Del(T_SPVM_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_spvm.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		VERIFY(pElem->DelListItem(Key, LT_SPVM_CMD, Key));
		// Current Stage가 Final Stage이면 Stage DB를 Update한다.
		if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
			m_pDoc->m_pStagCtrl->DeactiveDesign(LT_SPVM_CMD, Key);
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_SPVM::Get(T_SPVM_K Key,T_SPVM_D& rData)
{
	/*if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_spvm->Lookup(Key, uTemp)) return FALSE;
	}*/
	return m_spvm.Lookup(Key,rData);
}

int CDB_SPVM::GetCount()
{
	/*if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_spvm->GetCount();
	}*/
	return m_spvm.GetCount();
}

POSITION CDB_SPVM::GetStart()
{
	/*if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_spvm->GetStartPosition();
	}*/
	return m_spvm.GetStartPosition();
}

void CDB_SPVM::GetNext(POSITION& rNextPosition,T_SPVM_K& rKey,T_SPVM_D& rData)
{
	//if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	//{
	//	if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
	//	UINT uTemp;
	//	m_pDoc->m_pStagCtrl->m_spvm->GetNextAssoc(rNextPosition, rKey, uTemp);
	//	m_spvm.Lookup(rKey, rData);
	//	return;
	//}
	m_spvm.GetNextAssoc(rNextPosition,rKey,rData);
}
