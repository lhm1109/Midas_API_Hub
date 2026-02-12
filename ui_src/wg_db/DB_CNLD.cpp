#include "stdafx.h"
#include "DB_STLD.h"
#include "DB_NODE.h"
#include "DB_CNLD.h"
#include "DB_LDGR.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_CNLD::CDB_CNLD()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_cnld.InitHashTable(HASHSIZECNLD);
}

CDB_CNLD::~CDB_CNLD()
{

}

void CDB_CNLD::Add(T_CNLD_K Key,T_CNLD_D& rData,CDB_STLD* pStld,CDB_NODE* pNode,CDB_LDGR* pLdgr)
{
	T_CNLD_D Data;
	BOOL bExist=m_cnld.Lookup(Key.keymap, Data);
	if(bExist && Data.LoadCaseKey != rData.LoadCaseKey)
		VERIFY(pStld->DelListItem(Data.LoadCaseKey, LT_CNLD_CMD, Key.keymap));
	if(bExist && Data.GroupKey != rData.GroupKey)
		VERIFY(pLdgr->DelListItem(Data.GroupKey, LT_CNLD_CMD, Key.keymap));
	m_cnld.SetAt(Key.keymap,rData);
	if(bExist)
	{
		UpdateType(Key,Data,TRUE);
	}
	UpdateType(Key,rData,FALSE);
	if(bExist && Data.LoadCaseKey != rData.LoadCaseKey)
		pStld->AddListItem(rData.LoadCaseKey, LT_CNLD_CMD, Key.keymap);
	if(bExist && Data.GroupKey != rData.GroupKey)
		pLdgr->AddListItem(rData.GroupKey, LT_CNLD_CMD, Key.keymap);
	if(!bExist)
	{
		pStld->AddListItem(rData.LoadCaseKey, LT_CNLD_CMD, Key.keymap);
		pNode->AddListItem(Key.key.entity, LT_CNLD_CMD, Key.keymap);
		pLdgr->AddListItem(rData.GroupKey, LT_CNLD_CMD, Key.keymap);
	}

	// Current Stage가 Base Stage가 아니면 Stage DB를 Update한다.
	// Load는 현재 Active된 그룹에 속하는 것만 Add될 수 있으므로
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveLoad(LT_CNLD_CMD, Key.keymap, rData.GroupKey);
	}
}

BOOL CDB_CNLD::Del(T_CNLD_K Key,CDB_STLD* pStld,CDB_NODE* pNode,CDB_LDGR* pLdgr)
{
	T_CNLD_D Data;
	BOOL bExist=m_cnld.Lookup(Key.keymap, Data);
	if(bExist)
	{
		BOOL ret=m_cnld.RemoveKey(Key.keymap);
		UpdateType(Key,Data,TRUE);
		ASSERT(ret);
		if(ret)
		{
			VERIFY(pStld->DelListItem(Data.LoadCaseKey, LT_CNLD_CMD, Key.keymap));
			VERIFY(pNode->DelListItem(Key.key.entity, LT_CNLD_CMD, Key.keymap));
			VERIFY(pLdgr->DelListItem(Data.GroupKey, LT_CNLD_CMD, Key.keymap));

			// Current Stage가 Base Stage가 아니면 Stage DB를 Update한다.
			if (m_pDoc->m_pStagCtrl->GetCurStag() != 0)
			{
				m_pDoc->m_pStagCtrl->DeactiveLoad(LT_CNLD_CMD, Key.keymap);
			}
		}
		return ret;
	}
	return bExist;
}

//--------------------------------------------------------------------------
BOOL CDB_CNLD::Get(T_CNLD_K Key,T_CNLD_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		T_KEY2 StagLdgrKey;
		if (!m_pDoc->m_pStagCtrl->m_cnld->Lookup(Key.keymap, StagLdgrKey)) return FALSE;
	}
	return m_cnld.Lookup(Key.keymap,rData);
}

int CDB_CNLD::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_cnld->GetCount();
	}
	return m_cnld.GetCount();
}

POSITION CDB_CNLD::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_cnld->GetStartPosition();
	}
	return m_cnld.GetStartPosition();
}

void CDB_CNLD::GetNext(POSITION& rNextPosition,T_CNLD_K& rKey,T_CNLD_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		T_KEY2 StagLdgrKey;
		m_pDoc->m_pStagCtrl->m_cnld->GetNextAssoc(rNextPosition, rKey.keymap, StagLdgrKey);
		m_cnld.Lookup(rKey.keymap, rData);
		return;
	}
	m_cnld.GetNextAssoc(rNextPosition,rKey.keymap,rData);
}

void CDB_CNLD::UpdateType(T_CNLD_K Key,T_CNLD_D& rData, BOOL bDelete)
{
#ifdef _CH
	if(rData.strType != _T(""))//Using strType.IsEmpty() is wrong;The Length not Is 0
	{
		CnldTypeList* pList = NULL;
		CString strTypeKey = rData.strType;
		if(bDelete)
		{
			if(!m_cnldType.Lookup(strTypeKey,pList))
				return;
			if(pList->IsEmpty())
			{
				ASSERT(FALSE);
				return;
			}
			for(int i = pList->GetSize()-1;i >= 0;i--)
			{
				if(pList->GetAt(i) == Key.keymap)
					pList->RemoveAt(i);
			}
			if(pList->IsEmpty())
			{
				delete pList;
				m_cnldType.RemoveKey(strTypeKey);
			}
		}
		else
		{
			if(!m_cnldType.Lookup(strTypeKey,pList))
			{
				pList = new CnldTypeList;
				m_cnldType[rData.strType] = pList;
			}
			pList->Add(Key.keymap);
		}
	}
#endif
}

//---------------------------Cnld Type-----------------------------------------------
BOOL CDB_CNLD::GetType(const CString& Key,CnldTypeList& pData)
{
	CnldTypeList* pOriData = 0;
	if(m_cnldType.Lookup(Key,pOriData))
	{
		pData.Copy(*pOriData);
		return TRUE;
	}
	return FALSE;
}

int CDB_CNLD::GetTypeCount()
{
	return m_cnldType.GetCount();
}

POSITION CDB_CNLD::GetTypeStart()
{
	return m_cnldType.GetStartPosition();
}

void CDB_CNLD::GetTypeNext(POSITION& rNextPosition,CString& rKey,CnldTypeList*& pData)
{
	m_cnldType.GetNextAssoc(rNextPosition,rKey,pData);
}

