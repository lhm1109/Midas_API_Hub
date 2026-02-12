#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_ISST.h"

#include "DBDoc.h"
//#include "StagCtrl.h"

CDB_ISST::CDB_ISST()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_dsst.InitHashTable(HASHSIZEMBTP);
}

CDB_ISST::~CDB_ISST()
{

}

void CDB_ISST::Add(T_ISST_K Key, T_ISST_D& rData, CDB_ELEM* pElem)
{
	T_ISST_D Data;
	BOOL bExist = m_dsst.Lookup(Key, Data);
	m_dsst.SetAt(Key, rData);
	if (!bExist)pElem->AddListItem(Key, LT_ISST_CMD, Key);

	//if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
	//{
	//	if (!bExist) m_pDoc->m_pStagCtrl->ActiveDesign(LT_ISST_CMD, Key);
	//}
}

BOOL CDB_ISST::Del(T_ISST_K Key, CDB_ELEM* pElem)
{
	BOOL ret = m_dsst.RemoveKey(Key);
	ASSERT(ret);
	if (ret)
	{
		VERIFY(pElem->DelListItem(Key, LT_ISST_CMD, Key));
		//if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
		//	m_pDoc->m_pStagCtrl->DeactiveDesign(LT_ISST_CMD, Key);
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_ISST::Get(T_ISST_K Key, T_ISST_D& rData)
{
	//if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	//{
	//	if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
	//	UINT uTemp;
	//	if (!m_pDoc->m_pStagCtrl->m_dsst->Lookup(Key, uTemp)) return FALSE;
	//}
	return m_dsst.Lookup(Key, rData);
}

int CDB_ISST::GetCount()
{
	//if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	//{
	//	if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
	//	return m_pDoc->m_pStagCtrl->m_dsst->GetCount();
	//}
	return m_dsst.GetCount();
}

POSITION CDB_ISST::GetStart()
{
	//if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	//{
	//	if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
	//	return m_pDoc->m_pStagCtrl->m_dsst->GetStartPosition();
	//}
	return m_dsst.GetStartPosition();
}

void CDB_ISST::GetNext(POSITION& rNextPosition, T_ISST_K& rKey, T_ISST_D& rData)
{
	//if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	//{
	//	if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
	//	UINT uTemp;
	//	m_pDoc->m_pStagCtrl->m_dsst->GetNextAssoc(rNextPosition, rKey, uTemp);
	//	m_dsst.Lookup(rKey, rData);
	//	return;
	//}
	m_dsst.GetNextAssoc(rNextPosition, rKey, rData);
}
