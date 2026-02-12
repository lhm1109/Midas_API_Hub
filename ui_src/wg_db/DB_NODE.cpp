#include "stdafx.h"
#include "DB_NODE.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_NODE::CDB_NODE()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nStartNum=1;
	m_nLastNum=0;
	m_node.InitHashTable(HASHSIZENODE);
	m_nodelist.InitHashTable(HASHSIZELISTNODE);
	mListPos.InitHashTable(HASHSIZENODE);
}

CDB_NODE::CDB_NODE(CDB_NODE& src)
{
	*this = src;
}

CDB_NODE& CDB_NODE::operator= (CDB_NODE& src)
{
	POSITION pos;

	m_nStartNum=src.m_nStartNum;
	m_nLastNum=src.m_nLastNum;
	m_pDoc = 0;  // 복사되는 것은 Stage와 무관하므로 NULL로 설정

	T_NODE_K KeyNode;
	T_NODE_D DataNode;
	m_node.RemoveAll();
	pos=src.m_node.GetStartPosition();
	while(pos != NULL)
	{
		src.m_node.GetNextAssoc(pos,KeyNode,DataNode);
		m_node.SetAt(KeyNode, DataNode);
	}

	DelAllList();
	NodeList* pList;
	pos=src.m_nodelist.GetStartPosition();
	while(pos != NULL)
	{
		src.m_nodelist.GetNextAssoc(pos,KeyNode,pList);
		NodeList* pListNew = new NodeList;
		POSITION pos2=pList->GetHeadPosition();
		while(pos2 != NULL)
		{
			T_LIST_DATA ListData=pList->GetNext(pos2);
			pListNew->AddTail(ListData);
		}
		m_nodelist.SetAt(KeyNode, pListNew);
	}

	// 복사 되는 것은 DistSortedList 안 쓴다.
	m_DistSortedList.ResetList();
//  CDistSortedList m_DistSortedList;
	return *this;
}

CDB_NODE::~CDB_NODE()
{
	DelAllList();
}

void CDB_NODE::Add(T_NODE_K Key,T_NODE_D& rData)
{
	T_NODE_D data;
	if(m_node.Lookup(Key, data))DeleteSortedList(Key, sqrt(data.x*data.x+data.y*data.y+data.z*data.z));

	m_node.SetAt(Key,rData);

	InsertSortedList(Key, sqrt(rData.x*rData.x+rData.y*rData.y+rData.z*rData.z));

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

BOOL CDB_NODE::Del(T_NODE_K Key)
{
	T_NODE_D data;
	m_node.Lookup(Key, data);

	BOOL ret=m_node.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		VERIFY(DeleteSortedList(Key, sqrt(data.x*data.x+data.y*data.y+data.z*data.z)) >= 0.);

		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_NODE_K key;
			T_NODE_D data;
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
	}
	return ret;
}

void CDB_NODE::AddList(T_NODE_K Key)
{
	NodeList* pList = new NodeList;
	m_nodelist.SetAt(Key, pList);
}

BOOL CDB_NODE::DelList(T_NODE_K Key)
{
	NodeList* pList;
	BOOL bExist=m_nodelist.Lookup(Key,pList);
	ASSERT(bExist);
	if(!bExist)return FALSE;
	m_nodelist.RemoveKey(Key);

	/*
	T_KEY_KEY_CMD_K IndexK;
	T_LIST_DATA ListData;
	POSITION pos2 = pList->GetHeadPosition();
	while (pos2)
	{
		ListData = pList->GetNext(pos2);
		IndexK.Set(Key, ListData.nKey, ListData.nCmd);
		mListPos.RemoveKey(IndexK);
	}
	*/

	delete pList;
	pList = NULL;

	return TRUE;
}

void CDB_NODE::DelAllList()
{
	T_NODE_K Key;
	NodeList* pList;
	POSITION pos=m_nodelist.GetStartPosition();
	while(pos != NULL)
	{
		m_nodelist.GetNextAssoc(pos,Key,pList);

		/*
		T_KEY_KEY_CMD_K IndexK;
		T_LIST_DATA ListData;
		POSITION pos2 = pList->GetHeadPosition();
		while (pos2)
		{
			ListData = pList->GetNext(pos2);
			IndexK.Set(Key, ListData.nKey, ListData.nCmd);
			mListPos.RemoveKey(IndexK);
		}
		*/

		delete pList;
	}
	m_nodelist.RemoveAll();
}

