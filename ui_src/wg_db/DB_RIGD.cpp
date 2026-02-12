#include "stdafx.h"
#include "DB_NODE.h"
#include "DB_RIGD.h"
#include "DB_BNGR.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_RIGD::CDB_RIGD()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_rigd.InitHashTable(HASHSIZERIGD);
}

CDB_RIGD::CDB_RIGD(CDB_RIGD& src)
{
	*this = src;
}

CDB_RIGD& CDB_RIGD::operator= (CDB_RIGD& src)
{
	POSITION pos;

	m_pDoc = 0;  // 복사되는 것은 Stage와 무관하므로 NULL로 설정

	m_rigd.RemoveAll();
	T_RIGD_KEY KeyRigd;
	T_RIGD_D DataRigd;
	pos=src.m_rigd.GetStartPosition();
	while(pos != NULL)
	{
		src.m_rigd.GetNextAssoc(pos,KeyRigd,DataRigd);
		m_rigd.SetAt(KeyRigd, DataRigd);
	}

	return *this;
}

CDB_RIGD::~CDB_RIGD()
{
}

void CDB_RIGD::Add(T_RIGD_K Key,T_RIGD_D& rData,CDB_NODE* pNode,CDB_BNGR* pBngr)
{
	T_RIGD_D Data;
	BOOL bExist=m_rigd.Lookup(Key.keymap, Data);
	CMap<T_NODE_K, T_NODE_K, T_NODE_K, T_NODE_K>mapRigdOld;
	CMap<T_NODE_K, T_NODE_K, T_NODE_K, T_NODE_K>mapRigdNew;
	T_NODE_K KeyNodeTemp;
	if(bExist)
	{
		mapRigdNew.InitHashTable(HASHSIZENODE);
		mapRigdOld.InitHashTable(HASHSIZENODE);
		for(int i = 0; i<Data.arSlave.GetSize(); i++)mapRigdOld.SetAt(Data.arSlave[i], Data.arSlave[i]);
		for(int i = 0; i<rData.arSlave.GetSize(); i++)mapRigdNew.SetAt(rData.arSlave[i], rData.arSlave[i]);

		ASSERT(rData.nSlaveType == Data.nSlaveType);
		if (pNode)
		{
			VERIFY(pNode->DelListItem(Key.key.entity, LT_RIGD_CMD, Key.keymap));
			int nSlave=Data.arSlave.GetSize();
			for(int i = 0; i<nSlave; i++)
			{
				if(!mapRigdNew.Lookup(Data.arSlave[i], KeyNodeTemp))
					VERIFY(pNode->DelListItem(Data.arSlave[i], LT_RIGD_CMD, Key.keymap));
			}
		}

		// Group Key 처리
		if (Data.GroupKey != rData.GroupKey && pBngr)
			pBngr->DelListItem(Data.GroupKey, LT_RIGD_CMD, Key.keymap);
	}

	m_rigd.SetAt(Key.keymap,rData);
	if (bExist && Data.GroupKey != rData.GroupKey && pBngr)
		pBngr->AddListItem(rData.GroupKey, LT_RIGD_CMD, Key.keymap);
	if (!bExist && pBngr)
		pBngr->AddListItem(rData.GroupKey, LT_RIGD_CMD, Key.keymap);

	if (pNode)
	{
		pNode->AddListItem(Key.key.entity, LT_RIGD_CMD, Key.keymap);
		int nSlave=rData.arSlave.GetSize();
		for(int i=0; i<nSlave; i++)
		{
			if(bExist)
			{
				if(!mapRigdOld.Lookup(rData.arSlave[i], KeyNodeTemp))
					pNode->AddListItem(rData.arSlave[i], LT_RIGD_CMD, Key.keymap);
			}
			else
				pNode->AddListItem(rData.arSlave[i], LT_RIGD_CMD, Key.keymap);
		}
	}

	// Current Stage가 Base Stage가 아니면 Stage DB를 Update한다.
	// Boundary는 현재 Active된 그룹에 속하는 것만 Add될 수 있으므로
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveBndr(LT_RIGD_CMD, Key.keymap, rData.GroupKey);
	}
}

