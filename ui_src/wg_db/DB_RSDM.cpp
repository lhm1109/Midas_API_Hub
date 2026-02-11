#include "stdafx.h"
#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_ELNK.h"
#include "DB_NLNK.h"
#include "DB_RSDM.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_RSDM::CDB_RSDM()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_rsdm.InitHashTable(HASHSIZERSDM);
}

CDB_RSDM::~CDB_RSDM()
{

}

void CDB_RSDM::Add(T_RSDM_K Key,T_RSDM_D& rData, CDB_ELEM* pElem, CDB_ELNK* pElnk, CDB_NLNK* pNlnk)
{
	T_RSDM_D Data;
	BOOL bExist=m_rsdm.Lookup(Key.keymap, Data);
	m_rsdm.SetAt(Key.keymap,rData);
	if(!bExist)
	{
		switch(Key.key.OType)
		{
		case 1: // Element
			pElem->AddListItem(Key.key.OKey, LT_RSDM_CMD, Key.keymap);
			break;
		case 2: // Elastic Link
			pElnk->AddListItem(Key.key.OKey, LT_RSDM_CMD, Key.keymap);
			break;
		case 3: // General Link
			pNlnk->AddListItem(Key.key.OKey, LT_RSDM_CMD, Key.keymap);
			break;
		default:
			ASSERT(0);
			break;
		}
	}
}

BOOL CDB_RSDM::Del(T_RSDM_K Key, CDB_ELEM* pElem, CDB_ELNK* pElnk, CDB_NLNK* pNlnk)
{
	T_RSDM_D Data;
	BOOL bExist=m_rsdm.Lookup(Key.keymap, Data);
	if(bExist)
	{
		BOOL ret = m_rsdm.RemoveKey(Key.keymap);
		ASSERT(ret);
		if(ret)
		{
			switch(Key.key.OType)
			{
			case 1: // Element
				VERIFY(pElem->DelListItem(Key.key.OKey, LT_RSDM_CMD, Key.keymap));
				break;
			case 2: // Elastic Link
				VERIFY(pElnk->DelListItem(Key.key.OKey, LT_RSDM_CMD, Key.keymap));
				break;
			case 3: // General Link
				VERIFY(pNlnk->DelListItem(Key.key.OKey, LT_RSDM_CMD, Key.keymap));
				break;
			default:
				ASSERT(0);
				break;
			}
		}
		return ret;
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_RSDM::Get(T_RSDM_K Key,T_RSDM_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_rsdm.Lookup(Key.keymap,rData);
}

int CDB_RSDM::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_rsdm.GetCount();
}

POSITION CDB_RSDM::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_rsdm.GetStartPosition();
}

void CDB_RSDM::GetNext(POSITION& rNextPosition,T_RSDM_K& rKey,T_RSDM_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_rsdm.GetNextAssoc(rNextPosition,rKey.keymap,rData);
}

