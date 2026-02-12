#include "stdafx.h"
#include "DB_LDGR.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_LDGR::CDB_LDGR()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nStartNum=1;
	m_nLastNum=0;
	m_ldgr.InitHashTable(HASHSIZELDGR);
	m_ldgrlist.InitHashTable(HASHSIZELISTLDGR);
	AddList(0); // add default group
}

CDB_LDGR::~CDB_LDGR()
{
	DelAllList();
}

void CDB_LDGR::Add(T_LDGR_K Key,T_LDGR_D& rData)
{
	// 새로 추가된 경우 기존 Group의 GroupId가 추가된 GroupId보다 
	// 크거나 같으면 GroupId를 1씩 증가
	T_LDGR_D data;
	BOOL bExist = m_ldgr.Lookup(Key, data);
	if (!bExist)  
	{
		T_LDGR_K key;
		T_LDGR_D data;
		POSITION pos = GetStart();
		while (pos)
		{
			GetNext(pos, key, data);
			if (data.nGroupId >= rData.nGroupId) 
			{
				data.nGroupId++;
				m_ldgr.SetAt(key, data);
			}
		}
	}
	m_ldgr.SetAt(Key,rData);
	if(Key == m_nStartNum)
	{
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_LDGR::Del(T_LDGR_K Key)
{
	T_LDGR_D Data;
	m_ldgr.Lookup(Key, Data);
	BOOL ret=m_ldgr.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_LDGR_K key;
			T_LDGR_D data;
			if(Get(m_nLastNum-1,data))
			{
				m_nLastNum--;
			}
			else
			{
				m_nLastNum=0;
				POSITION pos=GetStart();
				while(pos != NULL)
				{
					GetNext(pos,key,data);
					if(key > m_nLastNum)m_nLastNum=key;
				}
			}
		}

		T_LDGR_K key;
		T_LDGR_D data;
		POSITION pos = GetStart();
		while (pos)
		{
			GetNext(pos, key, data);
			if (data.nGroupId > Data.nGroupId)
			{
				data.nGroupId--;
				m_ldgr.SetAt(key, data);
			}
		}
	}
	return ret;
}

void CDB_LDGR::AddList(T_LDGR_K Key)
{
	LdgrList* pList = new LdgrList;
	m_ldgrlist.SetAt(Key, pList);
}

BOOL CDB_LDGR::DelList(T_LDGR_K Key)
{
	LdgrList* pList;
	BOOL bExist=m_ldgrlist.Lookup(Key,pList);
	ASSERT(bExist);
	if(!bExist)return FALSE;
	m_ldgrlist.RemoveKey(Key);
	delete pList;
	return TRUE;
}

void CDB_LDGR::DelAllList()
{
	T_LDGR_K Key;
	LdgrList* pList;
	POSITION pos=m_ldgrlist.GetStartPosition();
	while(pos != NULL)
	{
		m_ldgrlist.GetNextAssoc(pos,Key,pList);
		delete pList;
	}
	m_ldgrlist.RemoveAll();
}

void CDB_LDGR::AddListItem(T_LDGR_K Key, int nCmd, int nKey)
{
	LdgrList* pList;
	//if(!GetList(Key, pList))return;
	if (!m_ldgrlist.Lookup(Key, pList)) return;

	T_LIST_DATA ListData;
	ListData.nCmd=nCmd;
	ListData.nKey=nKey;
	POSITION pos = pList->AddTail(ListData);
	// Default Load Group에 대해 10000개 이상의 하중이 있거나 Map을 유지하고 있으면
	if (Key == 0 && (pList->GetCount() > 10000 || m_CmdKey2Pos.GetCount() > 0))  
	{
		T_CMDKEY_K CmdKey;
		if (m_CmdKey2Pos.GetCount() == 0) // 등록된게 없으면 초기화 하고 새로 등록
		{
			m_CmdKey2Pos.InitHashTable(HASHSIZEELEM);
			T_LIST_DATA ListData;
			POSITION pos2 = pList->GetHeadPosition();
			POSITION pos3;
			while (pos2)
			{
				// GetNext하면 pos2는 리턴된 데이터의 다음 위치가 되므로 여기서 미리 저장
				pos3 = pos2;  
				ListData = pList->GetNext(pos2);
				CmdKey.nCmd = ListData.nCmd;
				CmdKey.nKey = ListData.nKey;
				m_CmdKey2Pos.SetAt(CmdKey, pos3);
			}
		}
		else
		{
			// 새로 추가된 거 등록
			CmdKey.nCmd = nCmd;
			CmdKey.nKey = nKey;
			m_CmdKey2Pos.SetAt(CmdKey, pos);
		}
	}
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && Key != 0) // Default Key는 제외
		m_pDoc->m_pStagCtrl->AddListItemForLdgr(Key, nCmd, nKey);
}

BOOL CDB_LDGR::DelListItem(T_LDGR_K Key, int nCmd, int nKey)
{
	LdgrList* pList;
	//if(!GetList(Key, pList))return TRUE;
	if (!m_ldgrlist.Lookup(Key, pList)) return TRUE;

	// Default Load Group에 대해 Map을 유지하고 있으면 삭제를 빨리 하기 위해
	// Map을 이용한다.
	if (Key == 0 && m_CmdKey2Pos.GetCount() > 0)
	{
		T_CMDKEY_K CmdKey;
		CmdKey.nCmd = nCmd;
		CmdKey.nKey = nKey;
		POSITION pos2;
		if (m_CmdKey2Pos.Lookup(CmdKey, pos2))  // Map에 있는 경우
		{
			// 중복된 Key가 등록될 수 도 있으므로 확인 작업 필요
			T_LIST_DATA ListData2 = pList->GetAt(pos2);
			if (ListData2.nCmd == nCmd && ListData2.nKey == nKey) // 데이터가 바른지 확인
			{
				pList->RemoveAt(pos2);
				m_CmdKey2Pos.RemoveKey(CmdKey);
				return TRUE;
			}
		}
	}

	BOOL bDeleted = FALSE;
	T_LIST_DATA ListData;
	POSITION pos=pList->GetHeadPosition(), posPrv;
	while(pos != NULL)
	{
		posPrv=pos;
		ListData=pList->GetNext(pos);
		if(ListData.nCmd == nCmd && ListData.nKey == nKey)
		{
			pList->RemoveAt(posPrv);
			bDeleted = TRUE;
			break;
		}
	}
	if (!bDeleted) return FALSE;

	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && Key != 0) // default key는 제외
		return m_pDoc->m_pStagCtrl->DelListItemForLdgr(Key, nCmd, nKey);
	return TRUE;
}

BOOL CDB_LDGR::GetList(T_LDGR_K Key,LdgrList*& rpList)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_ldgrlist->Lookup(Key,rpList);
	}
	return m_ldgrlist.Lookup(Key,rpList);
}
