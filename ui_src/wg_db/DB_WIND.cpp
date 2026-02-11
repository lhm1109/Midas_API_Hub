#include "stdafx.h"
#include "DB_STLD.h"
#include "DB_WIND.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_WIND::CDB_WIND()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_wind.InitHashTable(HASHSIZEWIND);
}

CDB_WIND::~CDB_WIND()
{

}

void CDB_WIND::Add(T_WIND_K Key,T_WIND_D& rData,CDB_STLD* pStld)
{
	T_WIND_D Data;
	BOOL bExist=m_wind.Lookup(Key, Data);
	m_wind.SetAt(Key,rData);
	if(!bExist)pStld->AddListItem(Key, LT_WIND_CMD, Key);
	//////////////////////////////////////////////////////////////////////////
	//T_STLD_D stld;  T_STLD_K key2;
	//pStld->Get(Key,stld);
	//CString swd = stld.LoadCaseName;
	//CString swL = swd+_T("_Lk");
	//CString swT = swd+_T("_Tk");
	//if (Data.nCodeType == 19 || Data.nCodeType == 23) //add by maxiao 2012-3-27
	//{
	//	POSITION pos = pStld->GetStart();
	//	while(pos != NULL)
	//	{
	//		pStld->GetNext(pos, key2, stld);
	//		if(_tcsicmp(swL, stld.LoadCaseName) == 0)
	//		{
	//			if(rData.nCodeType != 19 && rData.nCodeType != 23) pStld->Del(key2);
	//		}
	//		if(_tcsicmp(swT, stld.LoadCaseName) == 0)
	//		{
	//			if(rData.nCodeType != 19 && rData.nCodeType != 23) pStld->Del(key2);
	//		}
	//	}
	//}
	if(bExist)
	{
		if((Data.nCodeType == CH_W_2012 && Data.CodeParam.CH2012.bCrossVibration && (Data.CodeParam.CH2012.bCross || Data.CodeParam.CH2012.bTorsion)) ||
		   (Data.nCodeType == CH_W_2021 && Data.CodeParam.CH2019.bCrossVibration && (Data.CodeParam.CH2019.bCross || Data.CodeParam.CH2019.bTorsion)))
		{
			T_STLD_K StldK;
			T_STLD_D StldD;
			if(!pStld->Get(Key, StldD)) ASSERT(0);
			T_STLD_K StldK_Lk = 0;
			T_STLD_K StldK_Tk = 0;
			CString swd = StldD.LoadCaseName;
			CString swL = swd + _T("_Lk");
			CString swT = swd + _T("_Tk");
			POSITION pos = pStld->GetStart();
			while(pos != NULL)
			{
				pStld->GetNext(pos, StldK, StldD);
				if(_tcsicmp(swL, StldD.LoadCaseName) == 0) StldK_Lk = StldK;
				if(_tcsicmp(swT, StldD.LoadCaseName) == 0) StldK_Tk = StldK;
			}
			if(StldK_Lk > 0 && ((Data.nCodeType == CH_W_2012 && Data.CodeParam.CH2012.bCross) || (Data.nCodeType == CH_W_2021 && Data.CodeParam.CH2019.bCross)))
			{
				if(!pStld->DelListItem(StldK_Lk, LT_WIND_CMD, Key)) ASSERT(0);
			}
			if(StldK_Tk > 0 && ((Data.nCodeType == CH_W_2012 && Data.CodeParam.CH2012.bTorsion) || (Data.nCodeType == CH_W_2021 && Data.CodeParam.CH2019.bTorsion)))
			{
				if(!pStld->DelListItem(StldK_Tk, LT_WIND_CMD, Key)) ASSERT(0);
			}
		}
	}

	if((rData.nCodeType == CH_W_2012 && rData.CodeParam.CH2012.bCrossVibration && (rData.CodeParam.CH2012.bCross || rData.CodeParam.CH2012.bTorsion)) ||
	   (rData.nCodeType == CH_W_2021 && rData.CodeParam.CH2019.bCrossVibration && (rData.CodeParam.CH2019.bCross || rData.CodeParam.CH2019.bTorsion)))	//add by maxiao 2012-3-27
	{
		T_STLD_D stld;  T_STLD_K StldK;
		pStld->Get(Key, stld);
		T_STLD_K StldK_Lk = 0;
		T_STLD_K StldK_Tk = 0;
		CString swd = stld.LoadCaseName;
		CString swL = swd + _T("_Lk");
		CString swT = swd + _T("_Tk");
		POSITION pos=pStld->GetStart();
		while(pos != NULL)
		{
			pStld->GetNext(pos,StldK,stld);
			if(_tcsicmp(swL, stld.LoadCaseName) == 0 ) StldK_Lk = StldK;
			if(_tcsicmp(swT, stld.LoadCaseName) == 0 ) StldK_Tk = StldK;
		}
		//stld.LoadCaseType = _T("W");    // cross wind load
		//stld.LoadCaseName = swL;
		//stld.LoadCaseId = pStld->GetCount()+1;
		//if (rData.nCodeType == CH_W_2012 && rData.CodeParam.CH2012.bCross && rData.CodeParam.CH2012.bCrossVibration && bL)
		//{
		//	int n = pStld->m_nStartNum;
		//	pStld->AddList(n);
		//	pStld->Add(n, stld);
		//}
		//stld.LoadCaseName = swT;
		//stld.LoadCaseId = pStld->GetCount()+1;
		//if (rData.CodeParam.CH2012.bTorsion && rData.CodeParam.CH2012.bCrossVibration && bT)
		//{
		//	int n = pStld->m_nStartNum;			
		//	pStld->AddList(n);
		//	pStld->Add(n, stld);	
		//}

		if((rData.nCodeType == CH_W_2012 && rData.CodeParam.CH2012.bCross) || (rData.nCodeType == CH_W_2021 && rData.CodeParam.CH2019.bCross))
		{
			if(StldK_Lk != 0)
			{
				pStld->AddListItem(StldK_Lk, LT_WIND_CMD, Key);
			}
			else
				ASSERT(FALSE);
		}

		if((rData.nCodeType == CH_W_2012 && rData.CodeParam.CH2012.bTorsion) || (rData.nCodeType == CH_W_2021 && rData.CodeParam.CH2019.bTorsion))
		{
			if(StldK_Tk != 0)
			{
				pStld->AddListItem(StldK_Tk, LT_WIND_CMD, Key);
			}
			else
				ASSERT(FALSE);
		}
	}	
}

