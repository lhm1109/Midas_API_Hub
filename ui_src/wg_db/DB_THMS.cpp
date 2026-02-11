#include "stdafx.h"
#include "DB_THIS.h"
#include "DB_THMS.h"
#include "DB_NODE.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_THMS::CDB_THMS()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_thms.InitHashTable(HASHSIZETHMS);
}

CDB_THMS::~CDB_THMS()
{

}

void CDB_THMS::Add(T_THMS_K Key,T_THMS_D& rData,CDB_THIS* pThis, CDB_NODE* pNode)
{
	T_THMS_D Data;
	BOOL bExist=m_thms.Lookup(Key.keymap, Data);
	if(bExist && Data.ThisKey != rData.ThisKey)
		VERIFY(pThis->DelListItem(Data.ThisKey, LT_THIS_CMD, Key.keymap));
	m_thms.SetAt(Key.keymap,rData);  
	if(bExist && Data.ThisKey != rData.ThisKey)
		pThis->AddListItem(rData.ThisKey, LT_THMS_CMD, Key.keymap);
	if(!bExist)
	{    
		pThis->AddListItem(rData.ThisKey, LT_THMS_CMD, Key.keymap);
		pNode->AddListItem(Key.key.entity, LT_THMS_CMD, Key.keymap);	
	}  
}
	
BOOL CDB_THMS::Del(T_THMS_K Key, CDB_THIS* pThis, CDB_NODE* pNode)
{
	T_THMS_D Data;
	BOOL bExist=m_thms.Lookup(Key.keymap, Data);
	if(bExist)
	{
		BOOL ret=m_thms.RemoveKey(Key.keymap);
		ASSERT(ret);
		if(ret)
		{
			VERIFY(pThis->DelListItem(Data.ThisKey, LT_THMS_CMD, Key.keymap));
 			VERIFY(pNode->DelListItem(Key.key.entity, LT_THMS_CMD, Key.keymap));
		}     
		return ret;
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_THMS::Get(T_THMS_K Key,T_THMS_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if(!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if(!m_pDoc->m_pStagCtrl->m_thms->Lookup(Key.keymap, uTemp)) return FALSE;
	}
	return m_thms.Lookup(Key.keymap,rData);
}

int CDB_THMS::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if(!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_thms->GetCount();
	}
	return m_thms.GetCount();
}

POSITION CDB_THMS::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if(!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_thms->GetStartPosition();
	}
	return m_thms.GetStartPosition();
}

void CDB_THMS::GetNext(POSITION& rNextPosition,T_THMS_K& rKey,T_THMS_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0) 
	{
		if(!m_pDoc->m_pStagCtrl->IsFinalStag()) {ASSERT(0); return;}
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_thms->GetNextAssoc(rNextPosition, rKey.keymap, uTemp);
		m_thms.Lookup(rKey.keymap, rData);
		return;
	}
	m_thms.GetNextAssoc(rNextPosition, rKey.keymap, rData);
}