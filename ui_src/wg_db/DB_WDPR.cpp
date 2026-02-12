#include "stdafx.h"
#include "DB_WDPR.h"

#include "DBDoc.h"
#include "StagCtrl.h"
#include "DB_STLD.h"
#include "DB_LDGR.h"
#include "DB_WVEP.h"
#include "DB_LAPL.h"
#include "DB_ELEM.h"
#include "DB_NODE.h"
#include "DB_WDFC.h"

CDB_WDPR::CDB_WDPR()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nStartNum=1;
	m_nLastNum=0;
	m_wdpr.InitHashTable(HASHSIZEWDPR);
	m_wdprlist.InitHashTable(HASHSIZELISTWDPR);
	AddList(0); // add default group
}

CDB_WDPR::~CDB_WDPR()
{
	DelAllList();
}

void CDB_WDPR::Add(T_WDPR_K Key,T_WDPR_D& rData, CDB_STLD* pStld, CDB_LDGR* pLdgr, CDB_WVEP* pWvep, CDB_WDFC* pWdfc, CDB_LAPL* pLapl, CDB_ELEM* pElem, CDB_NODE* pNode)
{
	T_WDPR_D data;
	BOOL bExist=m_wdpr.Lookup(Key, data);

	CMap<T_ELEM_K, T_ELEM_K, T_ELEM_K, T_ELEM_K> mapElemNew;
	CMap<T_ELEM_K, T_ELEM_K, T_ELEM_K, T_ELEM_K> mapElemOld;
	CMap<T_NODE_K, T_NODE_K, T_NODE_K, T_NODE_K> mapNodeNew;
	CMap<T_NODE_K, T_NODE_K, T_NODE_K, T_NODE_K> mapNodeOld;

	int i;
	int nSize;
	T_ELEM_K KeyElemTemp;
	T_NODE_K KeyNodeTemp;

	if(bExist)
	{
		//다르면 제거한다.
		if(data.KeyStld != rData.KeyStld)
			VERIFY(pStld->DelListItem(data.KeyStld, LT_WDPR_CMD, Key));
		//     if(data.KeyLdgr != rData.KeyLdgr)
		//       VERIFY(pLdgr->DelListItem(data.KeyLdgr, LT_WDPR_CMD, Key));
		if(data.KeyWvep != rData.KeyWvep)
			VERIFY(pWvep->DelListItem(data.KeyWvep, LT_WDPR_CMD, Key));
		if (data.KeyWdfc != rData.KeyWdfc)
			VERIFY(pWdfc->DelListItem(data.KeyWdfc, LT_WDPR_CMD, Key));
		if(data.KeyLapl != rData.KeyLapl)
			VERIFY(pLapl->DelListItem(data.KeyLapl, LT_WDPR_CMD, Key));

		for(i=0 ; i<rData.arKeysDefineElem.GetSize(); i++) mapElemNew.SetAt(rData.arKeysDefineElem[i], rData.arKeysDefineElem[i]);
		for(i=0 ; i<data.arKeysDefineElem.GetSize() ; i++) mapElemOld.SetAt(data.arKeysDefineElem[i], data.arKeysDefineElem[i]);
		for(i=0 ; i<rData.arKeysDefineNode.GetSize(); i++) mapNodeNew.SetAt(rData.arKeysDefineNode[i], rData.arKeysDefineNode[i]);
		for(i=0 ; i<data.arKeysDefineNode.GetSize() ; i++) mapNodeOld.SetAt(data.arKeysDefineNode[i], data.arKeysDefineNode[i]);

		nSize = data.arKeysDefineElem.GetSize();
		for(int i = 0; i<nSize; i++)
		{
			if(!mapElemNew.Lookup(data.arKeysDefineElem[i], KeyElemTemp))
				VERIFY(pElem->DelListItem(data.arKeysDefineElem[i], LT_WDPR_CMD, Key));
		}
		nSize = data.arKeysDefineNode.GetSize();
		for(int i = 0; i<nSize; i++)
		{
			if(!mapNodeNew.Lookup(data.arKeysDefineNode[i], KeyNodeTemp))
				VERIFY(pNode->DelListItem(data.arKeysDefineNode[i], LT_WDPR_CMD, Key));
		}
	}

	m_wdpr.SetAt(Key,rData);
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
		if(data.KeyStld != rData.KeyStld)
			pStld->AddListItem(rData.KeyStld, LT_WDPR_CMD, Key);
		//     if(data.KeyLdgr != rData.KeyLdgr)
		//       pLdgr->AddListItem(rData.KeyLdgr, LT_WDPR_CMD, Key);
		if(data.KeyWvep != rData.KeyWvep)
			pWvep->AddListItem(rData.KeyWvep, LT_WDPR_CMD, Key);
		if (data.KeyWdfc != rData.KeyWdfc)
			pWdfc->AddListItem(rData.KeyWdfc, LT_WDPR_CMD, Key);
		if(data.KeyLapl != rData.KeyLapl)
			pLapl->AddListItem(rData.KeyLapl, LT_WDPR_CMD, Key);

		nSize = rData.arKeysDefineElem.GetSize();
		for(int i = 0; i<nSize; i++)
		{
			if(!mapElemOld.Lookup(rData.arKeysDefineElem[i], KeyElemTemp))
				pElem->AddListItem(rData.arKeysDefineElem[i], LT_WDPR_CMD, Key);
		}
		nSize = rData.arKeysDefineNode.GetSize();
		for(int i = 0; i<nSize; i++)
		{
			if(!mapNodeOld.Lookup(rData.arKeysDefineNode[i], KeyNodeTemp))
				pNode->AddListItem(rData.arKeysDefineNode[i], LT_WDPR_CMD, Key);
		}
	}
	else
	{
		pStld->AddListItem(rData.KeyStld, LT_WDPR_CMD, Key);
		//pLdgr->AddListItem(rData.KeyLdgr, LT_WDPR_CMD, Key);
		pWvep->AddListItem(rData.KeyWvep, LT_WDPR_CMD, Key);
		if (rData.KeyWdfc!=0)
			pWdfc->AddListItem(rData.KeyWdfc, LT_WDPR_CMD, Key);
		pLapl->AddListItem(rData.KeyLapl, LT_WDPR_CMD, Key);

		nSize = rData.arKeysDefineElem.GetSize();
		for(int i = 0; i<nSize; i++)
		{
			pElem->AddListItem(rData.arKeysDefineElem[i], LT_WDPR_CMD, Key);
		}
		nSize = rData.arKeysDefineNode.GetSize();
		for(int i = 0; i<nSize; i++)
		{
			pNode->AddListItem(rData.arKeysDefineNode[i], LT_WDPR_CMD, Key);
		}
	}
	//위부분은 Relation 처리를 위한 부분.

	//아래에는 중국코드 2012일때 load case를 추가함.
	/* Since there is no command for stld operations, put the stld operations in Editdat2.cpp:AddWdpr_CH_2012_Wind, where addlistitem is implemented by xuezc 2024-11-27*/
	if(CDBDoc::IsEnableWindPressure())
	{
		T_STLD_D StldD;
		CString swd;
		CString swL;
		CString swT;
		T_STLD_K StldK;
		T_STLD_K StldK_Lk = 0;
		T_STLD_K StldK_Tk = 0;
		StldList* pList;
		POSITION pos;
		if(bExist)
		{
			if((data.nCodeType == CH_W_2012 && data.CodeParam.CH2012.bCrossVibration && (data.CodeParam.CH2012.bCross || data.CodeParam.CH2012.bTorsion)) ||
			   (data.nCodeType == CH_W_2021 && data.CodeParam.CH2019.bCrossVibration && (data.CodeParam.CH2019.bCross || data.CodeParam.CH2019.bTorsion)))
			{
				if(!pStld->Get(data.KeyStld, StldD)) ASSERT(0);

				swd = StldD.LoadCaseName;
				swL = swd+_T("_Crs");
				swT = swd+_T("_Tor");

				pos=pStld->GetStart();
				while(pos != NULL)
				{
					pStld->GetNext(pos, StldK, StldD);
					if(_tcsicmp(swL, StldD.LoadCaseName) == 0) StldK_Lk = StldK;
					if(_tcsicmp(swT, StldD.LoadCaseName) == 0) StldK_Tk = StldK;
				}

				if(StldK_Lk > 0 && ((data.nCodeType == CH_W_2012 && data.CodeParam.CH2012.bCross) || (data.nCodeType == CH_W_2021 && data.CodeParam.CH2019.bCross)))
				{
					if(!pStld->DelListItem(StldK_Lk, LT_WDPR_CMD, Key)) ASSERT(0);
					//pStld->GetList(StldK_Lk, pList);
					//if(pList->GetCount() == 0) pStld->Del(StldK_Lk);
				}
				if(StldK_Tk > 0 && ((data.nCodeType == CH_W_2012 && data.CodeParam.CH2012.bTorsion) || (data.nCodeType == CH_W_2021 && data.CodeParam.CH2019.bTorsion)))
				{
					if(!pStld->DelListItem(StldK_Tk, LT_WDPR_CMD, Key)) ASSERT(0);
					//pStld->GetList(StldK_Tk, pList);
					//if(pList->GetCount() == 0) pStld->Del(StldK_Tk);
				}
			}
		}

		if((rData.nCodeType == CH_W_2012 && rData.CodeParam.CH2012.bCrossVibration && (rData.CodeParam.CH2012.bCross || rData.CodeParam.CH2012.bTorsion)) ||
		   (rData.nCodeType == CH_W_2021 && rData.CodeParam.CH2019.bCrossVibration && (rData.CodeParam.CH2019.bCross || rData.CodeParam.CH2019.bTorsion)))
		{
			if(!pStld->Get(rData.KeyStld, StldD)) ASSERT(0);

			StldK_Lk = 0;
			StldK_Tk = 0;
			swd = StldD.LoadCaseName;
			swL = swd+_T("_Crs");
			swT = swd+_T("_Tor");

			pos=pStld->GetStart();
			while(pos != NULL)
			{
				pStld->GetNext(pos, StldK, StldD);
				if(_tcsicmp(swL, StldD.LoadCaseName) == 0) StldK_Lk = StldK;
				if(_tcsicmp(swT, StldD.LoadCaseName) == 0) StldK_Tk = StldK;
			}

			if((rData.nCodeType == CH_W_2012 && rData.CodeParam.CH2012.bCross) || (rData.nCodeType == CH_W_2021 && rData.CodeParam.CH2019.bCross))
			{
				if(StldK_Lk == 0) //이런경우는 추가. 기존에 없음. 다시 만든다.
				{
					ASSERT(FALSE);
					//StldD.LoadCaseType = _T("W");    // cross wind load
					//StldD.LoadCaseName = swL;
					//StldD.LoadCaseId = pStld->GetCount()+1;
					//StldK = pStld->m_nStartNum;
					//pStld->AddList(StldK);
					//pStld->Add(StldK, StldD);
					//pStld->AddListItem(StldK, LT_WDPR_CMD, Key);
				}
				else
				{
					pStld->AddListItem(StldK_Lk, LT_WDPR_CMD, Key);
				}
			}

			if((rData.nCodeType == CH_W_2012 && rData.CodeParam.CH2012.bTorsion) || (rData.nCodeType == CH_W_2021 && rData.CodeParam.CH2019.bTorsion))
			{
				if(StldK_Tk == 0)
				{
					ASSERT(FALSE);
					//StldD.LoadCaseType = _T("W");    // cross wind load
					//StldD.LoadCaseName = swT;
					//StldD.LoadCaseId = pStld->GetCount()+1;
					//StldK = pStld->m_nStartNum;
					//pStld->AddList(StldK);
					//pStld->Add(StldK, StldD);
					//pStld->AddListItem(StldK, LT_WDPR_CMD, Key);
				}
				else
				{
					pStld->AddListItem(StldK_Tk, LT_WDPR_CMD, Key);
				}
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////
}

BOOL CDB_WDPR::Del(T_WDPR_K Key, CDB_STLD* pStld, CDB_LDGR* pLdgr, CDB_WVEP* pWvep, CDB_WDFC* pWdfc, CDB_LAPL* pLapl, CDB_ELEM* pElem, CDB_NODE* pNode)
{
	T_WDPR_D Data;
	BOOL bExist = m_wdpr.Lookup(Key, Data);

	BOOL ret=m_wdpr.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		if(bExist)
		{
			int nSize;

			if(Data.KeyStld != 0) VERIFY(pStld->DelListItem(Data.KeyStld, LT_WDPR_CMD, Key));
			//if(Data.KeyLdgr != 0) VERIFY(pLdgr->DelListItem(Data.KeyLdgr, LT_WDPR_CMD, Key));
			if(Data.KeyWvep != 0) VERIFY(pWvep->DelListItem(Data.KeyWvep, LT_WDPR_CMD, Key));
			if(Data.KeyWdfc != 0) VERIFY(pWdfc->DelListItem(Data.KeyWdfc, LT_WDPR_CMD, Key));
			if(Data.KeyLapl != 0) VERIFY(pLapl->DelListItem(Data.KeyLapl, LT_WDPR_CMD, Key));

			nSize = Data.arKeysDefineElem.GetSize();
			for(int i = 0; i<nSize; i++) VERIFY(pElem->DelListItem(Data.arKeysDefineElem[i], LT_WDPR_CMD, Key));
			nSize = Data.arKeysDefineNode.GetSize();
			for(int i = 0; i<nSize; i++) VERIFY(pNode->DelListItem(Data.arKeysDefineNode[i], LT_WDPR_CMD, Key));

			/* Since there is no command for stld operations, put the stld operations in Editdat2.cpp, where addlistitem is implemented by xuezc 2024-11-27*/
			if(CDBDoc::IsEnableWindPressure())
			{
				//////////////////////////////////////////////////////////////////////////
				// 여기는 중국 2012기준 관련....  관련 로드케이스 삭제
				T_STLD_D StldD;
				CString swd;
				CString swL;
				CString swT;
				T_STLD_K StldK;
				T_STLD_K StldK_Lk = 0;
				T_STLD_K StldK_Tk = 0;
				POSITION pos;
				StldList* pList;
				//Consider Cross or torsion 일때만 존재할 것이기때문에.
				if((Data.nCodeType == CH_W_2012 && Data.CodeParam.CH2012.bCrossVibration && (Data.CodeParam.CH2012.bCross || Data.CodeParam.CH2012.bTorsion)) ||
				   (Data.nCodeType == CH_W_2021 && Data.CodeParam.CH2019.bCrossVibration && (Data.CodeParam.CH2019.bCross || Data.CodeParam.CH2019.bTorsion)))
				{
					if(!pStld->Get(Data.KeyStld, StldD)) ASSERT(0);

					swd = StldD.LoadCaseName;
					swL = swd+_T("_Crs");
					swT = swd+_T("_Tor");

					pos=pStld->GetStart();
					while(pos != NULL)
					{
						pStld->GetNext(pos, StldK, StldD);
						if(_tcsicmp(swL, StldD.LoadCaseName) == 0) StldK_Lk = StldK;
						if(_tcsicmp(swT, StldD.LoadCaseName) == 0) StldK_Tk = StldK;
					}

					if(StldK_Lk > 0 && ((Data.nCodeType == CH_W_2012 && Data.CodeParam.CH2012.bCross) || (Data.nCodeType == CH_W_2021 && Data.CodeParam.CH2019.bCross)))
					{
						if(!pStld->DelListItem(StldK_Lk, LT_WDPR_CMD, Key)) ASSERT(0);
						//pStld->GetList(StldK_Lk, pList);
						//if(pList->GetCount() == 0) pStld->Del(StldK_Lk);
					}
					if(StldK_Tk > 0 && ((Data.nCodeType == CH_W_2012 && Data.CodeParam.CH2012.bTorsion) || (Data.nCodeType == CH_W_2021 && Data.CodeParam.CH2019.bTorsion)))
					{
						if(!pStld->DelListItem(StldK_Tk, LT_WDPR_CMD, Key)) ASSERT(0);
						//pStld->GetList(StldK_Tk, pList);
						//if(pList->GetCount() == 0) pStld->Del(StldK_Tk);
					}
				}
				//////////////////////////////////////////////////////////////////////////
			}
		}

		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_WDPR_K key;
			T_WDPR_D data;
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

void CDB_WDPR::AddList(T_WDPR_K Key)
{
	WdprList* pList = new WdprList;
	m_wdprlist.SetAt(Key, pList);
}

BOOL CDB_WDPR::DelList(T_WDPR_K Key)
{
	WdprList* pList;
	BOOL bExist=m_wdprlist.Lookup(Key,pList);
	ASSERT(bExist);
	if(!bExist)return FALSE;
	m_wdprlist.RemoveKey(Key);
	delete pList;
	return TRUE;
}

void CDB_WDPR::DelAllList()
{
	T_WDPR_K Key;
	WdprList* pList;
	POSITION pos=m_wdprlist.GetStartPosition();
	while(pos != NULL)
	{
		m_wdprlist.GetNextAssoc(pos,Key,pList);
		delete pList;
	}
	m_wdprlist.RemoveAll();
}

void CDB_WDPR::AddListItem(T_WDPR_K Key, int nCmd, int nKey)
{
	LaplList* pList;
	if(!GetList(Key, pList))return;

	T_LIST_DATA ListData;
	ListData.nCmd=nCmd;
	ListData.nKey=nKey;
	pList->AddTail(ListData);
}

BOOL CDB_WDPR::DelListItem(T_WDPR_K Key, int nCmd, int nKey)
{
	LaplList* pList;
	if(!GetList(Key, pList))return TRUE;

	T_LIST_DATA ListData;
	POSITION pos=pList->GetHeadPosition(), posPrv;
	while(pos != NULL)
	{
		posPrv=pos;
		ListData=pList->GetNext(pos);
		if(ListData.nCmd == nCmd && ListData.nKey == nKey)
		{
			pList->RemoveAt(posPrv);
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CDB_WDPR::GetList(T_WDPR_K Key,WdprList*& rpList)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		ASSERT(0);
		//return m_pDoc->m_pStagCtrl->m_wdprlist->Lookup(Key,rpList);
	}
	return m_wdprlist.Lookup(Key,rpList);
}

