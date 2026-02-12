#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_MLSP.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_MLSP::CDB_MLSP()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_mlsp.InitHashTable(HASHSIZEMLSP);
}

CDB_MLSP::~CDB_MLSP()
{

}

void CDB_MLSP::Add(T_MLSP_K Key,T_MLSP_D& rData,CDB_ELEM* pElem)
{
	T_MLSP_D Data;
	BOOL bExist=m_mlsp.Lookup(Key.keymap, Data);
	m_mlsp.SetAt(Key.keymap,rData);
	if(!bExist && Key.key.serial==0)pElem->AddListItem(Key.key.entity, LT_MLSP_CMD, Key.keymap);
}

BOOL CDB_MLSP::Del(T_MLSP_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_mlsp.RemoveKey(Key.keymap);
	ASSERT(ret);
	if(ret && Key.key.serial==0)VERIFY(pElem->DelListItem(Key.key.entity, LT_MLSP_CMD, Key.keymap));
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_MLSP::Get(T_MLSP_K Key,T_MLSP_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_mlsp->Lookup(Key.keymap, uTemp)) return FALSE;
	}
	return m_mlsp.Lookup(Key.keymap,rData);
}

int CDB_MLSP::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_mlsp->GetCount();
	}
	return m_mlsp.GetCount();
}

POSITION CDB_MLSP::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_mlsp->GetStartPosition();
	}
	return m_mlsp.GetStartPosition();
}

void CDB_MLSP::GetNext(POSITION& rNextPosition,T_MLSP_K& rKey,T_MLSP_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_mlsp->GetNextAssoc(rNextPosition, rKey.keymap, uTemp);
		m_mlsp.Lookup(rKey.keymap, rData);
		return;
	}
	m_mlsp.GetNextAssoc(rNextPosition,rKey.keymap,rData);
}
