#include "stdafx.h"
#include "DB_NODE.h"
#include "DB_GSPR.h"
#include "DB_GSTP.h"
#include "DB_BNGR.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_GSPR::CDB_GSPR()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_gspr.InitHashTable(HASHSIZEGSPR);
}

CDB_GSPR::~CDB_GSPR()
{

}

void CDB_GSPR::Add(T_GSPR_K Key,T_GSPR_D& rData,CDB_GSTP* pGstp,CDB_NODE* pNode,CDB_BNGR* pBngr)
{
	T_GSPR_D Data;
	BOOL bExist=m_gspr.Lookup(Key.keymap, Data);
	if(bExist)
	{
		pGstp->DelListItem(Data.SpringTypeKey, LT_GSPR_CMD, Key.keymap);
		pBngr->DelListItem(Data.GroupKey, LT_GSPR_CMD, Key.keymap);
	}
	m_gspr.SetAt(Key.keymap,rData);
	pGstp->AddListItem(rData.SpringTypeKey, LT_GSPR_CMD, Key.keymap);
	pBngr->AddListItem(rData.GroupKey, LT_GSPR_CMD, Key.keymap);
	if(!bExist && pNode != NULL)pNode->AddListItem(Key.key.entity, LT_GSPR_CMD, Key.keymap);

	// Current Stage가 Base Stage가 아니면 Stage DB를 Update한다.
	// Boundary는 현재 Active된 그룹에 속하는 것만 Add될 수 있으므로
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveBndr(LT_GSPR_CMD, Key.keymap, rData.GroupKey);
	}
}

BOOL CDB_GSPR::Del(T_GSPR_K Key,CDB_GSTP* pGstp,CDB_NODE* pNode,CDB_BNGR* pBngr)
{
	T_GSPR_D Data;
	BOOL bExist=m_gspr.Lookup(Key.keymap, Data);
	if(bExist)
	{
		BOOL ret=m_gspr.RemoveKey(Key.keymap);
		ASSERT(ret);
		if(ret)
		{
			VERIFY(pGstp->DelListItem(Data.SpringTypeKey, LT_GSPR_CMD, Key.keymap));
			VERIFY(pBngr->DelListItem(Data.GroupKey, LT_GSPR_CMD, Key.keymap));
			if(pNode != NULL)VERIFY(pNode->DelListItem(Key.key.entity, LT_GSPR_CMD, Key.keymap));

			// Current Stage가 Base Stage가 아니면 Stage DB를 Update한다.
			if (m_pDoc->m_pStagCtrl->GetCurStag() != 0)
			{
				m_pDoc->m_pStagCtrl->DeactiveBndr(LT_GSPR_CMD, Key.keymap);
			}
		}
		return ret;
	}
	return bExist;
}

//--------------------------------------------------------------------------
BOOL CDB_GSPR::Get(T_GSPR_K Key,T_GSPR_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		T_KEY2 StagBngrKey;
		if (!m_pDoc->m_pStagCtrl->m_gspr->Lookup(Key.keymap, StagBngrKey)) return FALSE;
	}
	return m_gspr.Lookup(Key.keymap,rData);
}

int CDB_GSPR::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_gspr->GetCount();
	}
	return m_gspr.GetCount();
}

POSITION CDB_GSPR::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_gspr->GetStartPosition();
	}
	return m_gspr.GetStartPosition();
}

void CDB_GSPR::GetNext(POSITION& rNextPosition,T_GSPR_K& rKey,T_GSPR_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		T_KEY2 StagBngrKey;
		m_pDoc->m_pStagCtrl->m_gspr->GetNextAssoc(rNextPosition, rKey.keymap, StagBngrKey);
		m_gspr.Lookup(rKey.keymap, rData);
		return;
	}
	m_gspr.GetNextAssoc(rNextPosition,rKey.keymap,rData);
}