void CDB_NODE::AddListItem(T_NODE_K Key, int nCmd, int nKey)
{
	NodeList* pList;
	//VERIFY(GetList(Key, pList));
	if (m_nodelist.Lookup(Key, pList) == FALSE) {
		VERIFY(FALSE);
		return;
	}

	T_LIST_DATA ListData;
	ListData.nCmd=nCmd;
	ListData.nKey=nKey;
	POSITION pos = pList->AddTail(ListData);

	//T_KEY_KEY_CMD_K IndexK;
	//IndexK.Set(Key, nKey, nCmd);
	//mListPos.SetAt(IndexK, pos);

	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
		m_pDoc->m_pStagCtrl->AddListItemForNode(Key, nCmd, nKey);
}

BOOL CDB_NODE::DelListItem(T_NODE_K Key, int nCmd, int nKey)
{
	NodeList* pList;
	//VERIFY(GetList(Key, pList));
	if (m_nodelist.Lookup(Key, pList) == FALSE) {
		VERIFY(FALSE);
		return FALSE;
	}

	BOOL bDeleted = FALSE;

	/*
	T_KEY_KEY_CMD_K IndexKey;
	IndexKey.Set(Key, nKey, nCmd);
	POSITION ListPos;
	if (mListPos.Lookup(IndexKey, ListPos))
	{
		POSITION posPrv = ListPos;
		T_LIST_DATA ListData = pList->GetNext(ListPos);
		if(ListData.nCmd == nCmd && ListData.nKey == nKey)
		{
			pList->RemoveAt(posPrv);
			bDeleted = TRUE;
		}
	}
	*/
	T_LIST_DATA ListData;
	//POSITION pos=pList->GetHeadPosition(), posPrv;
	POSITION pos=pList->GetTailPosition(), posPrv;
	while(pos != NULL)
	{
		posPrv=pos;
		//ListData=pList->GetNext(pos);
		ListData=pList->GetPrev(pos);
		if(ListData.nCmd == nCmd && ListData.nKey == nKey)
		{
			pList->RemoveAt(posPrv);
			bDeleted = TRUE;
			break;
		}
	}
	if (!bDeleted) return FALSE;

	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
		return m_pDoc->m_pStagCtrl->DelListItemForNode(Key, nCmd, nKey);
	return TRUE;
}

void CDB_NODE::AddListItem(CArray<T_NODE_K, T_NODE_K>& aNewNodeK, 
													 CArray<T_NODE_K, T_NODE_K>& aOldNodeK,
													 int nCmd, int nKey, BOOL bExist)
{
	CMap<T_NODE_K, T_NODE_K, T_NODE_K, T_NODE_K>mapOld;
	CMap<T_NODE_K, T_NODE_K, T_NODE_K, T_NODE_K>mapNew;
	T_NODE_K KeyNodeTemp;
	if (bExist)
	{
		mapNew.InitHashTable(997);
		mapOld.InitHashTable(997);
		for(int i = 0; i<aOldNodeK.GetSize(); i++)mapOld.SetAt(aOldNodeK[i], aOldNodeK[i]);
		for(int i = 0; i<aNewNodeK.GetSize(); i++)mapNew.SetAt(aNewNodeK[i], aNewNodeK[i]);
		for(int i = 0; i<aOldNodeK.GetSize(); i++)
		{
			if(!mapNew.Lookup(aOldNodeK[i], KeyNodeTemp))
				VERIFY(DelListItem(aOldNodeK[i], nCmd, nKey));
		}
	}
	for(int i=0; i<aNewNodeK.GetSize(); i++)
	{
		if(bExist)
		{
			if(!mapOld.Lookup(aNewNodeK[i], KeyNodeTemp))
				AddListItem(aNewNodeK[i], nCmd, nKey);
		}
		else
			AddListItem(aNewNodeK[i], nCmd, nKey);
	}
}

BOOL CDB_NODE::DelListItem(CArray<T_NODE_K, T_NODE_K>& aOldNodeK, 
													 int nCmd, int nKey)
{
	for(int i=0; i<aOldNodeK.GetSize(); i++)
	{
		if (!DelListItem(aOldNodeK[i], nCmd, nKey)) return FALSE;
	}
	return TRUE;
}

void CDB_NODE::InsertSortedList(T_NODE_K Key, double dblDistance)
{
	// 복사 되는 것은 DistSortedList 안 쓴다.
	if (m_pDoc == 0) return;
	if (m_DistSortedList.GetCount() == 0) return;

	m_DistSortedList.Insert(Key, dblDistance);
}

