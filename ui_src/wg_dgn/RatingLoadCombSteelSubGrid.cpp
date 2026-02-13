// RatingLoadCombSubGrid.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "RatingLoadCombSteelSubGrid.h"

#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\AttrCtrl2.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRatingLoadCombSteelSubGrid

CRatingLoadCombSteelSubGrid::CRatingLoadCombSteelSubGrid(int nType)
:CRatingLoadCombCommonSubGrid(nType)
{
	
}

CRatingLoadCombSteelSubGrid::~CRatingLoadCombSteelSubGrid()
{
}

BEGIN_MESSAGE_MAP(CRatingLoadCombSteelSubGrid, CRatingLoadCombCommonSubGrid)
	//{{AFX_MSG_MAP(CRatingLoadCombSteelSubGrid)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CRatingLoadCombSteelSubGrid message handlers
void CRatingLoadCombSteelSubGrid::SetData(CString sKey)
{
	m_sKey = sKey;

	SetLoadCaseColumnStyle(D_COL_RAC_LCASE);
	SetLoadCaseColumnStyle(D_COL_RAC_CHK_OR_FACTOR);
	if (IsMainAppendRow() || IsMainNewRow())
	{
		if (GetRowCount() == 1) return; // already cleared
		//SetRecordCount(0);
		DeleteAllRecords();
		return;
	}

	T_RLCS_K key;
	T_RLCS_D data;

	// load data and fill each row
	key = _ttol(m_sKey);
	//m_pDoc->m_pAttrCtrl->GetLcom(m_nLcomType, key, data);
	m_pDoc->m_pAttrCtrl2->GetRlcs(key, data);

	BOOL bOldLock = LockUpdate(TRUE);
	
	if(m_nGridType == 0)
	{
		//int nCount = m_pDoc->m_pAttrCtrl->GetCountLcomCase(m_nLcomType, data.LoadCombName);
		int nCount = data.arLoadComb.GetSize();
		if (GetRowCount() == 1 && nCount == 0) return;  // already cleared
		
		//////////////////////////////////////////////////////////////////////////
		CArray<T_LCOM_BASE, T_LCOM_BASE&> arLoadComb; arLoadComb.RemoveAll();
		CArray<BOOL, BOOL&> arStaticLoadComb;    arStaticLoadComb.RemoveAll();

		CMap<T_SGLD_K, T_SGLD_K, int, int> mapErect;
		m_pDoc->m_pAttrCtrl->GetErectionSelected(mapErect);

		for (int i = 0; i < data.arLoadComb.GetSize(); i++)
		{
			if (data.arLoadComb[i].AnalType == D_LCOM_STAGE)
			{
				unsigned int SgldKey = data.arLoadComb[i].LoadCaseKey;
				// Erection Load 및 CS로 정의된 것만 보여주기(Tendon 제외).
				if (SgldKey >= D_SGLD_ER1_CVL && SgldKey < D_SGLD_SUM_CVL)
				{
					int nTemp = 0;
					if (!mapErect.Lookup(SgldKey, nTemp)) continue;
				}
			}

			arLoadComb.Add(data.arLoadComb[i]);
			arStaticLoadComb.Add(data.arStaticLoadComb[i]);
		}
		//////////////////////////////////////////////////////////////////////////

		nCount = arLoadComb.GetSize();
		if (GetRowCount() == 1 && nCount == 0) return;  // already cleared

		DeleteAllRecords();
		SetRecordCount(nCount);
		CString sValue;
		int nRangeStart;
		
		for (int i = 0; i < nCount; i++)
		{
			nRangeStart = D_STLD_MAXNUM*arLoadComb[i].AnalType;
			sValue.Format(_T("%d"), nRangeStart+arLoadComb[i].LoadCaseKey);
			SetExpressionRowCol(i+1, D_COL_RAC_LCASE, sValue);

			BOOL bStaticLCom = TRUE;
			bStaticLCom = arStaticLoadComb[i];
			CString strbStaticLCom = bStaticLCom ? _T("1") : _T("0");
			sValue.Format(_T("%s"), strbStaticLCom);
			SetExpressionRowCol(i+1, D_COL_RAC_CHK_OR_FACTOR, sValue);

			sValue.Format(_T("%g"), arLoadComb[i].Factor);
			SetExpressionRowCol(i+1, D_COL_RAC_FACTOR_OR_IMPACTOR, sValue);
		}
	}
	else
	{
		DeleteAllRecords();
		SetRecordCount(1);
		HideRows(2, 2, TRUE);

		CString sValue;
		int nRangeStart;	
		nRangeStart = D_STLD_MAXNUM * data.MoveLoadCase.AnalType;
		sValue.Format(_T("%d"), nRangeStart + data.MoveLoadCase.LoadCaseKey);
		SetExpressionRowCol(1, D_COL_RAC_LCASE, sValue);

		sValue.Format(_T("%g"), data.MoveLoadCase.Factor);
		SetExpressionRowCol(1, D_COL_RAC_CHK_OR_FACTOR, sValue);	

		sValue.Format(_T("%g"), data.dImpFactor);
		SetExpressionRowCol(1, D_COL_RAC_FACTOR_OR_IMPACTOR, sValue);	
	}
	
	LockUpdate(bOldLock);
	if (!bOldLock) Redraw();
}

void CRatingLoadCombSteelSubGrid::GetData(T_RLCS_D &data)
{
	if(m_nGridType == 0)		
	{
		int nAnalType;
		T_STLD_K nKey;
		data.arLoadComb.SetSize(GetRowCount()-1);
		data.arStaticLoadComb.SetSize(GetRowCount()-1);

		for (int i = 0; i < GetRowCount()-1; i++)
		{
			nKey = _ttol(GetValueRowCol(i+1, D_COL_RAC_LCASE));
			nAnalType = nKey / D_STLD_MAXNUM;
			data.arLoadComb[i].LoadCaseKey = nKey % D_STLD_MAXNUM;

			CString strbStaticLCom;
			strbStaticLCom = GetValueRowCol(i+1, D_COL_RAC_CHK_OR_FACTOR);
			BOOL bStaticLCom =  strbStaticLCom == _T("1") ? TRUE : FALSE;     
			data.arStaticLoadComb[i] =bStaticLCom;

			data.arLoadComb[i].Factor = _tstof(GetValueRowCol(i+1, D_COL_RAC_FACTOR_OR_IMPACTOR));

			data.arLoadComb[i].AnalType = nAnalType;
		}		
	}
	else if(m_nGridType == 1)
	{
		int nAnalType;
		T_STLD_K nKey;
		
		nKey = _ttol(GetValueRowCol(1, D_COL_RAC_LCASE));
		nAnalType = nKey / D_STLD_MAXNUM;
		data.MoveLoadCase.LoadCaseKey = nKey % D_STLD_MAXNUM;

		data.MoveLoadCase.Factor = _tstof(GetValueRowCol(1, D_COL_RAC_CHK_OR_FACTOR));

		data.dImpFactor = _tstof(GetValueRowCol(1, D_COL_RAC_FACTOR_OR_IMPACTOR));

		data.MoveLoadCase.AnalType = nAnalType;
	}
	else
	{
		ASSERT(0);
	}
}

CString CRatingLoadCombSteelSubGrid::GetDefFactorVal(int nGridType)
{
	if (nGridType == 0)
	{
		return _T("1.0");
	}
	else if(nGridType == 1)
	{
		return _T("1.0");
	}
	else
	{
		ASSERT(0); return _T("0.0");
	}
}