#include "stdafx.h"

#include "../wg_db/DBDoc.h"
#include "../wg_db/AttrCtrl2.h"
#include "../wg_db/AttrCtrl.h"
#include "../wg_db/DataCtrl.h"

#include "RatingLoadCombUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


void CRatingLoadCombUtil::CopySteelRatingToLCom(CDBDoc* pDoc)
{
	T_RLCS_D tRlcs;
	T_LCOM_D tLcomComb;
	CArray<T_RLCS_K, T_RLCS_K> rKeyList;
	pDoc->m_pAttrCtrl2->GetRlcsKeyList(rKeyList);
	for(int i = 0; i < rKeyList.GetSize(); i++)
	{
		pDoc->m_pAttrCtrl2->GetRlcs(rKeyList[i], tRlcs);
		
		//ChangeToUniqueLcomName(pDoc, tRlcs.strCaseName);
		
		tLcomComb.Initialize();
		GetLComByRlcs(pDoc, tRlcs, tLcomComb, 0);
		if(i == 0)
		{
			if(!ReplaceLcom(pDoc, tLcomComb, 1))	return;
		}
		else
		{
			if(!ReplaceLcom(pDoc, tLcomComb, 3))	return;
		}
		
		tLcomComb.Initialize();
		GetLComByRlcs(pDoc, tRlcs, tLcomComb, 1);
		if(!ReplaceLcom(pDoc, tLcomComb, 3))	return;
		
		tLcomComb.Initialize();
		GetLComByRlcs(pDoc, tRlcs, tLcomComb, 2);
		if(i == rKeyList.GetSize() - 1)
		{
			if(!ReplaceLcom(pDoc, tLcomComb, 2))	return;
		}		
		else
		{
			if(!ReplaceLcom(pDoc, tLcomComb, 3))	return;
		}
	}
}


void CRatingLoadCombUtil::CopyPSCAssessmentToLCom(CDBDoc* pDoc)
{
	T_ALCS_D tAlcs;
	T_LCOM_D tLcomComb;
	CArray<T_ALCS_K, T_ALCS_K> rKeyList;
	pDoc->m_pAttrCtrl2->GetAlcsKeyList(rKeyList);
	for(int i = 0; i < rKeyList.GetSize(); i++)
	{
		pDoc->m_pAttrCtrl2->GetAlcs(rKeyList[i], tAlcs);

		//ChangeToUniqueLcomName(pDoc, tAlcs.strCaseName);

		tLcomComb.Initialize();
		GetLComByAlcs(pDoc, tAlcs, tLcomComb, 0);
		if(i == 0)
		{
			if(!ReplaceLcom(pDoc, tLcomComb, 1))	return;
		}
		else
		{
			if(!ReplaceLcom(pDoc, tLcomComb, 3))	return;
		}

		tLcomComb.Initialize();
		GetLComByAlcs(pDoc, tAlcs, tLcomComb, 1);
		if(!ReplaceLcom(pDoc, tLcomComb, 3))	return;

		tLcomComb.Initialize();
		GetLComByAlcs(pDoc, tAlcs, tLcomComb, 2);
		if(i == rKeyList.GetSize() - 1)
		{
			if(!ReplaceLcom(pDoc, tLcomComb, 2))	return;
		}		
		else
		{
			if(!ReplaceLcom(pDoc, tLcomComb, 3))	return;
		}
	}
}

