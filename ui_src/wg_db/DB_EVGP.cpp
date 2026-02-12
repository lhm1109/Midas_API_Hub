#include "stdafx.h"
#include "DB_EVGP.h"
#include "DB_ELEM.h"
#include "DB_NODE.h"

#include "DBDoc.h"
#include "StagCtrl.h"


CDB_EVGP::CDB_EVGP()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_evgp.InitHashTable(HASHSIZEEVGP);
	m_nStartNum = 1;
	m_nLastNum  = 0;
}

CDB_EVGP::~CDB_EVGP()
{

}

void CDB_EVGP::Add(T_EVGP_K Key,T_EVGP_D& rData,CDB_NODE* pNode,CDB_ELEM* pElem)
{
	T_EVGP_D Data;
	BOOL bExist=m_evgp.Lookup(Key, Data);

	T_ELEM_D ElemData;
	T_NODE_D NodeData;

	if(bExist)
	{
		for(int i=0; i<Data.aPierCap.GetSize(); i++)
		{
			if (pElem) 
			{
				if(pElem->Get(Data.aPierCap[i], ElemData))	
					pElem->DelListItem(Data.aPierCap[i], LT_EVGP_CMD, Key);
			}
		}

		for(int i=0; i<Data.aEvgpColm.GetSize(); i++)
		{
			for(int j=0; j<Data.aEvgpColm[i].aColumn.GetSize(); j++)
			{
				if (pElem)
				{
					if(pElem->Get(Data.aEvgpColm[i].aColumn[j], ElemData))
						pElem->DelListItem(Data.aEvgpColm[i].aColumn[j], LT_EVGP_CMD, Key);
				}
			}
		}		

		for(int i=0; i<Data.aLink.GetSize(); i++)
		{
			if (pNode) 
			{
				if(pNode->Get(Data.aLink[i], NodeData))
					pNode->DelListItem(Data.aLink[i], LT_EVGP_CMD, Key);
			}
		}

		for (int i = 0; i < Data.aSupport.GetSize(); i++)
		{
			if (pNode)
			{
				if(pNode->Get(Data.aSupport[i], NodeData))
					pNode->DelListItem(Data.aSupport[i], LT_EVGP_CMD, Key);
			}
		}

	}

	m_evgp.SetAt(Key,rData);

	for(int i=0; i<rData.aPierCap.GetSize(); i++)
	{
		if (pElem)
		{
			if (pElem->Get(rData.aPierCap[i], ElemData))
				pElem->AddListItem(rData.aPierCap[i], LT_EVGP_CMD, Key);
		}			
	}

	for(int i=0; i<rData.aEvgpColm.GetSize(); i++)
	{
		for(int j=0; j<rData.aEvgpColm[i].aColumn.GetSize(); j++)
		{
			if (pElem)
			{
				if (pElem->Get(rData.aEvgpColm[i].aColumn[j], ElemData))
					pElem->AddListItem(rData.aEvgpColm[i].aColumn[j], LT_EVGP_CMD, Key);
			}			
		}	
	}

	for(int i=0; i<rData.aLink.GetSize(); i++)
	{
		if (pNode)
		{
			if (pNode->Get(rData.aLink[i], NodeData))
				pNode->AddListItem(rData.aLink[i], LT_EVGP_CMD, Key);
		}
	}

	for (int i = 0; i < rData.aSupport.GetSize(); i++)
	{
		if (pNode)
		{
			if (pNode->Get(rData.aSupport[i], NodeData))
				pNode->AddListItem(rData.aSupport[i], LT_EVGP_CMD, Key);
		}
	}

	if(Key == m_nStartNum)
	{
		T_EVGP_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!m_evgp.Lookup(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_EVGP::Del(T_EVGP_K Key,CDB_NODE* pNode,CDB_ELEM* pElem)
{
	T_EVGP_D Data;
	BOOL bExist=m_evgp.Lookup(Key, Data);
	if(bExist)
	{
		T_ELEM_D ElemData;
		T_NODE_D NodeData;
		BOOL ret=m_evgp.RemoveKey(Key);
		ASSERT(ret);
		if(ret)
		{
			for(int i=0; i<Data.aPierCap.GetSize(); i++)
			{
				if (pElem)
				{
					if(pElem->Get(Data.aPierCap[i], ElemData))
						VERIFY(pElem->DelListItem(Data.aPierCap[i], LT_EVGP_CMD, Key));
				}
			}

			for(int i=0; i<Data.aEvgpColm.GetSize(); i++)
			{
				for(int j=0; j<Data.aEvgpColm[i].aColumn.GetSize(); j++)
				{
					if (pElem)
					{
						if(pElem->Get(Data.aEvgpColm[i].aColumn[j], ElemData))
							VERIFY(pElem->DelListItem(Data.aEvgpColm[i].aColumn[j], LT_EVGP_CMD, Key));
					}
				}
			}		

			for(int i=0; i<Data.aLink.GetSize(); i++)
			{
				if (pNode)
				{
					if(pNode->Get(Data.aLink[i], NodeData))
						VERIFY(pNode->DelListItem(Data.aLink[i], LT_EVGP_CMD, Key));
				}
			}

			for (int i = 0; i < Data.aSupport.GetSize(); i++)
			{
				if (pNode)
				{
					if(pNode->Get(Data.aSupport[i], NodeData))
						VERIFY(pNode->DelListItem(Data.aSupport[i], LT_EVGP_CMD, Key));
				}
			}

		}
		
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_EVGP_K key;
			T_EVGP_D data;
			if(m_evgp.Lookup(m_nLastNum-1,data))
			{
				m_nLastNum--;
			}
			else
			{
				m_nLastNum=0;
				POSITION pos=m_evgp.GetStartPosition();
				while(pos != NULL)
				{
					m_evgp.GetNextAssoc(pos,key,data);
					if(key > m_nLastNum)m_nLastNum=key;
				}
			}
		}
		return ret;
	}
	return bExist;
}

//--------------------------------------------------------------------------
BOOL CDB_EVGP::Get(T_EVGP_K Key,T_EVGP_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_evgp->Lookup(Key, uTemp)) return FALSE;
	}
	return m_evgp.Lookup(Key,rData);
}

int CDB_EVGP::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_evgp->GetCount();
	}
	return m_evgp.GetCount();
}

POSITION CDB_EVGP::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_evgp->GetStartPosition();
	}
	return m_evgp.GetStartPosition();
}

void CDB_EVGP::GetNext(POSITION& rNextPosition,T_EVGP_K& rKey,T_EVGP_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_evgp->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_evgp.Lookup(rKey, rData);
		return;
	}
	m_evgp.GetNextAssoc(rNextPosition,rKey,rData);
}

