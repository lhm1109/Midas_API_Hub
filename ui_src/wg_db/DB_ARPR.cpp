#include "stdafx.h"
#include "DB_ARPR.h"

#include "DBDoc.h"
#include "StagCtrl.h"
#include "DB_STLD.h"
#include "DB_LDGR.h"
#include "DB_LAPL.h"
#include "DB_ELEM.h"

CDB_ARPR::CDB_ARPR()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nStartNum=1;
	m_nLastNum=0;
	m_arpr.InitHashTable(HASHSIZEARPR);
}

CDB_ARPR::~CDB_ARPR()
{
}

void CDB_ARPR::Add(T_ARPR_K Key,T_ARPR_D& rData, CDB_STLD* pStld, CDB_LDGR* pLdgr, CDB_LAPL* pLapl, CDB_ELEM* pElem)
{
	T_ARPR_D data;
	BOOL bExist=m_arpr.Lookup(Key, data);

	CMap<T_ELEM_K, T_ELEM_K, T_ELEM_K, T_ELEM_K> mapElemNew;
	CMap<T_ELEM_K, T_ELEM_K, T_ELEM_K, T_ELEM_K> mapElemOld;

	int i;
	int nSize;
	T_ELEM_K KeyElemTemp;

	if(bExist)
	{
		//다르면 제거한다.
		if(data.StldK != rData.StldK)
			VERIFY(pStld->DelListItem(data.StldK, LT_ARPR_CMD, Key));
		if (data.LdgrK != rData.LdgrK)
			VERIFY(pLdgr->DelListItem(data.LdgrK, LT_ARPR_CMD, Key));
		if(data.LaplK != rData.LaplK)
			VERIFY(pLapl->DelListItem(data.LaplK, LT_ARPR_CMD, Key));

		for(i=0 ; i<rData.aElemK.GetSize(); i++) mapElemNew.SetAt(rData.aElemK[i], rData.aElemK[i]);
		for(i=0 ; i<data.aElemK.GetSize() ; i++) mapElemOld.SetAt(data.aElemK[i], data.aElemK[i]);

		nSize = data.aElemK.GetSize();
		for(int i = 0; i<nSize; i++)
		{
			if(!mapElemNew.Lookup(data.aElemK[i], KeyElemTemp))
				VERIFY(pElem->DelListItem(data.aElemK[i], LT_ARPR_CMD, Key));
		}
	}

	m_arpr.SetAt(Key,rData);
	if(Key == m_nStartNum)
	{
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))
				break;
		}
	}
	if(Key > m_nLastNum)
		m_nLastNum=Key;

	if(bExist)
	{
		if(data.StldK != rData.StldK)
			pStld->AddListItem(rData.StldK, LT_ARPR_CMD, Key);
		if (data.LdgrK != rData.LdgrK)
			pLdgr->AddListItem(rData.LdgrK, LT_ARPR_CMD, Key);
		if(data.LaplK != rData.LaplK)
			pLapl->AddListItem(rData.LaplK, LT_ARPR_CMD, Key);

		nSize = rData.aElemK.GetSize();
		for(int i = 0; i<nSize; i++)
		{
			if(!mapElemOld.Lookup(rData.aElemK[i], KeyElemTemp))
				pElem->AddListItem(rData.aElemK[i], LT_ARPR_CMD, Key);
		}
	}
	else
	{
		pStld->AddListItem(rData.StldK, LT_ARPR_CMD, Key);
		pLdgr->AddListItem(rData.LdgrK, LT_ARPR_CMD, Key);
		pLapl->AddListItem(rData.LaplK, LT_ARPR_CMD, Key);

		nSize = rData.aElemK.GetSize();
		for(int i = 0; i<nSize; i++)
		{
			pElem->AddListItem(rData.aElemK[i], LT_ARPR_CMD, Key);
		}
	}
	//위부분은 Relation 처리를 위한 부분.

	// Current Stage가 Base Stage가 아니면 Stage DB를 Update한다.
	// Boundary는 현재 Active된 그룹에 속하는 것만 Add될 수 있으므로
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveLoad(LT_ARPR_CMD, Key, rData.LdgrK);
	}

