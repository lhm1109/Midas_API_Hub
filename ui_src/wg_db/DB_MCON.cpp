#include "stdafx.h"
#include "DB_NODE.h"
#include "DB_MCON.h"
#include "DB_BNGR.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_MCON::CDB_MCON()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_mcon.InitHashTable(HASHSIZEMCON);
}

CDB_MCON::CDB_MCON(CDB_MCON& src)
{
	*this = src;
}

CDB_MCON& CDB_MCON::operator= (CDB_MCON& src)
{
	POSITION pos;

	m_pDoc = 0;  // 복사되는 것은 Stage와 무관하므로 NULL로 설정

	m_mcon.RemoveAll();
	T_MCON_KEY KeyMcon;
	T_MCON_D DataMcon;
	pos=src.m_mcon.GetStartPosition();
	while(pos != NULL)
	{
		src.m_mcon.GetNextAssoc(pos,KeyMcon,DataMcon);
		m_mcon.SetAt(KeyMcon, DataMcon);
	}

	return *this;
}

CDB_MCON::~CDB_MCON()
{
}

void CDB_MCON::Add(T_MCON_K Key,T_MCON_D& rData,CDB_NODE* pNode,CDB_BNGR* pBngr)
{
	T_MCON_D Data;
	BOOL bExist=m_mcon.Lookup(Key.keymap, Data);
	CMap<T_NODE_K, T_NODE_K, T_NODE_K, T_NODE_K>mapMconOld;
	CMap<T_NODE_K, T_NODE_K, T_NODE_K, T_NODE_K>mapMconNew;
	T_NODE_K KeyNodeTemp;
	if(bExist)
	{
		mapMconNew.InitHashTable(101);
		mapMconOld.InitHashTable(101);
		for(int i = 0; i<Data.arSlave.GetSize(); i++)mapMconOld.SetAt(Data.arSlave[i].NodeK, Data.arSlave[i].NodeK);
		for(int i = 0; i<rData.arSlave.GetSize(); i++)mapMconNew.SetAt(rData.arSlave[i].NodeK, rData.arSlave[i].NodeK);

		//ASSERT(rData.nSlaveType == Data.nSlaveType);
		if (pNode)
		{
			VERIFY(pNode->DelListItem(Key.key.entity, LT_MCON_CMD, Key.keymap));
			int nSlave=Data.arSlave.GetSize();
			for(int i = 0; i<nSlave; i++)
			{
				if(!mapMconNew.Lookup(Data.arSlave[i].NodeK, KeyNodeTemp))
					VERIFY(pNode->DelListItem(Data.arSlave[i].NodeK, LT_MCON_CMD, Key.keymap));
			}
		}

		// Group Key 처리
		if (Data.GroupKey != rData.GroupKey && pBngr)
			pBngr->DelListItem(Data.GroupKey, LT_MCON_CMD, Key.keymap);
	}

	m_mcon.SetAt(Key.keymap,rData);
	if (bExist && Data.GroupKey != rData.GroupKey && pBngr)
		pBngr->AddListItem(rData.GroupKey, LT_MCON_CMD, Key.keymap);
	if (!bExist && pBngr)
		pBngr->AddListItem(rData.GroupKey, LT_MCON_CMD, Key.keymap);

	if (pNode)
	{
		pNode->AddListItem(Key.key.entity, LT_MCON_CMD, Key.keymap);
		int nSlave=rData.arSlave.GetSize();
		for(int i=0; i<nSlave; i++)
		{
			if(bExist)
			{
				if(!mapMconOld.Lookup(rData.arSlave[i].NodeK, KeyNodeTemp))
					pNode->AddListItem(rData.arSlave[i].NodeK, LT_MCON_CMD, Key.keymap);
			}
			else
				pNode->AddListItem(rData.arSlave[i].NodeK, LT_MCON_CMD, Key.keymap);
		}
	}

	// Current Stage가 Base Stage가 아니면 Stage DB를 Update한다.
	// Boundary는 현재 Active된 그룹에 속하는 것만 Add될 수 있으므로
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveBndr(LT_MCON_CMD, Key.keymap, rData.GroupKey);
	}
}

BOOL CDB_MCON::Del(T_MCON_K Key,CDB_NODE* pNode,CDB_BNGR* pBngr)
{
	T_MCON_D Data;
	BOOL bExist=m_mcon.Lookup(Key.keymap, Data);
	if(bExist)
	{
		if (pBngr) pBngr->DelListItem(Data.GroupKey, LT_MCON_CMD, Key.keymap);
		BOOL ret=m_mcon.RemoveKey(Key.keymap);
		ASSERT(ret);
		if(ret)
		{
			if (pNode) 
			{
				VERIFY(pNode->DelListItem(Key.key.entity, LT_MCON_CMD, Key.keymap));
				int nSlave=Data.arSlave.GetSize();
				for(int i=0; i<nSlave; i++)
					VERIFY(pNode->DelListItem(Data.arSlave[i].NodeK, LT_MCON_CMD, Key.keymap));
			}

			// Current Stage가 Base Stage가 아니면 Stage DB를 Update한다.
			if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
			{
				m_pDoc->m_pStagCtrl->DeactiveBndr(LT_MCON_CMD, Key.keymap);
			}
		}
		return ret;
	}
	return bExist;
}

void CDB_MCON::AddItem(T_MCON_K Key,T_MCON_BASE& KeySlave,CDB_NODE* pNode)
{
	T_MCON_D Data;
	if(!m_mcon.Lookup(Key.keymap, Data))
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
	m_mcon.SetAt(Key.keymap,Data);
	if (pNode) pNode->AddListItem(KeySlave.NodeK, LT_MCON_CMD, Key.keymap);
}

BOOL CDB_MCON::DelItem(T_MCON_K Key,T_MCON_BASE& KeySlave,CDB_NODE* pNode)
{
	T_MCON_D Data;
	if(!m_mcon.Lookup(Key.keymap, Data))
	{
		ASSERT(FALSE);
		return FALSE;
	}

	int nSlave=Data.arSlave.GetSize();
	for(int i=0; i<nSlave; i++)
	{
		if(Data.arSlave[i].NodeK == KeySlave.NodeK)
		{
			Data.arSlave.RemoveAt(i);
			m_mcon.SetAt(Key.keymap,Data);
			if (pNode) VERIFY(pNode->DelListItem(KeySlave.NodeK, LT_MCON_CMD, Key.keymap));
			return TRUE;
		}
	}
	ASSERT(FALSE);
	return FALSE;
}

//--------------------------------------------------------------------------
BOOL CDB_MCON::Get(T_MCON_K Key,T_MCON_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		T_KEY2 StagBngrKey;
		if (!m_pDoc->m_pStagCtrl->m_mcon->Lookup(Key.keymap, StagBngrKey)) return FALSE;
	}
	return m_mcon.Lookup(Key.keymap,rData);
}

int CDB_MCON::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_mcon->GetCount();
	}
	return m_mcon.GetCount();
}

POSITION CDB_MCON::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_mcon->GetStartPosition();
	}
	return m_mcon.GetStartPosition();
}

void CDB_MCON::GetNext(POSITION& rNextPosition,T_MCON_K& rKey,T_MCON_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		T_KEY2 StagBngrKey;
		m_pDoc->m_pStagCtrl->m_mcon->GetNextAssoc(rNextPosition, rKey.keymap, StagBngrKey);
		m_mcon.Lookup(rKey.keymap, rData);
		return;
	}
	m_mcon.GetNextAssoc(rNextPosition,rKey.keymap,rData);
}