void CRatingLoadCombUtil::CopySteelAssessmentToLCom(CDBDoc* pDoc)
{
	T_ALCS_D tAlcs;
	T_LCOM_D tLcomComb;
	CArray<T_ALCS_K, T_ALCS_K> rKeyList;
	pDoc->m_pAttrCtrl2->GetAlcsKeyList(rKeyList);
	for (int i = 0; i < rKeyList.GetSize(); i++)
	{
		pDoc->m_pAttrCtrl2->GetAlcs(rKeyList[i], tAlcs);

		//ChangeToUniqueLcomName(pDoc, tAlcs.strCaseName);

		tLcomComb.Initialize();
		GetLComByAlcs(pDoc, tAlcs, tLcomComb, 0);
		if (i == 0)
		{
			if (!ReplaceLcom(pDoc, tLcomComb, 1))	return;
		}
		else
		{
			if (!ReplaceLcom(pDoc, tLcomComb, 3))	return;
		}

		tLcomComb.Initialize();
		GetLComByAlcs(pDoc, tAlcs, tLcomComb, 1);
		if (!ReplaceLcom(pDoc, tLcomComb, 3))	return;

		tLcomComb.Initialize();
		GetLComByAlcs(pDoc, tAlcs, tLcomComb, 2);
		if (i == rKeyList.GetSize() - 1)
		{
			if (!ReplaceLcom(pDoc, tLcomComb, 2))	return;
		}
		else
		{
			if (!ReplaceLcom(pDoc, tLcomComb, 3))	return;
		}
	}
}

void CRatingLoadCombUtil::CopySteelNRToLCom(CDBDoc* pDoc)
{
	T_ALCS_D tAlcs;
	T_LCOM_D tLcomComb;
	CArray<T_ALCS_K, T_ALCS_K> rKeyList;
	pDoc->m_pAttrCtrl2->GetAlcsKeyList(rKeyList);
	for (int i = 0; i < rKeyList.GetSize(); i++)
	{
		pDoc->m_pAttrCtrl2->GetAlcs(rKeyList[i], tAlcs);

		if (!tAlcs.bCombinedMV && !tAlcs.bSpecialMV) { ASSERT(0); return; }

		tLcomComb.Initialize();
		GetLComByAlcsNR(pDoc, tAlcs, tLcomComb, 0);

		if (i == 0)
		{
			if (!ReplaceLcom(pDoc, tLcomComb, 1))	return;
		}
		else
		{
			if (!ReplaceLcom(pDoc, tLcomComb, 3))	return;
		}

		if (tAlcs.bCombinedMV)
		{
			tLcomComb.Initialize();
			GetLComByAlcsNR(pDoc, tAlcs, tLcomComb, 1);
			if (!ReplaceLcom(pDoc, tLcomComb, 3))	return;
		}

		if (tAlcs.bSpecialMV)
		{
			tLcomComb.Initialize();
			GetLComByAlcsNR(pDoc, tAlcs, tLcomComb, 2);
			if (!ReplaceLcom(pDoc, tLcomComb, 3))	return;
		}

		if (tAlcs.bCombinedMV && !tAlcs.bSpecialMV)
		{
			tLcomComb.Initialize();
			GetLComByAlcsNR(pDoc, tAlcs, tLcomComb, 3);
			if (i == rKeyList.GetSize() - 1)
			{
				if (!ReplaceLcom(pDoc, tLcomComb, 2))	return;
			}
			else
			{
				if (!ReplaceLcom(pDoc, tLcomComb, 3))	return;
			}
		}
		else if (!tAlcs.bCombinedMV && tAlcs.bSpecialMV)
		{
			tLcomComb.Initialize();
			GetLComByAlcsNR(pDoc, tAlcs, tLcomComb, 4);
			if (i == rKeyList.GetSize() - 1)
			{
				if (!ReplaceLcom(pDoc, tLcomComb, 2))	return;
			}
			else
			{
				if (!ReplaceLcom(pDoc, tLcomComb, 3))	return;
			}
		}
		else
		{
			tLcomComb.Initialize();
			GetLComByAlcsNR(pDoc, tAlcs, tLcomComb, 3);
			if (!ReplaceLcom(pDoc, tLcomComb, 3))	return;

			tLcomComb.Initialize();
			GetLComByAlcsNR(pDoc, tAlcs, tLcomComb, 4);
			if (i == rKeyList.GetSize() - 1)
			{
				if (!ReplaceLcom(pDoc, tLcomComb, 2))	return;
			}
			else
			{
				if (!ReplaceLcom(pDoc, tLcomComb, 3))	return;
			}
		}		
	}
}

