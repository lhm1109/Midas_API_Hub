#include "stdafx.h"
#include "DB_VBEM.h"
#include "DB_SPAV.h"

#include "DBDoc.h"
#include "StagCtrl.h"
#include "DBLib.h"
#include "AttrCtrl2.h"

#include "..\wg_base\AfxTemplEx.h"

CDB_SPAV::CDB_SPAV()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nStartNum=1;
	m_nLastNum=0;
	m_spav.InitHashTable(HASHSIZESPAV);
}

CDB_SPAV::~CDB_SPAV()
{
}

#define M_OP_ADD 1
#define M_OP_MOD 0
#define M_OP_DEL -1
void CDB_SPAV::MakeList(int nOP, const T_SPAV_D* pCurSpanD, const T_SPAV_D* pNewSpanD, 
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

void CDB_SPAV::Add(T_SPAV_K Key,T_SPAV_D& rData,CDB_VBEM* pElem)
{
	T_SPAV_D Data;
	BOOL bExist=m_spav.Lookup(Key, Data);
	CArray<UINT, UINT> aAddList, aDelList;
	if (bExist) MakeList(M_OP_MOD, &Data, &rData, &aAddList, &aDelList);
	else        MakeList(M_OP_ADD, 0    , &rData, &aAddList, 0);
	if(bExist)
	{
		int nCount = aDelList.GetSize();
		for(int i=0; i<nCount; i++)
		{
			pElem->DelListItem(aDelList[i], LT_SPAV_CMD, Key);
		}
	}
	m_spav.SetAt(Key,rData);

	int nCount = aAddList.GetSize();
	for (int i = 0; i < nCount; i++)
	{
		pElem->AddListItem(aAddList[i], LT_SPAV_CMD, Key);
	}

	if(Key == m_nStartNum)
	{
		T_SPAV_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_SPAV::Del(T_SPAV_K Key, CDB_VBEM* pElem)
{
	T_SPAV_D data;
	if(!Get(Key, data))return FALSE;

	BOOL ret=m_spav.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		CArray<UINT, UINT> aDelList;
		MakeList(M_OP_DEL, &data, 0, 0, &aDelList);
		int nCount=aDelList.GetSize();
		for(int i=0; i<nCount; i++)
		{
			pElem->DelListItem(aDelList[i], LT_SPAV_CMD, Key);
		}

		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_SPAV_K key;
			T_SPAV_D data;
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
BOOL CDB_SPAV::Get(T_SPAV_K Key,T_SPAV_D& rData)
{
	T_CPGD_D CpgdD;
	CpgdD.Initialize();
	m_pDoc->m_pAttrCtrl2->GetDgnCpgd(CpgdD);

	if(CDBLib::IsCSGCode(CpgdD.iDgnCode))
	{
		return m_spav.Lookup(Key,rData);
	}
	else
	{
		if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
		{
			return FALSE;
		}
		return m_spav.Lookup(Key,rData);
	}
}

int CDB_SPAV::GetCount()
{
	T_CPGD_D CpgdD;
	CpgdD.Initialize();
	m_pDoc->m_pAttrCtrl2->GetDgnCpgd(CpgdD);

 if(CDBLib::IsCSGCode(CpgdD.iDgnCode))
	{
		return m_spav.GetCount();
	}
	else
	{
		if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
		{
			return 0;
		}
		return m_spav.GetCount();
	}
}

POSITION CDB_SPAV::GetStart()
{
	T_CPGD_D CpgdD;
	CpgdD.Initialize();
	m_pDoc->m_pAttrCtrl2->GetDgnCpgd(CpgdD);

	if(CDBLib::IsCSGCode(CpgdD.iDgnCode))
	{
		return m_spav.GetStartPosition();
	}
	else
	{
		if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
		{
			return 0;
		}
		return m_spav.GetStartPosition();
	}

}

void CDB_SPAV::GetNext(POSITION& rNextPosition,T_SPAV_K& rKey,T_SPAV_D& rData)
{
	T_CPGD_D CpgdD;
	CpgdD.Initialize();
	m_pDoc->m_pAttrCtrl2->GetDgnCpgd(CpgdD);

	if(CDBLib::IsCSGCode(CpgdD.iDgnCode))
	{
		m_spav.GetNextAssoc(rNextPosition,rKey,rData);
	}
	else
	{
		if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
		{
			ASSERT(0);
			return;
		}
		m_spav.GetNextAssoc(rNextPosition,rKey,rData);
	}
}
