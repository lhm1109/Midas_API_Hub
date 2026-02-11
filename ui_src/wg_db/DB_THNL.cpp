#include "stdafx.h"
#include "DB_THIS.h"
#include "DB_NODE.h"
#include "DB_THNL.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_THNL::CDB_THNL()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_thnl.InitHashTable(HASHSIZETHNL);
}

CDB_THNL::~CDB_THNL()
{

}

void CDB_THNL::Add(T_THNL_K Key,T_THNL_D& rData,CDB_THIS* pThis,CDB_NODE* pNode)
{
	T_THNL_D Data;
	BOOL bExist=m_thnl.Lookup(Key.keymap, Data);
	if(bExist && Data.LoadCaseKey != rData.LoadCaseKey)
		VERIFY(pThis->DelListItem(Data.LoadCaseKey, LT_THNL_CMD, Key.keymap));
	m_thnl.SetAt(Key.keymap,rData);
	if(bExist && Data.LoadCaseKey != rData.LoadCaseKey)
		pThis->AddListItem(rData.LoadCaseKey, LT_THNL_CMD, Key.keymap);
	if(!bExist)
	{
		pThis->AddListItem(rData.LoadCaseKey, LT_THNL_CMD, Key.keymap);
		pNode->AddListItem(Key.key.entity, LT_THNL_CMD, Key.keymap);
	}
}

BOOL CDB_THNL::Del(T_THNL_K Key,CDB_THIS* pThis,CDB_NODE* pNode)
{
	T_THNL_D Data;
	BOOL bExist=m_thnl.Lookup(Key.keymap, Data);
	if(bExist)
	{
		BOOL ret=m_thnl.RemoveKey(Key.keymap);
		ASSERT(ret);
		if(ret)
		{
			VERIFY(pThis->DelListItem(Data.LoadCaseKey, LT_THNL_CMD, Key.keymap));
			VERIFY(pNode->DelListItem(Key.key.entity, LT_THNL_CMD, Key.keymap));
		}
		return ret;
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_THNL::Get(T_THNL_K Key,T_THNL_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_thnl->Lookup(Key.keymap, uTemp)) return FALSE;
	}
	return m_thnl.Lookup(Key.keymap,rData);
}

int CDB_THNL::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_thnl->GetCount();
	}
	return m_thnl.GetCount();
}

POSITION CDB_THNL::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_thnl->GetStartPosition();
	}
	return m_thnl.GetStartPosition();
}

void CDB_THNL::GetNext(POSITION& rNextPosition,T_THNL_K& rKey,T_THNL_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_thnl->GetNextAssoc(rNextPosition, rKey.keymap, uTemp);
		m_thnl.Lookup(rKey.keymap, rData);
		return;
	}
	m_thnl.GetNextAssoc(rNextPosition,rKey.keymap,rData);
}