BOOL CRatingLoadCombUtil::ReplaceLcom(CDBDoc* pDoc, T_LCOM_D& tLcom, int nTransectionType)
{
	if(pDoc->m_pAttrCtrl->ExistLcom(D_LCOMTYPE_GENERAL, tLcom.LoadCombName))
	{
		T_LCOM_D tLComTemp;
		pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_GENERAL, tLcom.LoadCombName, tLComTemp);
		tLcom.LoadCombId = tLComTemp.LoadCombId;
		if(!pDoc->m_pDataCtrl->ModifyLcom(D_LCOMTYPE_GENERAL, tLcom.LoadCombName, tLcom, nTransectionType))				return FALSE;
	}
	else
	{
		if(!pDoc->m_pDataCtrl->AddLcom(D_LCOMTYPE_GENERAL, tLcom, nTransectionType))				return FALSE;
	}
	return TRUE;
}

void CRatingLoadCombUtil::GetLComByRlcs(CDBDoc* pDoc, T_RLCS_D &tRlcs, T_LCOM_D& tLcom, int nCopyType)  // nCopyType: 0) Main + Combinaton 1) Main + MV 2) Main + Combinaton + MV
{
	// Type, Active
	tLcom.LoadCombType = 0;
	tLcom.nActive = 1;

	// name, Combination
	switch(nCopyType)
	{
	case 0:  // Main + Combinaton
		{
			tLcom.LoadCombName = tRlcs.strCaseName + _T("(Dead Load)");
			tLcom.Description = _T("Steel Bridge Rating Design");

			if(tRlcs.arLoadComb.GetSize() > D_LCOM_NUMCOMB)
			{
				ASSERT(0);
				return ;
			}
			tLcom.aCombination.SetSize(tRlcs.arLoadComb.GetSize());
			for(int i = 0; i < tRlcs.arLoadComb.GetSize(); i++)
			{
				if (i<tRlcs.arStaticLoadComb.GetSize()) 
				{
					if (tRlcs.arStaticLoadComb[i]==FALSE) continue;       
				}        
				tLcom.aCombination[i] = tRlcs.arLoadComb[i];
			}	
		}
		break;
	case 1:  // Main + MV
		{
			tLcom.LoadCombName = tRlcs.strCaseName + _T("(Live Load)");
			tLcom.Description = _T("Steel Bridge Rating Design");

			tLcom.aCombination.Add(tRlcs.MoveLoadCase);
		}
	break;
	case 2:  // Main + Combinaton + MV
		{
			tLcom.LoadCombName = tRlcs.strCaseName;
			tLcom.Description = _T("Steel Bridge Design");

			int nLoadCombSize = tRlcs.arLoadComb.GetSize();
			if(nLoadCombSize + 1 > D_LCOM_NUMCOMB)
			{
				ASSERT(0);
				return ;
			}
			
// 			for(int i = 0; i < nLoadCombSize; i++)
// 			{
// 				tLcom.Combination[i] = tRlcs.arLoadComb[i];
// 			}	
			tLcom.aCombination.Copy(tRlcs.arLoadComb);
			tLcom.aCombination.Add(tRlcs.MoveLoadCase);
		}
	break;
	default:
		ASSERT(0);
	}

	//ChangeToUniqueLcomName(pDoc, tLcom.LoadCombName);
}