// 	//아래에는 중국코드 2012일때 load case를 추가함.
// 	//////////////////////////////////////////////////////////////////////////
// 	if(CDBDoc::IsEnableWindPressure())
// 	{
// 		T_STLD_D StldD;
// 		CString swd;
// 		CString swL;
// 		CString swT;
// 		T_STLD_K StldK;
// 		T_STLD_K StldK_Lk = 0;
// 		T_STLD_K StldK_Tk = 0;
// 		StldList* pList;
// 		POSITION pos;
// 		if(bExist)
// 		{
// 			if((data.nCodeType == 19 && data.CH2012.bCrossVibration) && 
// 				(data.CH2012.bCross || data.CH2012.bTorsion)) //Consider Cross or torsion 일때만 존재할 것이기때문에.
// 			{
// 				if(!pStld->Get(data.StldK, StldD)) ASSERT(0);
// 
// 				swd = StldD.LoadCaseName;
// 				swL = swd+"_Crs";
// 				swT = swd+"_Tor";
// 
// 				pos=pStld->GetStart();
// 				while(pos != NULL)
// 				{
// 					pStld->GetNext(pos, StldK, StldD);
// 					if(_stricmp(swL, StldD.LoadCaseName) == 0) StldK_Lk = StldK;
// 					if(_stricmp(swT, StldD.LoadCaseName) == 0) StldK_Tk = StldK;
// 				}
// 
// 				if(StldK_Lk > 0)
// 				{
// 					if(!pStld->DelListItem(StldK_Lk, LT_ARPR_CMD, Key)) ASSERT(0);
// 					pStld->GetList(StldK_Lk, pList);
// 					if(pList->GetCount() == 0) pStld->Del(StldK_Lk);
// 				}
// 				if(StldK_Tk > 0)
// 				{
// 					if(!pStld->DelListItem(StldK_Tk, LT_ARPR_CMD, Key)) ASSERT(0);
// 					pStld->GetList(StldK_Tk, pList);
// 					if(pList->GetCount() == 0) pStld->Del(StldK_Tk);
// 				}
// 			}
// 		}
// 
// 		if( (rData.nCodeType == 19 && rData.CH2012.bCrossVibration) && 
// 			(rData.CH2012.bCross || rData.CH2012.bTorsion) ) //Consider Cross or torsion 일때만 존재할 것이기때문에.
// 		{
// 			if(!pStld->Get(rData.StldK, StldD)) ASSERT(0);
// 
// 			StldK_Lk = 0;
// 			StldK_Tk = 0;
// 			swd = StldD.LoadCaseName;
// 			swL = swd+"_Crs";
// 			swT = swd+"_Tor";
// 
// 			pos=pStld->GetStart();
// 			while(pos != NULL)
// 			{
// 				pStld->GetNext(pos, StldK, StldD);
// 				if(_stricmp(swL, StldD.LoadCaseName) == 0) StldK_Lk = StldK;
// 				if(_stricmp(swT, StldD.LoadCaseName) == 0) StldK_Tk = StldK;
// 			}
// 
// 			if(rData.CH2012.bCross)
// 			{
// 				if(StldK_Lk == 0) //이런경우는 추가. 기존에 없음. 다시 만든다.
// 				{
// 					StldD.LoadCaseType = "W";    // cross wind load
// 					StldD.LoadCaseName = swL;
// 					StldD.LoadCaseId = pStld->GetCount()+1;
// 					StldK = pStld->m_nStartNum;
// 					pStld->AddList(StldK);
// 					pStld->Add(StldK, StldD);
// 					pStld->AddListItem(StldK, LT_ARPR_CMD, Key);
// 				}
// 				else
// 				{
// 					pStld->AddListItem(StldK_Lk, LT_ARPR_CMD, Key);
// 				}
// 			}
// 
// 			if(rData.CH2012.bTorsion)
// 			{
// 				if(StldK_Tk == 0)
// 				{
// 					StldD.LoadCaseType = "W";    // cross wind load
// 					StldD.LoadCaseName = swT;
// 					StldD.LoadCaseId = pStld->GetCount()+1;
// 					StldK = pStld->m_nStartNum;
// 					pStld->AddList(StldK);
// 					pStld->Add(StldK, StldD);
// 					pStld->AddListItem(StldK, LT_ARPR_CMD, Key);
// 				}
// 				else
// 				{
// 					pStld->AddListItem(StldK_Tk, LT_ARPR_CMD, Key);
// 				}
// 			}
// 		}
// 	}
	//////////////////////////////////////////////////////////////////////////
}

