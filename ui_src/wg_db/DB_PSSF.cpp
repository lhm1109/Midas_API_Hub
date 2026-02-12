#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_PSSF.h"
#include "DB_BNGR.h"

#include "DBDoc.h"
#include "StagCtrl.h"

// 코멘트 1 조건에 따라 시공단계 입력
// - CS analysis control > Include Nonlinear Analysis 옵션 선택 > Accumulative => PSSF 고려안함 
// - CS analysis control > Include Time Dependent Effect 옵션선택 => PSSF 고려안함 
// - CS analysis control > Include Nonlinear Analysis 옵션 선택, Include Time Dependent Effect 옵션선택 안함> Independent=> PSSF 고려 
// - CS analysis control > Include Nonlinear Analysis와 Include Time Dependent Effect 옵션 모두 선택안함 => PSSF 고려 

CDB_PSSF::CDB_PSSF()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_pssf.InitHashTable(HASHSIZEPSSF);
}

CDB_PSSF::~CDB_PSSF()
{

}

void CDB_PSSF::Add(T_PSSF_K Key,T_PSSF_D& rData,CDB_ELEM* pElem,CDB_BNGR* pBngr)
{
	T_PSSF_D Data;
	BOOL bExist=m_pssf.Lookup(Key.keymap, Data);
	if(bExist && Data.GroupKey != rData.GroupKey && pBngr)
		pBngr->DelListItem(Data.GroupKey, LT_PSSF_CMD, Key.keymap);
	m_pssf.SetAt(Key.keymap,rData);
	if(bExist && Data.GroupKey != rData.GroupKey && pBngr)
		pBngr->AddListItem(rData.GroupKey, LT_PSSF_CMD, Key.keymap);
	if(!bExist)
	{
		if(pElem) pElem->AddListItem(Key.key.entity, LT_PSSF_CMD, Key.keymap);
		if(pBngr) pBngr->AddListItem(rData.GroupKey, LT_PSSF_CMD, Key.keymap);
	}

	// Current Stage가 Base Stage가 아니면 Stage DB를 Update한다.
	// Boundary는 현재 Active된 그룹에 속하는 것만 Add될 수 있으므로
	// 이미 있는 것이 아니면 새로 Active시킨다.
	/* 코멘트1 참조
	if(m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if(!bExist) m_pDoc->m_pStagCtrl->ActiveBndr(LT_PSSF_CMD, Key.keymap, rData.GroupKey);
	}
	*/
}

BOOL CDB_PSSF::Del(T_PSSF_K Key,CDB_ELEM* pElem,CDB_BNGR* pBngr)
{
	T_PSSF_D Data;
	m_pssf.Lookup(Key.keymap, Data);
	BOOL ret=m_pssf.RemoveKey(Key.keymap);
	ASSERT(ret);
	if(ret)
	{
		if (pElem) VERIFY(pElem->DelListItem(Key.key.entity, LT_PSSF_CMD, Key.keymap));
		if (pBngr) VERIFY(pBngr->DelListItem(Data.GroupKey, LT_PSSF_CMD, Key.keymap));

		// Current Stage가 Base Stage가 아니면 Stage DB를 Update한다.
		/* 코멘트 1 참조
		if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
		{
			m_pDoc->m_pStagCtrl->DeactiveBndr(LT_PSSF_CMD, Key.keymap);
		}
		*/
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_PSSF::Get(T_PSSF_K Key,T_PSSF_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		T_KEY2 StagBngrKey;
		if (!m_pDoc->m_pStagCtrl->m_pssf->Lookup(Key.keymap, StagBngrKey)) return FALSE;
	}
	return m_pssf.Lookup(Key.keymap,rData);
}

int CDB_PSSF::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_pssf->GetCount();
	}
	return m_pssf.GetCount();
}

POSITION CDB_PSSF::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_pssf->GetStartPosition();
	}
	return m_pssf.GetStartPosition();
}

void CDB_PSSF::GetNext(POSITION& rNextPosition,T_PSSF_K& rKey,T_PSSF_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		T_KEY2 StagBngrKey;
		m_pDoc->m_pStagCtrl->m_pssf->GetNextAssoc(rNextPosition,rKey.keymap,StagBngrKey);
		m_pssf.Lookup(rKey.keymap, rData);
		return;
	}
	m_pssf.GetNextAssoc(rNextPosition,rKey.keymap,rData);
}