void CRatingLoadCombUtil::GetLComByAlcs(CDBDoc* pDoc, T_ALCS_D &tAlcs, T_LCOM_D& tLcom, int nCopyType)  // nCopyType: 0) Main + Combinaton 1) Main + MV 2) Main + Combinaton + MV
{
	// Type, Active
	tLcom.LoadCombType = 0;
	tLcom.nActive = 1;

	// name, Combination
	switch(nCopyType)
	{
	case 0:  // Main + Combinaton
		{
			tLcom.LoadCombName = tAlcs.strCaseName + _T("(Dead Load)");
			tLcom.Description = _T("PSC Bridge Rating Design");

			if(tAlcs.arLoadComb.GetSize() > D_LCOM_NUMCOMB)
			{
				ASSERT(0);
				return ;
			}
			tLcom.aCombination.SetSize(tAlcs.arLoadComb.GetSize());
			for(int i = 0; i < tAlcs.arLoadComb.GetSize(); i++)
			{
				tLcom.aCombination[i] = tAlcs.arLoadComb[i];
			}	
		}
		break;
	case 1:  // Main + MV
		{
			tLcom.LoadCombName = tAlcs.strCaseName + _T("(Live Load)");
			tLcom.Description = _T("PSC Bridge Rating Design");

            // CIVIL-1063 khj0102
            if (tAlcs.nMovType == 1) //Special인 경우
            {
                tLcom.aCombination.Add(tAlcs.SpecialMVCase);
            }
            else //Combined인 경우
            {
                tLcom.aCombination.Add(tAlcs.CombinedMVCase);
            }


			// CIVIL-1063
// 			if(tAlcs.bStdMVCase)
// 			{
// 				tLcom.aCombination.Add(tAlcs.StdMVCase);
// 			}
// 			if(tAlcs.bSpecialMVCase)
// 			{
// 				tLcom.aCombination.Add(tAlcs.SpecialMVCase);
// 			}
		}
		break;
	case 2:  // Main + Combinaton + MV
		{
			tLcom.LoadCombName = tAlcs.strCaseName;
			tLcom.Description = _T("PSC Bridge Design");

			int nLoadCombSize = tAlcs.arLoadComb.GetSize();
			if(nLoadCombSize + 1 > D_LCOM_NUMCOMB)
			{
				ASSERT(0);
				return ;
			}

			// 			for(int i = 0; i < nLoadCombSize; i++)
			// 			{
			// 				tLcom.Combination[i] = tAlcs.arLoadComb[i];
			// 			}	
			tLcom.aCombination.Copy(tAlcs.arLoadComb);

            // CIVIL-1063 khj0102
            if (tAlcs.nMovType == 1) //Special인 경우
            {
                tLcom.aCombination.Add(tAlcs.SpecialMVCase);
            }
            else //Combined인 경우
            {
                tLcom.aCombination.Add(tAlcs.CombinedMVCase);
            }


			// CIVIL-1063
// 			if(tAlcs.bStdMVCase)
// 			{
// 				tLcom.aCombination.Add(tAlcs.StdMVCase);
// 			}
// 			if(tAlcs.bSpecialMVCase)
// 			{
// 				tLcom.aCombination.Add(tAlcs.SpecialMVCase);
// 			}
		}
		break;
	default:
		ASSERT(0);
	}

	//ChangeToUniqueLcomName(pDoc, tLcom.LoadCombName);
}

void CRatingLoadCombUtil::GetLComByAlcsNR(CDBDoc* pDoc, T_ALCS_D& tAlcs, T_LCOM_D& tLcom, int nCopyType)
{
	// Type, Active
	tLcom.LoadCombType = 0;
	tLcom.nActive = 1;

	// name, Combination
	switch (nCopyType)
	{
	case 0:
	{
		tLcom.LoadCombName = tAlcs.strCaseName + _T("(Dead Load)");
		tLcom.Description = _T("");

		if (tAlcs.arLoadComb.GetSize() > D_LCOM_NUMCOMB)
		{
			ASSERT(0);
			return;
		}
		tLcom.aCombination.SetSize(tAlcs.arLoadComb.GetSize());
		for (int i = 0; i < tAlcs.arLoadComb.GetSize(); i++)
		{
			tLcom.aCombination[i] = tAlcs.arLoadComb[i];
		}
	}
	break;
	case 1:
	{
		tLcom.LoadCombName = tAlcs.strCaseName + _T("(Static Live)");
		tLcom.Description = _T("");

		tLcom.aCombination.Add(tAlcs.CombinedMVCase);
	}
	break;
	case 2:
	{
		tLcom.LoadCombName = tAlcs.strCaseName + _T("(Dynamic Live)");
		tLcom.Description = _T("");

		tLcom.aCombination.Add(tAlcs.SpecialMVCase);
	}
	break;
	case 3:
	{
		tLcom.LoadCombName = tAlcs.strCaseName + _T("(Static)");
		tLcom.Description = _T("");

		int nLoadCombSize = tAlcs.arLoadComb.GetSize();
		if (nLoadCombSize + 1 > D_LCOM_NUMCOMB)
		{
			ASSERT(0);
			return;
		}

		tLcom.aCombination.Copy(tAlcs.arLoadComb);

		tLcom.aCombination.Add(tAlcs.CombinedMVCase);
	}
	break;
	case 4:
	{
		tLcom.LoadCombName = tAlcs.strCaseName + _T("(Dynamic)");
		tLcom.Description = _T("");

		int nLoadCombSize = tAlcs.arLoadComb.GetSize();
		if (nLoadCombSize + 1 > D_LCOM_NUMCOMB)
		{
			ASSERT(0);
			return;
		}

		tLcom.aCombination.Copy(tAlcs.arLoadComb);

		tLcom.aCombination.Add(tAlcs.SpecialMVCase);
	}
	break;
	default:
		ASSERT(0);
	}

	//ChangeToUniqueLcomName(pDoc, tLcom.LoadCombName);
}