BOOL CDB_RIGD::Del(T_RIGD_K Key,CDB_NODE* pNode,CDB_BNGR* pBngr)
{
	T_RIGD_D Data;
	BOOL bExist=m_rigd.Lookup(Key.keymap, Data);
	if(bExist)
	{
		if (pBngr) pBngr->DelListItem(Data.GroupKey, LT_RIGD_CMD, Key.keymap);
		BOOL ret=m_rigd.RemoveKey(Key.keymap);
		ASSERT(ret);
		if(ret)
		{
			if (pNode) 
			{
				VERIFY(pNode->DelListItem(Key.key.entity, LT_RIGD_CMD, Key.keymap));
				int nSlave=Data.arSlave.GetSize();
				for(int i=0; i<nSlave; i++)
					VERIFY(pNode->DelListItem(Data.arSlave[i], LT_RIGD_CMD, Key.keymap));
			}

			// Current Stage가 Base Stage가 아니면 Stage DB를 Update한다.
			if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
			{
				m_pDoc->m_pStagCtrl->DeactiveBndr(LT_RIGD_CMD, Key.keymap);
			}
		}
		return ret;
	}
	return bExist;
}

void CDB_RIGD::AddItem(T_RIGD_K Key,T_NODE_K KeySlave,CDB_NODE* pNode)
{
	T_RIGD_D Data;
	if(!m_rigd.Lookup(Key.keymap, Data))
	{
		ASSERT(FALSE);
		return;
	}

// 기존 데이터에 없는 Slave Key만 들어와야 됨
//  int nSlave=Data.arSlave.GetSize();
//  for(int i=0; i<nSlave; i++)
//  {
//    if(Data.arSlave[i] == KeySlave)return;
//  }
	Data.arSlave.Add(KeySlave);
	m_rigd.SetAt(Key.keymap,Data);
	if (pNode) pNode->AddListItem(KeySlave, LT_RIGD_CMD, Key.keymap);
}

BOOL CDB_RIGD::DelItem(T_RIGD_K Key,T_NODE_K KeySlave,CDB_NODE* pNode)
{
	T_RIGD_D Data;
	if(!m_rigd.Lookup(Key.keymap, Data))
	{
		ASSERT(FALSE);
		return FALSE;
	}

	int nSlave=Data.arSlave.GetSize();
	for(int i=0; i<nSlave; i++)
	{
		if(Data.arSlave[i] == KeySlave)
		{
			Data.arSlave.RemoveAt(i);
			m_rigd.SetAt(Key.keymap,Data);
			if (pNode) VERIFY(pNode->DelListItem(KeySlave, LT_RIGD_CMD, Key.keymap));
			return TRUE;
		}
	}
	ASSERT(FALSE);
	return FALSE;
}

//--------------------------------------------------------------------------
BOOL CDB_RIGD::Get(T_RIGD_K Key,T_RIGD_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		T_KEY2 StagBngrKey;
		if (!m_pDoc->m_pStagCtrl->m_rigd->Lookup(Key.keymap, StagBngrKey)) return FALSE;
	}
	return m_rigd.Lookup(Key.keymap,rData);
}

int CDB_RIGD::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_rigd->GetCount();
	}
	return m_rigd.GetCount();
}

POSITION CDB_RIGD::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_rigd->GetStartPosition();
	}
	return m_rigd.GetStartPosition();
}

void CDB_RIGD::GetNext(POSITION& rNextPosition,T_RIGD_K& rKey,T_RIGD_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		T_KEY2 StagBngrKey;
		m_pDoc->m_pStagCtrl->m_rigd->GetNextAssoc(rNextPosition, rKey.keymap, StagBngrKey);
		m_rigd.Lookup(rKey.keymap, rData);
		return;
	}
	m_rigd.GetNextAssoc(rNextPosition,rKey.keymap,rData);
}
