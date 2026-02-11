#include "stdafx.h"
#include "DB_STLD.h"
#include "DB_ELEM.h"
#include "DB_ETMP.h"
#include "DB_LDGR.h"
#include "DB_MATL.h"
#include "DB_FLUE.h"
#include "DB_STTF.h"
#include "DB_FIPA.h"
#include "DB_FSSF.h"
#include "AttrCtrl.h"
#include "DBDoc.h"
#include "StagCtrl.h"

CDB_ETMP::CDB_ETMP()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_etmp.InitHashTable(HASHSIZEETMP);
}

CDB_ETMP::~CDB_ETMP()
{

}

void CDB_ETMP::Add(T_ETMP_K Key,T_ETMP_D& rData,CDB_STLD* pStld,CDB_ELEM* pElem,CDB_LDGR* pLdgr,
				   CDB_FLUE* pFlue /* = NULL */,CDB_STTF* pSttf /* = NULL */,CDB_FIPA* pFipa /* = NULL */,CDB_FSSF* pFssf /* = NULL */,CDB_MATL* pMatl /* = NULL */)
{
	T_ETMP_D Data;
	BOOL bExist=m_etmp.Lookup(Key.keymap, Data);
	if(bExist && Data.LoadCaseKey != rData.LoadCaseKey)
		VERIFY(pStld->DelListItem(Data.LoadCaseKey, LT_ETMP_CMD, Key.keymap));
	if(bExist && Data.GroupKey != rData.GroupKey)
		VERIFY(pLdgr->DelListItem(Data.GroupKey, LT_ETMP_CMD, Key.keymap));
	m_etmp.SetAt(Key.keymap,rData);
	if(bExist && Data.LoadCaseKey != rData.LoadCaseKey)
		pStld->AddListItem(rData.LoadCaseKey, LT_ETMP_CMD, Key.keymap);
	if(bExist && Data.GroupKey != rData.GroupKey)
		pLdgr->AddListItem(rData.GroupKey, LT_ETMP_CMD, Key.keymap);
	T_STLD_K fireK = m_pDoc->m_pAttrCtrl->GetStldKey(FIRE_LOAD_CASE_NAME);
	BOOL bPreFire = bExist && (fireK == Data.LoadCaseKey);
	BOOL bCurFire = fireK == rData.LoadCaseKey;
	if(bPreFire)
	{
		VERIFY(pFipa->DelListItem(Key.key.entity, LT_ETMP_CMD, Key.keymap));
		VERIFY(pFssf->DelListItem(Key.key.entity, LT_ETMP_CMD, Key.keymap));
		T_ELEM_D ElemD;
		m_pDoc->m_pAttrCtrl->GetElem(Key.key.entity,ElemD);
		VERIFY(pMatl->DelListItem(ElemD.elmat,LT_ETMP_CMD,Key.keymap));
	}
	if(bCurFire)
	{
		pFipa->AddListItem(Key.key.entity, LT_ETMP_CMD, Key.keymap);
		pFssf->AddListItem(Key.key.entity, LT_ETMP_CMD, Key.keymap);
		T_ELEM_D ElemD;
		m_pDoc->m_pAttrCtrl->GetElem(Key.key.entity,ElemD);
		pMatl->AddListItem(ElemD.elmat,LT_ETMP_CMD,Key.keymap);
	}
	if(!bExist)
	{
		pStld->AddListItem(rData.LoadCaseKey, LT_ETMP_CMD, Key.keymap);
		pElem->AddListItem(Key.key.entity, LT_ETMP_CMD, Key.keymap);
		pLdgr->AddListItem(rData.GroupKey, LT_ETMP_CMD, Key.keymap);
	}
	// Current Stage가 Base Stage가 아니면 Stage DB를 Update한다.
	// Load는 현재 Active된 그룹에 속하는 것만 Add될 수 있으므로
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveLoad(LT_ETMP_CMD, Key.keymap, rData.GroupKey);
	}
}

BOOL CDB_ETMP::Del(T_ETMP_K Key,CDB_STLD* pStld,CDB_ELEM* pElem,CDB_LDGR* pLdgr,
				   CDB_FLUE* pFlue /* = NULL */,CDB_STTF* pSttf /* = NULL */,CDB_FIPA* pFipa /* = NULL */,CDB_FSSF* pFssf /* = NULL */,CDB_MATL* pMatl /* = NULL */)
{
	T_ETMP_D Data;
	BOOL bExist=m_etmp.Lookup(Key.keymap, Data);
	if(bExist)
	{
		T_STLD_K fireK = m_pDoc->m_pAttrCtrl->GetStldKey(FIRE_LOAD_CASE_NAME);
		BOOL bFire = fireK == Data.LoadCaseKey;
		BOOL ret=m_etmp.RemoveKey(Key.keymap);
		ASSERT(ret);
		if(ret)
		{
			VERIFY(pStld->DelListItem(Data.LoadCaseKey, LT_ETMP_CMD, Key.keymap));
			VERIFY(pElem->DelListItem(Key.key.entity, LT_ETMP_CMD, Key.keymap));
			VERIFY(pLdgr->DelListItem(Data.GroupKey, LT_ETMP_CMD, Key.keymap));
			if(bFire)
			{
				pFipa->DelListItem(Key.key.entity, LT_ETMP_CMD, Key.keymap);
				pFssf->DelListItem(Key.key.entity, LT_ETMP_CMD, Key.keymap);
				T_ELEM_D ElemD;
				m_pDoc->m_pAttrCtrl->GetElem(Key.key.entity,ElemD);
				pMatl->DelListItem(ElemD.elmat,LT_ETMP_CMD,Key.keymap);
			}
			// Current Stage가 Base Stage가 아니면 Stage DB를 Update한다.
			if (m_pDoc->m_pStagCtrl->GetCurStag() != 0)
			{
				m_pDoc->m_pStagCtrl->DeactiveLoad(LT_ETMP_CMD, Key.keymap);
			}
		}
		return ret;
	}
	return bExist;
}

//--------------------------------------------------------------------------
BOOL CDB_ETMP::Get(T_ETMP_K Key,T_ETMP_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		T_KEY2 StagLdgrKey;
		if (!m_pDoc->m_pStagCtrl->m_etmp->Lookup(Key.keymap, StagLdgrKey)) return FALSE;
	}
	return m_etmp.Lookup(Key.keymap,rData);
}

int CDB_ETMP::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_etmp->GetCount();
	}
	return m_etmp.GetCount();
}

POSITION CDB_ETMP::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_etmp->GetStartPosition();
	}
	return m_etmp.GetStartPosition();
}

void CDB_ETMP::GetNext(POSITION& rNextPosition,T_ETMP_K& rKey,T_ETMP_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		T_KEY2 StagLdgrKey;
		m_pDoc->m_pStagCtrl->m_etmp->GetNextAssoc(rNextPosition, rKey.keymap, StagLdgrKey);
		m_etmp.Lookup(rKey.keymap, rData);
		return;
	}
	m_etmp.GetNextAssoc(rNextPosition,rKey.keymap,rData);
}