void CRatingLoadCombUtil::CopyPSCRatingToLCom(CDBDoc* pDoc)
{
	T_RKLC_D tRklc;
	T_LCOM_D tLcomComb;
	CArray<T_RKLC_K, T_RKLC_K> rKeyList;
	pDoc->m_pAttrCtrl->GetRklcKeyList(rKeyList);
	for(int i = 0; i < rKeyList.GetSize(); i++)
	{
		pDoc->m_pAttrCtrl->GetRklc(rKeyList[i], tRklc);
		
		//ChangeToUniqueLcomName(pDoc, tRklc.strCaseName);
		
		tLcomComb.Initialize();
		GetLComByRklc(pDoc, tRklc, tLcomComb, 0);
		if(i == 0)
		{
			if(!ReplaceLcom(pDoc, tLcomComb, 1))	return;
		}
		else
		{
			if(!ReplaceLcom(pDoc, tLcomComb, 3))	return;
		}
		
		tLcomComb.Initialize();
		GetLComByRklc(pDoc, tRklc, tLcomComb, 1);
		if(!ReplaceLcom(pDoc, tLcomComb, 3))	return;
		

		tLcomComb.Initialize();
		GetLComByRklc(pDoc, tRklc, tLcomComb, 2);
		if(i == rKeyList.GetSize() - 1)
		{
			if(!ReplaceLcom(pDoc, tLcomComb, 2))	return;
		}		
		else
		{
			if(!ReplaceLcom(pDoc, tLcomComb, 3))	return;
		}
	}
}