BOOL CDB_ARPR::Del(T_ARPR_K Key, CDB_STLD* pStld, CDB_LDGR* pLdgr, CDB_LAPL* pLapl, CDB_ELEM* pElem)
{
	T_ARPR_D Data;
	BOOL bExist = m_arpr.Lookup(Key, Data);

	BOOL ret=m_arpr.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		if(bExist)
		{
			int nSize;

			if(Data.StldK != 0) VERIFY(pStld->DelListItem(Data.StldK, LT_ARPR_CMD, Key));
			if(Data.LaplK != 0) VERIFY(pLapl->DelListItem(Data.LaplK, LT_ARPR_CMD, Key));

			nSize = Data.aElemK.GetSize();
			for(int i = 0; i<nSize; i++) VERIFY(pElem->DelListItem(Data.aElemK[i], LT_ARPR_CMD, Key));

			// Current Stage가 Base Stage가 아니면 Stage DB를 Update한다.
			if (m_pDoc->m_pStagCtrl->GetCurStag() != 0)
			{
				m_pDoc->m_pStagCtrl->DeactiveLoad(LT_ARPR_CMD, Key);
			}

// 			if(CDBDoc::IsEnableWindPressure())
// 			{
// 				//////////////////////////////////////////////////////////////////////////
// 				// 여기는 중국 2012기준 관련....  관련 로드케이스 삭제
// 				T_STLD_D StldD;
// 				CString swd;
// 				CString swL;
// 				CString swT;
// 				T_STLD_K StldK;
// 				T_STLD_K StldK_Lk = 0;
// 				T_STLD_K StldK_Tk = 0;
// 				POSITION pos;
// 				StldList* pList;
// 				if( (Data.nCodeType == 19 && Data.CH2012.bCrossVibration) && 
// 					(Data.CH2012.bCross || Data.CH2012.bTorsion) ) //Consider Cross or torsion 일때만 존재할 것이기때문에.
// 				{
// 					if(!pStld->Get(Data.StldK, StldD)) ASSERT(0);
// 
// 					swd = StldD.LoadCaseName;
// 					swL = swd+"_Crs";
// 					swT = swd+"_Tor";
// 
// 					pos=pStld->GetStart();
// 					while(pos != NULL)
// 					{
// 						pStld->GetNext(pos, StldK, StldD);
// 						if(_stricmp(swL, StldD.LoadCaseName) == 0) StldK_Lk = StldK;
// 						if(_stricmp(swT, StldD.LoadCaseName) == 0) StldK_Tk = StldK;
// 					}
// 
// 					if(StldK_Lk > 0)
// 					{
// 						if(!pStld->DelListItem(StldK_Lk, LT_ARPR_CMD, Key)) ASSERT(0);
// 						pStld->GetList(StldK_Lk, pList);
// 						if(pList->GetCount() == 0) pStld->Del(StldK_Lk);
// 					}
// 					if(StldK_Tk > 0)
// 					{
// 						if(!pStld->DelListItem(StldK_Tk, LT_ARPR_CMD, Key)) ASSERT(0);
// 						pStld->GetList(StldK_Tk, pList);
// 						if(pList->GetCount() == 0) pStld->Del(StldK_Tk);
// 					}
// 				}
// 				//////////////////////////////////////////////////////////////////////////
// 			}

		}

		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_ARPR_K key;
			T_ARPR_D data;
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

BOOL CDB_ARPR::Get(T_ARPR_K Key, T_ARPR_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		T_KEY2 StagLdgrKey;
		if (!m_pDoc->m_pStagCtrl->m_arpr->Lookup(Key, StagLdgrKey)) return FALSE;
	}
	return m_arpr.Lookup(Key, rData);
}
int CDB_ARPR::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_arpr->GetCount();
	}
	return m_arpr.GetCount();
}
POSITION CDB_ARPR::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_arpr->GetStartPosition();
	}
	return m_arpr.GetStartPosition();
}
void CDB_ARPR::GetNext(POSITION& rNextPosition, T_ARPR_K& rKey, T_ARPR_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		T_KEY2 StagLdgrKey;
		m_pDoc->m_pStagCtrl->m_arpr->GetNextAssoc(rNextPosition, rKey, StagLdgrKey);
		m_arpr.Lookup(rKey, rData);
		return;
	}
	m_arpr.GetNextAssoc(rNextPosition, rKey, rData);
}