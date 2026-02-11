#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_SPAN.h"

#include "DBDoc.h"
#include "StagCtrl.h"
#include "DBLib.h"
#include "AttrCtrl2.h"

#include "..\wg_base\AfxTemplEx.h"

CDB_SPAN::CDB_SPAN()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nStartNum=1;
	m_nLastNum=0;
	m_span.InitHashTable(HASHSIZESPAN);
}

CDB_SPAN::~CDB_SPAN()
{
}

#define M_OP_ADD 1
#define M_OP_MOD 0
#define M_OP_DEL -1
void CDB_SPAN::MakeList(int nOP, const T_SPAN_D* pCurSpanD, const T_SPAN_D* pNewSpanD, 
												CArray<UINT, UINT>* paAddList, CArray<UINT, UINT>* paDelList)
{
	switch(nOP)
	{
	case M_OP_ADD:
		{
			ASSERT(pNewSpanD);
			paAddList->RemoveAll();
			for(int i = 0; i < pNewSpanD->aElemBase.GetSize(); i++)
			{
				paAddList->Add(pNewSpanD->aElemBase[i].ElemK);
			}      
		}
		break;
	case M_OP_MOD:
		// 삭제할 것, 추가할 것 구분
		{
			paAddList->RemoveAll();
			paDelList->RemoveAll();
			
				CMapEx<UINT, UINT, int, int> mCur;
				mCur.InitHashTable(101);
				for(int i = 0; i < pCurSpanD->aElemBase.GetSize(); i++)
					mCur.SetAt(pCurSpanD->aElemBase[i].ElemK, 0);
				for(int i = 0; i < pNewSpanD->aElemBase.GetSize(); i++)
				{
					if (!mCur.Exist(pNewSpanD->aElemBase[i].ElemK)) paAddList->Add(pNewSpanD->aElemBase[i].ElemK);
					else mCur.RemoveKey(pNewSpanD->aElemBase[i].ElemK);  // 교집합.
				}
				mCur.GetKeyList(*paDelList);
		}
		break;
	case M_OP_DEL:
		{
			ASSERT(pCurSpanD);
			paDelList->RemoveAll();
			for(int i = 0; i < pCurSpanD->aElemBase.GetSize(); i++)
				paDelList->Add(pCurSpanD->aElemBase[i].ElemK);
		}
		break;
	}
}

void CDB_SPAN::Add(T_SPAN_K Key,T_SPAN_D& rData,CDB_ELEM* pElem)
{
	T_SPAN_D Data;
	BOOL bExist=m_span.Lookup(Key, Data);
	CArray<UINT, UINT> aAddList, aDelList;
	if (bExist) MakeList(M_OP_MOD, &Data, &rData, &aAddList, &aDelList);
	else        MakeList(M_OP_ADD, 0    , &rData, &aAddList, 0);
	if(bExist)
	{
		int nCount = aDelList.GetSize();
		for(int i=0; i<nCount; i++)
		{
			pElem->DelListItem(aDelList[i], LT_SPAN_CMD, Key);
		}
	}
	m_span.SetAt(Key,rData);

	int nCount = aAddList.GetSize();
	for (int i = 0; i < nCount; i++)
	{
		pElem->AddListItem(aAddList[i], LT_SPAN_CMD, Key);
	}

	if(Key == m_nStartNum)
	{
		T_SPAN_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_SPAN::Del(T_SPAN_K Key,CDB_ELEM* pElem)
{
	T_SPAN_D data;
	if(!Get(Key, data))return FALSE;

	BOOL ret=m_span.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		CArray<UINT, UINT> aDelList;
		MakeList(M_OP_DEL, &data, 0, 0, &aDelList);
		int nCount=aDelList.GetSize();
		for(int i=0; i<nCount; i++)
		{
			pElem->DelListItem(aDelList[i], LT_SPAN_CMD, Key);
		}

		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_SPAN_K key;
			T_SPAN_D data;
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

//-------------------------------------------------------------------------
BOOL CDB_SPAN::Get(T_SPAN_K Key,T_SPAN_D& rData)
{
	T_CPGD_D CpgdD;
	CpgdD.Initialize();
	m_pDoc->m_pAttrCtrl2->GetDgnCpgd(CpgdD);

	if(CDBLib::IsCSGCode(CpgdD.iDgnCode))
	{
		return m_span.Lookup(Key,rData);
	}
	else
	{
		if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
		{
			return FALSE;
		}
		return m_span.Lookup(Key,rData);
	}
}

int CDB_SPAN::GetCount()
{
	T_CPGD_D CpgdD;
	CpgdD.Initialize();
	m_pDoc->m_pAttrCtrl2->GetDgnCpgd(CpgdD);

 if(CDBLib::IsCSGCode(CpgdD.iDgnCode))
	{
		return m_span.GetCount();
	}
	else
	{
		if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
		{
			return 0;
		}
		return m_span.GetCount();
	}
}

POSITION CDB_SPAN::GetStart()
{
	T_CPGD_D CpgdD;
	CpgdD.Initialize();
	m_pDoc->m_pAttrCtrl2->GetDgnCpgd(CpgdD);

	if(CDBLib::IsCSGCode(CpgdD.iDgnCode))
	{
		return m_span.GetStartPosition();
	}
	else
	{
		if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
		{
			return 0;
		}
		return m_span.GetStartPosition();
	}

}

void CDB_SPAN::GetNext(POSITION& rNextPosition,T_SPAN_K& rKey,T_SPAN_D& rData)
{
	T_CPGD_D CpgdD;
	CpgdD.Initialize();
	m_pDoc->m_pAttrCtrl2->GetDgnCpgd(CpgdD);

	if(CDBLib::IsCSGCode(CpgdD.iDgnCode))
	{
		m_span.GetNextAssoc(rNextPosition,rKey,rData);
	}
	else
	{
		if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
		{
			ASSERT(0);
			return;
		}
		m_span.GetNextAssoc(rNextPosition,rKey,rData);
	}
}