void CRatingLoadCombUtil::CopyRCRatingToLCom(CDBDoc* pDoc)
{
	T_RKLC_D tRklc;
	T_LCOM_D tLcomComb;
	CArray<T_RKLC_K, T_RKLC_K> rKeyList;
	pDoc->m_pAttrCtrl->GetRklcKeyList(rKeyList);
	for(int i = 0; i < rKeyList.GetSize(); i++)
	{
		pDoc->m_pAttrCtrl->GetRklc(rKeyList[i], tRklc);

		//ChangeToUniqueLcomName(pDoc, tRklc.strCaseName);

		tLcomComb.Initialize();
		GetLComByRklc(pDoc, tRklc, tLcomComb, 0);
		if(i == 0)
		{
			if(!ReplaceLcom(pDoc, tLcomComb, 1))	return;
		}
		else
		{
			if(!ReplaceLcom(pDoc, tLcomComb, 3))	return;
		}

		tLcomComb.Initialize();
		GetLComByRklc(pDoc, tRklc, tLcomComb, 1);
		if(!ReplaceLcom(pDoc, tLcomComb, 3))	return;


		tLcomComb.Initialize();
		GetLComByRklc(pDoc, tRklc, tLcomComb, 2);
		if(i == rKeyList.GetSize() - 1)
		{
			if(!ReplaceLcom(pDoc, tLcomComb, 2))	return;
		}		
		else
		{
			if(!ReplaceLcom(pDoc, tLcomComb, 3))	return;
		}
	}
}
void CRatingLoadCombUtil::GetLComByRklc(CDBDoc* pDoc, T_RKLC_D &tRklc, T_LCOM_D& tLcom, int nCopyType)  // nCopyType: 0) Main + Combinaton 1) Main + MV 2) Main + Combinaton + MV
{
	// Type, Active
	tLcom.LoadCombType = 0;
	tLcom.nActive = 1;

	// name, Combination
	switch(nCopyType)
	{
	case 0:  // Main + Combinaton
		{
			tLcom.LoadCombName = tRklc.strCaseName + _T("(Dead Load)");
			tLcom.Description = _T("PSC Bridge Rating Design");

			if(tRklc.arLoadComb.GetSize() > D_LCOM_NUMCOMB)
			{
				ASSERT(0);
				return ;
			}
			tLcom.aCombination.SetSize(tRklc.arLoadComb.GetSize());
			for(int i = 0; i < tRklc.arLoadComb.GetSize(); i++)
			{
				if (i<tRklc.arStaticLoadComb.GetSize()) 
				{
					if (tRklc.arStaticLoadComb[i]==FALSE) continue;       
				}        
				tLcom.aCombination[i] = tRklc.arLoadComb[i];
			}	
		}
		break;
	case 1:  // Main + MV
		{
			tLcom.LoadCombName = tRklc.strCaseName + _T("(Live Load)");
			tLcom.Description = _T("PSC Bridge Rating Design");

			tLcom.aCombination.Add(tRklc.MoveLoadCase);
		}
	break;
	case 2:  // Main + Combinaton + MV
		{
			tLcom.LoadCombName = tRklc.strCaseName;
			tLcom.Description = _T("PSC Bridge Design");

			int nLoadCombSize = tRklc.arLoadComb.GetSize();
			if(nLoadCombSize + 1 > D_LCOM_NUMCOMB)
			{
				ASSERT(0);
				return ;
			}
			
// 			for(int i = 0; i < nLoadCombSize; i++)
// 			{
// 				tLcom.Combination[i] = tRklc.arLoadComb[i];
// 			}	
			tLcom.aCombination.Copy(tRklc.arLoadComb);
			tLcom.aCombination.Add(tRklc.MoveLoadCase);
		}
	break;
	default:
		ASSERT(0);
	}

	//ChangeToUniqueLcomName(pDoc, tLcom.LoadCombName);
}
/*
void CRatingLoadCombUtil::ChangeToUniqueLcomName(CDBDoc* pDoc, CString& strLcomName)
{
	int nCnt = 1;
	CString strNewLcomName;
	strNewLcomName = strLcomName;
	while(1)
	{
		if(!pDoc->m_pAttrCtrl->ExistLcom(D_LCOMTYPE_GENERAL, strNewLcomName))
		{
			strLcomName = strNewLcomName;
			break;
		}
		strNewLcomName.Format(_T("%s-%d"), strLcomName, nCnt);
		nCnt++;
	}
}
*/
void CRatingLoadCombUtil::GetLComByGenTypeCombination(CDBDoc* pDoc, CArray<T_LCOM_D, T_LCOM_D&>& aCombination, CString strLComName, CString Description, T_LCOM_D& tLcomOut)
{
	ASSERT(0); // 스펙 변경으로 인해 안쓰게 되었습니다. (aCombination 배열을 Combinatio으로 가지는 LCom을 Return해 주는 함수 임..)
	if(aCombination.GetSize() == 0)
	{
		ASSERT(0);
		return;
	}
	
	tLcomOut.LoadCombType = 0;
	tLcomOut.nActive = 1;
	tLcomOut.LoadCombName = strLComName;
	tLcomOut.Description = Description;
	
	int nSize = aCombination.GetSize();
	tLcomOut.aCombination.SetSize(nSize);
	for(int i = 0; i < nSize; i++)
	{
		tLcomOut.aCombination[i].AnalType = D_LCOM_CB_GENERAL;	
		tLcomOut.aCombination[i].LoadCaseKey = pDoc->m_pAttrCtrl->GetLcomKey(D_LCOMTYPE_GENERAL, aCombination[i].LoadCombName);
		tLcomOut.aCombination[i].Factor = 1.0;
	}	
}