double CDB_NODE::DeleteSortedList(T_NODE_K Key, double dblDistance)
{
	// 복사 되는 것은 DistSortedList 안 쓴다.
	if (m_pDoc == 0) return 0.0;
	if (m_DistSortedList.GetCount() == 0) return 0.0;
	return m_DistSortedList.Delete(Key, dblDistance);
}

struct NodeDist
{
	T_NODE_K NodeK;
	double   dDist;
	static int Comp(const void* p1, const void* p2)
	{
		NodeDist* v1 = (NodeDist*)p1;
		NodeDist* v2 = (NodeDist*)p2;
		if (v1->dDist > v2->dDist) return 1;
		else if (v1->dDist < v2->dDist) return -1;
		return 0;
	}
};

void CDB_NODE::FindSameDistNodeList(double distance, double tolerance,
									CArray<T_NODE_K, T_NODE_K>& node_list)
{
	// 복사 되는 것은 DistSortedList 안 쓴다.
	if (m_pDoc == 0) return;
	if (m_node.GetCount() != m_DistSortedList.GetCount())
	{
		#if defined(_DEBUG)
		clock_t start = clock();
		#endif

		//최초 접근이므로 만든다.
		CArray<NodeDist, NodeDist&> aNodeDist;
		aNodeDist.SetSize(m_node.GetCount());
		T_NODE_D NodeD;
		int i = 0;
		POSITION pos = m_node.GetStartPosition();
		while (pos)
		{
			m_node.GetNextAssoc(pos, aNodeDist[i].NodeK, NodeD);
			aNodeDist[i].dDist = sqrt(NodeD.x*NodeD.x+NodeD.y*NodeD.y+NodeD.z*NodeD.z);
			i++;
		}
		qsort(aNodeDist.GetData(), aNodeDist.GetSize(), sizeof(NodeDist), NodeDist::Comp);
		CArray<UINT, UINT> aKey;
		CArray<double, double> aDist;
		aKey.SetSize(aNodeDist.GetSize());
		aDist.SetSize(aNodeDist.GetSize());
		for(int i = 0; i < aNodeDist.GetSize(); i++)
		{
			aKey[i] = aNodeDist[i].NodeK;
			aDist[i] = aNodeDist[i].dDist;
		}
		m_DistSortedList.InsertSortedList(aKey, aDist);

		#if defined(_DEBUG)
		clock_t end = clock();
		double dET = (double)(end - start) / CLOCKS_PER_SEC;
		GSaveHistoryFormatNF(_T("Sorted list reconstruction time is %g"), dET);
		#endif
	}

	m_DistSortedList.FindSameDistNodeList(distance, tolerance, node_list, NULL);
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		int nTemp;
		int nSize = node_list.GetSize();
		for (int i = nSize-1; i >= 0; i--)
			if (!m_pDoc->m_pStagCtrl->m_node->Lookup(node_list[i], nTemp))
				node_list.RemoveAt(i);
	}
}

void CDB_NODE::ResetSortedList()
{
	m_DistSortedList.ResetList();
}

//------------------------------------------------------------------------
BOOL CDB_NODE::Get(T_NODE_K Key,T_NODE_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		int nTemp;
		if (!m_pDoc->m_pStagCtrl->m_node->Lookup(Key, nTemp)) return FALSE;
	}
	return m_node.Lookup(Key,rData);
}

int CDB_NODE::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_node->GetCount();
	}
	return m_node.GetCount();
}

POSITION CDB_NODE::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_node->GetStartPosition();
	}
	return m_node.GetStartPosition();
}

void CDB_NODE::GetNext(POSITION& rNextPosition,T_NODE_K& rKey,T_NODE_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		int nTemp;
		m_pDoc->m_pStagCtrl->m_node->GetNextAssoc(rNextPosition, rKey, nTemp);
		m_node.Lookup(rKey, rData);
		return;
	}
	m_node.GetNextAssoc(rNextPosition,rKey,rData);
}

BOOL CDB_NODE::GetList(T_NODE_K Key,NodeList*& rpList)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_nodelist->Lookup(Key,rpList);
	}
	return m_nodelist.Lookup(Key,rpList);
}

int CDB_NODE::GetCountList()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_nodelist->GetCount();
	}
	return m_nodelist.GetCount();
}