BOOL CDB_WIND::Del(T_WIND_K Key,CDB_STLD* pStld)
{
	T_WIND_D Data;
	BOOL bExist=m_wind.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_wind.RemoveKey(Key);
		ASSERT(ret);
		if(ret)
		{
			/* Since there is no command for stld operations, put the stld operations in Editdat.cpp:AddWind_CH_2012_Wind, where addlistitem is implemented by xuezc 2024-11-27*/
			//T_STLD_D stld;  T_STLD_K key2;
			//pStld->Get(Key,stld);
			//CString swd = stld.LoadCaseName;
			//CString swL = swd+_T("_Lk");
			//CString swT = swd+_T("_Tk");
			//if(Data.nCodeType == CH_W_2012 || Data.nCodeType == CH_W_2021) //add by maxiao 2012-3-27
			//{
			//	POSITION pos = pStld->GetStart();
			//	while(pos != NULL)
			//	{
			//		pStld->GetNext(pos, key2, stld);
			//		if(_tcsicmp(swL, stld.LoadCaseName) == 0 || _tcsicmp(swT, stld.LoadCaseName) == 0)
			//			pStld->Del(key2);
			//	}
			//}
			//////////////////////////////////////////////////////////////////////////
			//VERIFY(pStld->DelListItem(Key, LT_WIND_CMD, Key));
			if((Data.nCodeType == CH_W_2012 && Data.CodeParam.CH2012.bCrossVibration && (Data.CodeParam.CH2012.bCross || Data.CodeParam.CH2012.bTorsion)) ||
			   (Data.nCodeType == CH_W_2021 && Data.CodeParam.CH2019.bCrossVibration && (Data.CodeParam.CH2019.bCross || Data.CodeParam.CH2019.bTorsion)))
			{
				T_STLD_D StldD;
				if(!pStld->Get(Key, StldD)) ASSERT(0);
				T_STLD_K StldK_Lk = 0;
				T_STLD_K StldK_Tk = 0;
				T_STLD_K StldK;
				CString swd = StldD.LoadCaseName;
				CString swL = swd + _T("_Lk");
				CString swT = swd + _T("_Tk");
				POSITION pos = pStld->GetStart();
				while(pos != NULL)
				{
					pStld->GetNext(pos, StldK, StldD);
					if(_tcsicmp(swL, StldD.LoadCaseName) == 0) StldK_Lk = StldK;
					if(_tcsicmp(swT, StldD.LoadCaseName) == 0) StldK_Tk = StldK;
				}
				if(StldK_Lk > 0 && ((Data.nCodeType == CH_W_2012 && Data.CodeParam.CH2012.bCross) || (Data.nCodeType == CH_W_2021 && Data.CodeParam.CH2019.bCross)))
				{
					if(!pStld->DelListItem(StldK_Lk, LT_WIND_CMD, Key)) ASSERT(0);
				}
				if(StldK_Tk > 0 && ((Data.nCodeType == CH_W_2012 && Data.CodeParam.CH2012.bTorsion) || (Data.nCodeType == CH_W_2021 && Data.CodeParam.CH2019.bTorsion)))
				{
					if(!pStld->DelListItem(StldK_Tk, LT_WIND_CMD, Key)) ASSERT(0);
				}
			}
		}
		return ret;
	}
	return bExist;
}


//--------------------------------------------------------------------------
BOOL CDB_WIND::Get(T_WIND_K Key,T_WIND_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_wind.Lookup(Key,rData);
}
int CDB_WIND::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_wind.GetCount();
}

POSITION CDB_WIND::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_wind.GetStartPosition();
}

void CDB_WIND::GetNext(POSITION& rNextPosition,T_WIND_K& rKey,T_WIND_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_wind.GetNextAssoc(rNextPosition,rKey,rData);
}