// General Lcom 중 Rating Case에서 복사된 것들을 찾아주는 함수
// Rating 하중은 Dead,Live,설계하중조합 3 형제가 입력된다.
// Lcom에 따로 플래그 같은걸 안 뚫었기 때문에 이름이랑 Description으로 판단해야 된다 (에잇! 이런걸 만들게 될 줄이야)
void CRatingLoadCombUtil::GetCopiedLcomKFromRatingCase(CArray<T_LCOM_K,T_LCOM_K>& aRatingLcomK, int nMode)
{
	aRatingLcomK.RemoveAll();

	CDBDoc* pDoc = CDBDoc::GetDocPoint(); 
	CArray<T_LCOM_K, T_LCOM_K> aLcomK;
	CArray<T_LCOM_D, T_LCOM_D> aLcomD;
	pDoc->m_pAttrCtrl->GetLcomKeyList(D_LCOMTYPE_GENERAL, aLcomK);
	aLcomD.SetSize(aLcomK.GetSize());
	for(int i=0; i<aLcomK.GetSize(); i++)
	{
		if(!pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_GENERAL, aLcomK[i], aLcomD[i])) { ASSERT(0); aLcomD[i].Initialize(); }
	}

	CString strLCComb, strLCLive;
	CString strDescRate = (nMode==RDPO_STL_MODE) ? _T("Steel Bridge Rating Design") : _T("PSC Bridge Rating Design");
	CString strDescComb = (nMode==RDPO_STL_MODE) ? _T("Steel Bridge Design") : _T("PSC Bridge Design");

	T_LCOM_K DeadK, LiveK, CombK;
	for(int i=0; i<aLcomK.GetSize(); i++)
	{
		DeadK = aLcomK[i];
		if(aLcomD[i].LoadCombName.Find(_T("(Dead Load)"))>=0 && aLcomD[i].Description==strDescRate)
		{
			strLCComb = aLcomD[i].LoadCombName;
			strLCComb.TrimRight(_T("(Dead Load)"));
			strLCLive = strLCComb + _T("(Live Load)");
			
			BOOL bLive=FALSE, bComb=FALSE;
			for(int j=0; j<aLcomK.GetSize(); j++)
			{
				if(i==j) continue;

				if(aLcomD[j].LoadCombName==strLCLive && aLcomD[j].Description==strDescRate)
				{
					bLive = TRUE;
					LiveK = aLcomK[j];
				}

				if(aLcomD[j].LoadCombName==strLCComb && aLcomD[j].Description==strDescComb)
				{
					bComb = TRUE;
					CombK = aLcomK[j];
				}
			}

			// 합격
			if(bLive && bComb)
			{
				aRatingLcomK.Add(DeadK);
				aRatingLcomK.Add(LiveK);
				aRatingLcomK.Add(CombK);
			}
		}
	}
}

// Rating Case의 Moving Load란에 사용된 정적하중을 표시해주는 함수
// nMode = 0:Steel보고서, 1:PSC보고서
// aIsMVStld = StldKeyList와 같은 인덱스 사용함. TRUE이면 Moving Load로 사용됨
int CRatingLoadCombUtil::GetStatusMVStldList(int nMode, CArray<BOOL,BOOL>& aIsMVStld)
{
	int nMvCount = 0;
	
	CDBDoc* pDoc = CDBDoc::GetDocPoint(); 
	CArray<T_STLD_K, T_STLD_K> aStldK;
	CArray<T_RLCS_K, T_RLCS_K> aRlcsK;
	CArray<T_RKLC_K, T_RKLC_K> aRklcK;
	T_RLCS_D RlcsD;
	T_RKLC_D RklcD;

	pDoc->m_pAttrCtrl->GetStldKeyList(aStldK);
	aIsMVStld.SetSize(aStldK.GetSize());
	for(int i=0; i<aStldK.GetSize(); i++)
	{
		aIsMVStld[i] = FALSE;
	}

	if(nMode==RDPO_STL_MODE) 
	{
		pDoc->m_pAttrCtrl2->GetRlcsKeyList(aRlcsK);

		for(int i=0; i<aStldK.GetSize(); i++)
		{
			for(int j=0; j<aRlcsK.GetSize(); j++)
			{
				pDoc->m_pAttrCtrl2->GetRlcs(aRlcsK[j], RlcsD);
				if(RlcsD.MoveLoadCase.LoadCaseKey==aStldK[i] && RlcsD.MoveLoadCase.AnalType==D_LCOM_STATIC)
				{
					aIsMVStld[i] = TRUE;
					nMvCount++;
					break;
				}
			}
		}
	}
	else
	{
		pDoc->m_pAttrCtrl->GetRklcKeyList(aRklcK);
		
		for(int i=0; i<aStldK.GetSize(); i++)
		{
			for(int j=0; j<aRklcK.GetSize(); j++)
			{
				pDoc->m_pAttrCtrl->GetRklc(aRklcK[j], RklcD);
				if(RklcD.MoveLoadCase.LoadCaseKey==aStldK[i] && RklcD.MoveLoadCase.AnalType==D_LCOM_STATIC)
				{
					aIsMVStld[i] = TRUE;
					nMvCount++;
					break;
				}
			}
		}
	}


	return nMvCount;
}

