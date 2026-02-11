#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_DSST.h"

#include "DBDoc.h"
//#include "StagCtrl.h"

CDB_DSST::CDB_DSST()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_dsst.InitHashTable(HASHSIZEMBTP);
}

CDB_DSST::~CDB_DSST()
{

}

void CDB_DSST::Add(T_DSST_K Key, T_DSST_D& rData, CDB_ELEM* pElem)
{
	T_DSST_D Data;
	BOOL bExist = m_dsst.Lookup(Key, Data);
	m_dsst.SetAt(Key, rData);
	if (!bExist)pElem->AddListItem(Key, LT_DSST_CMD, Key);

	//if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
	//{
	//	if (!bExist) m_pDoc->m_pStagCtrl->ActiveDesign(LT_DSST_CMD, Key);
	//}
}

BOOL CDB_DSST::Del(T_DSST_K Key, CDB_ELEM* pElem)
{
	BOOL ret = m_dsst.RemoveKey(Key);
	ASSERT(ret);
	if (ret)
	{
		VERIFY(pElem->DelListItem(Key, LT_DSST_CMD, Key));
		//if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
		//	m_pDoc->m_pStagCtrl->DeactiveDesign(LT_DSST_CMD, Key);
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_DSST::Get(T_DSST_K Key, T_DSST_D& rData)
{
	//if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	//{
	//	if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
	//	UINT uTemp;
	//	if (!m_pDoc->m_pStagCtrl->m_dsst->Lookup(Key, uTemp)) return FALSE;
	//}
	return m_dsst.Lookup(Key, rData);
}

int CDB_DSST::GetCount()
{
	//if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	//{
	//	if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
	//	return m_pDoc->m_pStagCtrl->m_dsst->GetCount();
	//}
	return m_dsst.GetCount();
}

POSITION CDB_DSST::GetStart()
{
	//if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	//{
	//	if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
	//	return m_pDoc->m_pStagCtrl->m_dsst->GetStartPosition();
	//}
	return m_dsst.GetStartPosition();
}

void CDB_DSST::GetNext(POSITION& rNextPosition, T_DSST_K& rKey, T_DSST_D& rData)
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