void CRatingLoadCombUtil::CopyPSCAssessmentLCom(CDBDoc* pDoc, CArray<T_ALCS_K, T_ALCS_K>& aAlcsK)
{
	T_ALCS_D AlcsD;
	AlcsD.Initialize();
	CString strNewName;

	CArray<T_ALCS_K, T_ALCS_K> aAlcsKAll;
	
	for (int i = 0; i < aAlcsK.GetSize(); i++)
	{
		if(!pDoc->m_pAttrCtrl2->GetAlcs(aAlcsK[i], AlcsD)) ASSERT(0);
		
		GetNewAlcsName(pDoc, AlcsD.strCaseName, strNewName);

		AlcsD.strCaseName = strNewName;

		pDoc->m_pAttrCtrl2->GetAlcsKeyList(aAlcsKAll);
		T_ALCS_K NewAlcsK = aAlcsKAll[aAlcsKAll.GetSize() - 1] + 1;
		if(!pDoc->m_pDataCtrl->AddAlcs(NewAlcsK, AlcsD))
		{
			ASSERT(0);
		}
	}
}

void CRatingLoadCombUtil::CopySteelAssessmentLCom(CDBDoc* pDoc, CArray<T_ALCS_K, T_ALCS_K>& aAlcsK)
{
	T_ALCS_D AlcsD;
	AlcsD.Initialize();
	CString strNewName;

	CArray<T_ALCS_K, T_ALCS_K> aAlcsKAll;

	for (int i = 0; i < aAlcsK.GetSize(); i++)
	{
		if (!pDoc->m_pAttrCtrl2->GetAlcs(aAlcsK[i], AlcsD)) ASSERT(0);

		GetNewAlcsName(pDoc, AlcsD.strCaseName, strNewName);

		AlcsD.strCaseName = strNewName;

		pDoc->m_pAttrCtrl2->GetAlcsKeyList(aAlcsKAll);
		T_ALCS_K NewAlcsK = aAlcsKAll[aAlcsKAll.GetSize() - 1] + 1;
		if (!pDoc->m_pDataCtrl->AddAlcs(NewAlcsK, AlcsD))
		{
			ASSERT(0);
		}
	}
}

void CRatingLoadCombUtil::GetNewAlcsName(CDBDoc* pDoc, CString strName, CString& strNewName)
{
	CArray<T_ALCS_K, T_ALCS_K> aAlcsK;
	pDoc->m_pAttrCtrl2->GetAlcsKeyList(aAlcsK);

	T_ALCS_D AlcsD;	
	strNewName = strName;
	int nCnt = 1;
	BOOL bDone;
	while (true)
	{
		strNewName.Format(_T("%s-%d"), strName, nCnt);
		nCnt++;
		bDone = TRUE;
		for (int i = 0; i < aAlcsK.GetSize(); i++)
		{
			AlcsD.Initialize();
			if(!pDoc->m_pAttrCtrl2->GetAlcs(aAlcsK[i], AlcsD)) ASSERT(0);
			if(AlcsD.strCaseName == strNewName)
			{
				bDone = FALSE;
				break;
			}
		}
		if(bDone)
			return;
	}
}